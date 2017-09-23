#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <ice-api-v4/rpc.h>

void on_recv(const IceRpcParam p, void *call_with) {
    assert(p != NULL);
    int ret = ice_rpc_param_get_i32(p);
    printf("%d\n", ret);
    //exit(0);
}

void on_connect_done(IceRpcClientConnection conn, void *call_with) {
    assert(conn != NULL);
    IceRpcParam params[2];

    for(int i = 0; i < 90000; i++) {
        params[0] = ice_rpc_param_build_i32(0);
        params[1] = ice_rpc_param_build_i32(i);
        ice_rpc_client_connection_call(
            conn,
            "add",
            params,
            2,
            on_recv,
            NULL
        );
        if(i % 16 == 0) usleep(1);
    }
}

int main() {
    IceRpcClient client = ice_rpc_client_create("127.0.0.1:1653");
    ice_rpc_client_connect(client, on_connect_done, NULL);

    while(1) {
        sleep(10000000);
    }

    return 0;
}
