#ifndef LAMINAR_HTTP_H_
#define LAMINAR_HTTP_H_

#include <kj/memory.h>
#include <kj/compat/http.h>
#include <string>
#include <set>

// Definition needed for musl
typedef unsigned int uint;
typedef unsigned long ulong;

struct Laminar;
struct Resources;
struct LogWatcher;
struct EventPeer;

class Http : public kj::HttpService {
public:
    Http(Laminar&li);
    virtual ~Http();

    kj::Promise<void> startServer(kj::Timer &timer, kj::Own<kj::ConnectionReceiver> &&listener);

    void notifyEvent(const char* data, std::string job = nullptr);
    void notifyLog(std::string job, uint run, std::string log_chunk, bool eot);

    // Allows supplying a custom HTML template. Pass an empty string to use the default.
    void setHtmlTemplate(std::string tmpl = std::string());

private:
    virtual kj::Promise<void> request(kj::HttpMethod method, kj::StringPtr url, const kj::HttpHeaders& headers,
                                      kj::AsyncInputStream& requestBody, Response& response) override;
    bool parseLogEndpoint(kj::StringPtr url, std::string &name, uint &num);

    // With SSE, there is no notification if a client disappears. Also, an idle
    // client must be kept alive if there is no activity in their MonitorScope.
    // Deal with these by sending a periodic keepalive and reaping the client if
    // the write fails.
    kj::Promise<void> cleanupPeers(kj::Timer &timer);

    Laminar& laminar;
    std::set<EventPeer*> eventPeers;
    kj::Own<kj::HttpHeaderTable> headerTable;
    kj::Own<Resources> resources;
    std::set<LogWatcher*> logWatchers;

    kj::HttpHeaderId ACCEPT;
};

#endif //LAMINAR_HTTP_H_
