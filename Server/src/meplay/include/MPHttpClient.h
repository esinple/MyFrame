#pragma once
#include <event2/http.h>
#include <event2/event.h>
#include <functional>

namespace meplay {
	class MPHttpClient final
	{
	public:
		//MPHttpClient(const char * ip,int port);
		template<class BaseType>
		MPHttpClient(const char * url, void(BaseType::*cb)(const char *, int),BaseType* pBase)
			: MPHttpClient(url)
		{
			m_cb = std::bind(cb, pBase, std::placeholders::_1, std::placeholders::_2);
		}

		MPHttpClient(const char * url);
		//MPHttpClient(const char * url, std::function<void(const char*, int)>& f);
		
		~MPHttpClient();
	public:
		void Get();
		//void http_request_done(struct evhttp_request *req, void *arg);
	public:
		struct event_base* base;
		struct evhttp_connection* conn;
		struct evhttp_request* req;
		struct evhttp_uri *http_uri;
		std::function<void(const char*,int)>  m_cb;
	};
}