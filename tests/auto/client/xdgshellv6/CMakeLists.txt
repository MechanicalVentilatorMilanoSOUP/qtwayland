# Generated from xdgshellv6.pro.

#####################################################################
## tst_client_xdgshellv6 Test:
#####################################################################

qt_add_test(tst_client_xdgshellv6
    SOURCES
        ../shared_old/mockcompositor.cpp ../shared_old/mockcompositor.h
        ../shared_old/mockfullscreenshellv1.cpp ../shared_old/mockfullscreenshellv1.h
        ../shared_old/mockinput.cpp ../shared_old/mockinput.h
        ../shared_old/mockiviapplication.cpp ../shared_old/mockiviapplication.h
        ../shared_old/mockoutput.cpp ../shared_old/mockoutput.h
        ../shared_old/mocksurface.cpp ../shared_old/mocksurface.h
        ../shared_old/mockwlshell.cpp ../shared_old/mockwlshell.h
        ../shared_old/mockxdgshellv6.cpp ../shared_old/mockxdgshellv6.h
        tst_xdgshellv6.cpp
    INCLUDE_DIRECTORIES
        ../shared_old
    PUBLIC_LIBRARIES
        Qt::CorePrivate
        Qt::Gui
        Qt::GuiPrivate
        Qt::OpenGL
        Qt::WaylandClientPrivate
        Wayland::Client
        Wayland::Server
        Threads::Threads # special case
)

qt6_generate_wayland_protocol_server_sources(tst_client_xdgshellv6
    FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/3rdparty/protocol/fullscreen-shell-unstable-v1.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/3rdparty/protocol/ivi-application.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/3rdparty/protocol/wayland.xml
        ${CMAKE_CURRENT_SOURCE_DIR}/../../../../src/3rdparty/protocol/xdg-shell-unstable-v6.xml
)
