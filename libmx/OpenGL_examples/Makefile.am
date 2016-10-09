bin_PROGRAMS=triangle rot  light rect



triangle_SOURCES=tri.cpp
light_SOURCES=light.cpp
rot_SOURCES=rot.cpp
rect_SOURCES=rect.cpp

AM_CPPFLAGS= $(CFLAGPREFIX)  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS= $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)

