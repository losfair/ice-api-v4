#include <stdio.h>
#include <unistd.h>
#include <ice-api-v4/http.h>

int main(int argc, const char *argv[]) {
    if(argc < 2) {
        fprintf(stderr, "Listen address required");
        return 1;
    }

    const char *listen_addr = argv[1];

    IceHttpServerConfig cfg = ice_http_server_config_create();
    ice_http_server_config_set_listen_addr(
        cfg,
        listen_addr
    );
    ice_http_server_config_set_num_executors(
        cfg,
        4
    );

    IceHttpServer server = ice_http_server_create(cfg);
    ice_http_server_start(server);

    while(1) {
        sleep(10000000);
    }

    return 0;
}
