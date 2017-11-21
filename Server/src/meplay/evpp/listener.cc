#include "inner_pre.h"

#include "listener.h"
#include "event_loop.h"
#include "fd_channel.h"
#include "libevent.h"
#include "sockets.h"
#include "MPLogger.h"

namespace evpp {
Listener::Listener(EventLoop* l, const std::string& addr)
    : loop_(l), addr_(addr) {
    MPLOG_TRACE << "addr=" << addr;
}

Listener::~Listener() {
    MPLOG_TRACE << "fd=" << chan_->fd();
    chan_.reset();
    EVUTIL_CLOSESOCKET(fd_);
    fd_ = INVALID_SOCKET;
}

bool Listener::Listen(int backlog) {
    MPLOG_TRACE;
    fd_ = sock::CreateNonblockingSocket(false);
    if (fd_ < 0) {
        int serrno = errno;
		MP_ERROR("Create a nonblocking socket failed! errno :[%d]", serrno);
		MP_ERROR("%s", strerror(serrno).c_str());
        return false;
    }

    struct sockaddr_storage addr = sock::ParseFromIPPort(addr_.data());
    // TODO Add retry when failed
    int ret = ::bind(fd_, sock::sockaddr_cast(&addr), static_cast<socklen_t>(sizeof(struct sockaddr)));
    if (ret < 0) {
        int serrno = errno;
        MP_ERROR("bind error :[%d] . addr = [%s]", serrno,addr_.c_str());
		MP_ERROR("%s",strerror(serrno).c_str());
        return false;
    }

    ret = ::listen(fd_, backlog);
    if (ret < 0) {
        int serrno = errno;
		MP_ERROR("Listen failed ,errno :[%d]", serrno);
		MP_ERROR("%s", strerror(serrno).c_str());
        return false;
    }
	return true;
}

void Listener::Accept() {
    MPLOG_TRACE;
    chan_.reset(new FdChannel(loop_, fd_, true, false));
    chan_->SetReadCallback(std::bind(&Listener::HandleAccept, this));
    loop_->RunInLoop(std::bind(&FdChannel::AttachToLoop, chan_.get()));
    MPLOG_INFO << "TCPServer is running at " << addr_;
}

void Listener::HandleAccept() {
    MPLOG_TRACE << "A new connection is comming in";
    assert(loop_->IsInLoopThread());
    struct sockaddr_storage ss;
    socklen_t addrlen = sizeof(ss);
    int nfd = -1;
    if ((nfd = ::accept(fd_, sock::sockaddr_cast(&ss), &addrlen)) == -1) {
        int serrno = errno;
        if (serrno != EAGAIN && serrno != EINTR) {
            MPLOG_INFO << __FUNCTION__ << " bad accept " << strerror(serrno);
        }
        return;
    }

    if (evutil_make_socket_nonblocking(nfd) < 0) {
        MPLOG_ERROR << "set fd=" << nfd << " nonblocking failed.";
        EVUTIL_CLOSESOCKET(nfd);
        return;
    }

    sock::SetKeepAlive(nfd, true);

    std::string raddr = sock::ToIPPort(&ss);
    if (raddr.empty()) {
        MPLOG_ERROR << "sock::ToIPPort(&ss) failed.";
        EVUTIL_CLOSESOCKET(nfd);
        return;
    }

    MPLOG_TRACE << "accepted a connection from " << raddr
        << ", listen fd=" << fd_
        << ", client fd=" << nfd;

    if (new_conn_fn_) {
        new_conn_fn_(nfd, raddr, sock::sockaddr_in_cast(&ss));
    }
}

void Listener::Stop() {
    assert(loop_->IsInLoopThread());
    chan_->DisableAllEvent();
    chan_->Close();
}
}
