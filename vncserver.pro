TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    socket.cpp \
    desktop.cpp

HEADERS += \
    vnc/rfb/keysym.h \
    vnc/rfb/rfb.h \
    vnc/rfb/rfbclient.h \
    vnc/rfb/rfbproto.h \
    vnc/rfb/rfbregion.h \
    socket.h \
    desktop.h \
    vnc/rfb/default.h \
    vnc/rfb/rfbssl.h \
    clientdata.h \
    vnc/eventhandler.h \
    vnc/rfb/globaleventhandler.h \
    vnc/radon.h \
    eventhandler.h \
    action.h

DISTFILES += \
    vnc/rfb/rfbconfig.h.cmake \
    vnc/rfb/rfbint.h.cmake

unix: CONFIG += link_pkgconfig
unix: PKGCONFIG += libvncserver
