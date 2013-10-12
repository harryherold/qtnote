HEADERS			= 	interfaces.h \
					core.h
SOURCES			=	core.cpp \
					main.cpp
symbian {
     LIBS           = -lpnp_plugin.lib
 } else {
     LIBS           =  ./plug/libpnp_plugin.a
}

 if(!debug_and_release|build_pass):CONFIG(debug, debug|release) {
    mac:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)_debug
    win32:LIBS = $$member(LIBS, 0) $$member(LIBS, 1)d
 }
