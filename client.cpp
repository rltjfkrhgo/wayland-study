// https://wayland-book.com/

#include <iostream>
#include <wayland-client.h>

int main()
{
    struct wl_display *display = wl_display_connect(NULL);
    if(!display)
    {
        std::cerr << "Failed to connect to Wayland display.\n";
        return 1;
    }
    std::cout << "Connection established!\n";

    wl_display_disconnect(display);
    return 0;
}
