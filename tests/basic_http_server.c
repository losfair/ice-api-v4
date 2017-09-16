#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <ice-api-v4/init.h>
#include <ice-api-v4/http.h>
#include <ice-api-v4/stream.h>
#include <ice-api-v4/wstream.h>

void print_kvp(const char *k, const char *v, void *call_with) {
    printf("%s -> %s\n", k, v);
}

void hello_world_callback(
    IceHttpEndpointContext ctx,
    IceHttpRequest req,
    void *call_with
) {
    IceHttpResponse resp = ice_http_response_create();
    const char *body = "Hello world!\n";

    ice_http_response_set_body(resp, (const ice_uint8_t *) body, strlen(body));
    ice_http_server_endpoint_context_end_with_response(ctx, resp);
}

void stream_callback(
    IceHttpEndpointContext ctx,
    IceHttpRequest req,
    void *call_with
) {
    IceHttpResponse resp = ice_http_response_create();
    const char *body = "Hello world! (stream)\n";

    struct IceStreamTxRxPair p;
    ice_stream_create_pair(&p);

    ice_http_response_attach_rstream(resp, p.rx);

    ice_stream_wstream_write(p.tx, (const ice_uint8_t *) body, strlen(body));
    ice_stream_wstream_destroy(p.tx);

    ice_http_server_endpoint_context_end_with_response(ctx, resp);
}

void debug_callback(
    IceHttpEndpointContext ctx,
    IceHttpRequest req,
    void *call_with
) {
    IceHttpResponse resp = ice_http_response_create();
    const char *body = "OK\n";

    ice_http_request_iter_headers(req, print_kvp, NULL);

    ice_http_response_set_body(resp, (const ice_uint8_t *) body, strlen(body));
    ice_http_server_endpoint_context_end_with_response(ctx, resp);
}

void default_callback(
    IceHttpEndpointContext ctx,
    IceHttpRequest req,
    void *call_with
) {
    IceHttpResponse resp = ice_http_response_create();
    const char *body = "Not found\n";

    ice_http_response_set_body(resp, (const ice_uint8_t *) body, strlen(body));
    ice_http_response_set_status(resp, 404);
    ice_http_server_endpoint_context_end_with_response(ctx, resp);
}

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

    IceHttpRouteInfo hello_world_route = ice_http_server_route_create("/hello_world", hello_world_callback, NULL);
    ice_http_server_add_route(server, hello_world_route);

    IceHttpRouteInfo default_route = ice_http_server_route_create("", default_callback, NULL);
    ice_http_server_set_default_route(server, default_route);

    IceHttpRouteInfo debug_route = ice_http_server_route_create("/debug", debug_callback, NULL);
    ice_http_server_add_route(server, debug_route);

    IceHttpRouteInfo stream_route = ice_http_server_route_create("/stream", stream_callback, NULL);
    ice_http_server_add_route(server, stream_route);

    while(1) {
        sleep(10000000);
    }

    return 0;
}
