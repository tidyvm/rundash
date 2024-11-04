#ifndef LAMINAR_SERVER_H_
#define LAMINAR_SERVER_H_

#include <kj/async-io.h>
#include <kj/compat/http.h>
#include <capnp/message.h>
#include <capnp/capability.h>
#include <functional>
#include <sys/types.h>

struct Laminar;
struct Http;
struct Rpc;

// This class manages the program's asynchronous event loop
class Server final : public kj::TaskSet::ErrorHandler {
public:
    Server(kj::AsyncIoContext& ioContext);
    ~Server();
    void start();
    void stop();

    // add a file descriptor to be monitored for output. The callback will be
    // invoked with the read data
    kj::Promise<void> readDescriptor(int fd, std::function<void(const char*,size_t)> cb);

    void addTask(kj::Promise<void> &&task);
    // add a one-shot timer callback
    kj::Promise<void> addTimeout(int seconds, std::function<void()> cb);

    // get a promise which resolves when a child process exits
    kj::Promise<int> onChildExit(kj::Maybe<pid_t>& pid);

    struct PathWatcher {
        virtual PathWatcher& addPath(const char* path) = 0;
    };

    PathWatcher& watchPaths(std::function<void()>);

    void listenRpc(Rpc& rpc, kj::StringPtr rpcBindAddress);
    void listenHttp(Http& http, kj::StringPtr httpBindAddress);

private:
    kj::Promise<void> acceptRpcClient(Rpc& rpc, kj::Own<kj::ConnectionReceiver>&& listener);
    kj::Promise<void> handleFdRead(kj::AsyncInputStream* stream, char* buffer, std::function<void(const char*,size_t)> cb);

    void taskFailed(kj::Exception&& exception) override;

private:
    int efd_quit;
    kj::AsyncIoContext& ioContext;
    kj::Own<kj::TaskSet> listeners;
    kj::TaskSet childTasks;
    kj::Maybe<kj::Promise<void>> reapWatch;
};

#endif // LAMINAR_SERVER_H_
