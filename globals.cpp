// https://wayland-book.com/registry/binding.html

#include <iostream>
#include <wayland-client.h>

static void registryHandleGlobal(void* data, 
                                 struct wl_registry* registry, 
                                 uint32_t name, 
                                 const char* interface, 
                                 uint32_t version)
{
    std::cout << "interface: " << interface << ", version: " << version << 
                 ", name: " << name << std::endl;
}

static void registryHandleGlobalRemove(void* data, 
                                       struct wl_registry* registry, 
                                       uint32_t name)
{
}

int main()
{
    struct wl_display* display = wl_display_connect(NULL);

    struct wl_registry* registry = wl_display_get_registry(display);
/*
--------------------------------------------------------------------------------
  <interface name="wl_display" version="1">
    <request name="sync">
      <arg name="callback" type="new_id" interface="wl_callback"
	   summary="callback object for the sync request"/>
    </request>

    <request name="get_registry">
      <arg name="registry" type="new_id" interface="wl_registry"
	   summary="global registry object"/>
    </request>

    ...
  </interface>
--------------------------------------------------------------------------------
wl_display의 object ID는 1이다.
        objectID  len   opcode  arg name="registry" type="new_id"
C->S    00000001  000C  0001    00000002

이후 서버는 지원하는 interface 정보를 담아서 wl_registry::global event를 보낸다.
--------------------------------------------------------------------------------
  <interface name="wl_registry" version="1">
    <request name="bind">
      <arg name="name" type="uint" summary="unique numeric name of the object"/>
      <arg name="id" type="new_id" summary="bounded object"/>
    </request>

    <event name="global">
      <arg name="name" type="uint" summary="numeric name of the global object"/>
      <arg name="interface" type="string" summary="interface implemented by the object"/>
      <arg name="version" type="uint" summary="interface version"/>
    </event>

    <event name="global_remove">
      <arg name="name" type="uint" summary="numeric name of the global object"/>
    </event>
  </interface>
--------------------------------------------------------------------------------
        objectID  len   opcode
S->C    00000002  001C  0000    
        arg name="name" type="uint"
        00000001 
        arg name="interface" type"string"
        00000007 776C5f73 686d0000           .... wl_s hm..
        arg name="version" type="uint"
        00000001
        [...]
*/

    static const struct wl_registry_listener registryListener
    {
        // registryHandleGlobal로 global event를 수신한다.
        .global = registryHandleGlobal, 
        .global_remove = registryHandleGlobalRemove, 
    };
    wl_registry_add_listener(registry, &registryListener, NULL);

    wl_display_roundtrip(display);

    wl_display_disconnect(display);
    return 0;
}
