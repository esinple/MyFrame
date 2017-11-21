#include "inner_pre.h"

#include "event_loop.h"
#include "event_loop_thread.h"

namespace evpp {

EventLoopThread::EventLoopThread()
    : event_loop_(new EventLoop) {
    MPLOG_TRACE << "loop=" << event_loop_;
}

EventLoopThread::~EventLoopThread() {
    MPLOG_TRACE << "loop=" << event_loop_;
    assert(IsStopped());
    Join();
}

bool EventLoopThread::Start(bool wait_thread_started, Functor pre, Functor post) {
    MPLOG_TRACE;
    status_ = kStarting;

    assert(thread_.get() == nullptr);
    thread_.reset(new std::thread(std::bind(&EventLoopThread::Run, this, pre, post)));

    if (wait_thread_started) {
        while (status_ < kRunning) {
            usleep(1);
        }
    }
    return true;
}

void EventLoopThread::Run(const Functor& pre, const Functor& post) {
    MPLOG_TRACE << "loop=" << event_loop_;
    if (name_.empty()) {
        std::ostringstream os;
        os << "thread-" << std::this_thread::get_id();
        name_ = os.str();
    }


    MPLOG_TRACE << "loop=" << event_loop_ << " execute pre functor.";
    auto fn = [this, pre]() {
        status_ = kRunning;
        if (pre) {
            auto rc = pre();
            if (rc != kOK) {
                event_loop_->Stop();
            }
        }
    };
    event_loop_->QueueInLoop(std::move(fn));
    event_loop_->Run();

    MPLOG_TRACE << "loop=" << event_loop_ << " execute post functor.";
    if (post) {
        post();
    }

    assert(event_loop_->IsStopped());
    MPLOG_TRACE << "loop=" << event_loop_ << " EventLoopThread stopped";
    status_ = kStopped;
}

void EventLoopThread::Stop(bool wait_thread_exit) {
    MPLOG_TRACE << "loop=" << event_loop_ << " wait_thread_exit=" << wait_thread_exit;
    assert(status_ == kRunning && IsRunning());
    status_ = kStopping;
    event_loop_->Stop();

    if (wait_thread_exit) {
        while (!IsStopped()) {
            usleep(1);
        }

        MPLOG_TRACE << "loop=" << event_loop_ << " thread stopped.";
        Join();
        MPLOG_TRACE << "loop=" << event_loop_ << " thread totally stopped.";
    }
    MPLOG_TRACE << "loop=" << event_loop_;
}

void EventLoopThread::Join() {
    // To avoid multi other threads call Join simultaneously
    std::lock_guard<std::mutex> guard(mutex_);
    if (thread_ && thread_->joinable()) {
        MPLOG_TRACE << "loop=" << event_loop_ << " thread=" << thread_ << " joinable";
        try {
            thread_->join();
        } catch (const std::system_error& e) {
            MPLOG_ERROR << "Caught a system_error:" << e.what() << " code=" << e.code();
        }
        thread_.reset();
    }
}

void EventLoopThread::set_name(const std::string& n) {
    name_ = n;
}

const std::string& EventLoopThread::name() const {
    return name_;
}


EventLoop* EventLoopThread::loop() const {
    return event_loop_.get();
}


struct event_base* EventLoopThread::event_base() {
    return loop()->event_base();
}

std::thread::id EventLoopThread::tid() const {
    if (thread_) {
        return thread_->get_id();
    }

    return std::thread::id();
}

bool EventLoopThread::IsRunning() const {
    // Using event_loop_->IsRunning() is more exact to query where thread is
    // running or not instead of status_ == kRunning
    //
    // Because in some particular circumstances,
    // when status_==kRunning and event_loop_::running_ == false,
    // the application will broke down
    return event_loop_->IsRunning();
}

void EventLoopThread::AfterFork() {
    loop()->AfterFork();
}

}
