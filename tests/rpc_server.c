#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ice-api-v4/rpc.h>

void add(IceRpcCallContext ctx, void *call_with) {
    assert(ice_rpc_call_context_get_num_params(ctx) == 2);
    IceRpcParam ret = ice_rpc_param_build_i32(
        ice_rpc_param_get_i32(ice_rpc_call_context_get_param(ctx, 0))
        + ice_rpc_param_get_i32(ice_rpc_call_context_get_param(ctx, 1))
    );
    ice_rpc_call_context_end(ctx, ret);
}

int main() {
    IceRpcServerConfig cfg = ice_rpc_server_config_create();
    ice_rpc_server_config_add_method(cfg, "add", add, NULL);
    IceRpcServer server = ice_rpc_server_create(cfg);
    ice_rpc_server_start(server, "127.0.0.1:1653");

    while(1) {
        sleep(10000000);
    }

    return 0;
}
