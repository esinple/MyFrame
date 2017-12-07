#include "MPHttpClient.h"
#include <event2/event_struct.h>
#include <evhttp.h>
#include "MPLogger.h"

using namespace meplay;

void http_request_done(struct evhttp_request *req, void *arg)
{
	if (req == nullptr)
	{
		MP_ERROR("http_request_error!req is null!");
		return;
	}
	auto pHC = (MPHttpClient*)arg;
	struct evbuffer *evbuf;
	evbuf = evhttp_request_get_input_buffer(req);
	int len = (int)evbuffer_get_length(evbuf);
	auto buff = evbuffer_pullup(evbuf, len);
	pHC->m_cb((char*)buff, len);
	evbuffer_drain(evbuf, len);
	event_base_loopbreak(pHC->base);
}

MPHttpClient::MPHttpClient(const char * url)
//MPHttpClient::MPHttpClient(const char * url, std::function<void(const char*, int)>& f)
	//: m_cb(f)
{
#ifdef WIN_SYSTEM
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(2, 2);

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) 
	{
		MP_ERROR("WSAStartup failed with error: %d\n", err);
	}
#endif
	const char /**scheme,*/ *host, *path, *query;
	char uri[256];
	int port;

	http_uri = evhttp_uri_parse(url);
	host = evhttp_uri_get_host(http_uri);
	port = evhttp_uri_get_port(http_uri);
	path = evhttp_uri_get_path(http_uri);
	if (strlen(path) == 0)
	{
		path = "/";
	}
	query = evhttp_uri_get_query(http_uri);
	if (query == nullptr)
	{
		snprintf(uri, sizeof(uri) - 1, "%s", path);
	}
	else
	{
		snprintf(uri, sizeof(uri) - 1, "%s?%s", path, query);
	}
	uri[sizeof(uri) - 1] = '\0';

	base = event_base_new();
	conn = evhttp_connection_base_new(base, nullptr, host, port);
	req = evhttp_request_new(http_request_done, this);

	evhttp_add_header(req->output_headers, "Host", host);

	evhttp_make_request(conn, req, EVHTTP_REQ_GET, uri);
	evhttp_connection_set_timeout(conn, 3);

}

MPHttpClient::~MPHttpClient()
{
	//evhttp_request_free(req);
	evhttp_uri_free(http_uri);
	evhttp_connection_free(conn);
	event_base_free(base);
#ifdef WIN_SYSTEM
	WSACleanup();
#endif
}

void MPHttpClient::Get()
{
	event_base_dispatch(base);
}
