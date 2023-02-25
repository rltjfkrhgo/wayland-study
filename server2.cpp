// https://wayland-book.com/registry/server-side.html

#include <iostream>
#include <list>
#include <wayland-server.h>

struct MyOutput;

struct ServerState
{
    std::list<MyOutput*> clientOutputs;
};

struct MyOutput
{
    struct wl_resource* resource;
    ServerState* serverState;
};

struct ClientOutput
{
};

static void wlOutputHandleRelease(struct wl_client* client, 
                                  struct wl_resource* resource)
{
    wl_resource_destroy(resource);
}

static void wlOutputHandleResourceDestroy(struct wl_resource *resource)
{
    MyOutput* client_output = static_cast<MyOutput*>(wl_resource_get_user_data(resource));

    // TODO: Clean up resource

    client_output->serverState->clientOutputs.remove(client_output);
}

static void wlOutputHandleBind(struct wl_client* client, 
                               void* data,
                               uint32_t version, 
                               uint32_t id)
{
    ServerState *serverState = static_cast<ServerState*>(data);

    MyOutput *client_output = static_cast<MyOutput*>(calloc(1, sizeof(ClientOutput)));

    struct wl_resource *resource = wl_resource_create(client, 
                                                      &wl_output_interface, 
                                                      wl_output_interface.version, 
                                                      id);

    // 주의! "struct wl_output_interface" vs. "wl_output_interface"
    static const struct wl_output_interface wlOutputImplementation
    {
        // 클라가 release request를 보내면 wlOutputHandleRelease가 실행됨.
        // 그리고 아래에서 destroy로 넘겨준 함수 "wlOutputHandleResourceDestroy" 호출.
        .release = wlOutputHandleRelease,
    };

    // 네 번째 인자 destory는 일종의 소멸자.
    // 클라가 release request 없이 종료해도 호출됨.
    wl_resource_set_implementation(resource, 
                                   &wlOutputImplementation, 
                                   client_output, 
                                   wlOutputHandleResourceDestroy);

    client_output->resource = resource;
    client_output->serverState = serverState;

    serverState->clientOutputs.push_back(client_output);
}

int main()
{
    struct wl_display *display = wl_display_create();
    if(!display)
    {
        std::cerr << "Unable to create Wayland display.\n";
        return 1;
    }

    ServerState state{};

    // register the wl_output global
    // 두 번째 인자 wl_output_interface는 외부 파일에 선언된 전역"변수"다!
    // 클라에서 wl_output을 bind하면 wlOutputHandleBind가 실행된다.
    wl_global_create(display, &wl_output_interface, 1, &state, wlOutputHandleBind);

    const char *socket = wl_display_add_socket_auto(display);
    if(!socket)
    {
        std::cerr << "Unable to add socket to Wayland display.\n";
        return 1;
    }

    std::cout << "Running Wayland display on" << socket << "\n";
    wl_display_run(display);

    wl_display_destroy(display);

    return 0;
}
