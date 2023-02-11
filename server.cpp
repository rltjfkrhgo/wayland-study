// https://wayland-book.com/

#include <iostream>
#include <wayland-server.h>

int main()
{
    // wl_display: 핵심 객체
    struct wl_display *display = wl_display_create();
    if(!display)
    {
        std::cerr << "Unable to create Wayland display.\n";
        return 1;
    }

    // 환경변수 XDG_RUNTIME_DIR 경로의 소켓
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
