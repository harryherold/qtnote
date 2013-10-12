TEMPLATE		 = lib
CONFIG			+= plugin static
INCLUDEPATH		+= ../..
HEADERS			 = plugin.h
SOURCES			 = plugin.cpp
TARGET        	 = $$qtLibraryTarget(pnp_plugin)
DESTDIR			 = .	
