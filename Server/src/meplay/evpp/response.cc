
#include "libevent.h"
#include "conn.h"
#include "conn_pool.h"
#include "response.h"
#include "request.h"

namespace evpp {
namespace httpc {
Response::Response(Request* r, struct evhttp_request* evreq)
    : request_(r), evreq_(evreq), http_code_(0) {
    if (evreq) {
        http_code_ = evreq->response_code;

#if LIBEVENT_VERSION_NUMBER >= 0x02001500
        struct evbuffer* evbuf = evhttp_request_get_input_buffer(evreq);
        size_t buffer_size = evbuffer_get_length(evbuf);
        if (buffer_size > 0) {
            this->body_ = evpp::Slice((char*)evbuffer_pullup(evbuf, -1), buffer_size);
        }
#else
        if (evreq->input_buffer->off > 0) {
            this->body_ = evpp::Slice((char*)evreq->input_buffer->buffer, evreq->input_buffer->off);
        }
#endif
    }
}

Response::~Response() {
}

const char* Response::FindHeader(const char* key) {
    if (http_code_ > 0) {
        assert(this->evreq_);
        return evhttp_find_header(this->evreq_->input_headers, key);
    }
    return nullptr;
}

}
}


