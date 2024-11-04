#ifndef LAMINAR_RPC_H_
#define LAMINAR_RPC_H_

#include <capnp/ez-rpc.h>
#include <capnp/rpc-twoparty.h>
#include <capnp/rpc.capnp.h>

struct Laminar;

class Rpc {
public:
    Rpc(Laminar&li);
    kj::Promise<void> accept(kj::Own<kj::AsyncIoStream>&& connection);

    capnp::Capability::Client rpcInterface;
};

#endif //LAMINAR_RPC_H_
