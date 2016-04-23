INCLUDES= $(LIBSDL_CFLAGS) $(all_includes)


lib_LTLIBRARIES=libmx.la

libmx_includedir=$(includedir)/mx
libmx_include_HEADERS=args.h mxclamp.h mx_exception.h mx.h mxinterface.h mxpng.h mxsurface.h  mx_types.h mxwnd.h colorVector.h mxf.h mxjoystick.h \
mxthread.h mx3d_graphics.h mx3d_math.h mx_tok.h mxvector.h mxtimer.h mx_font.h mxscreen.h mxfunction.h mxsemaphore.h  mxpixel.h mxeternal.h \
mxtree.h mxsocket.h mxtype.h mxmemory.h mxjpeg.h mximage.h mximg.h mxttf.h


libmx_la_SOURCES= args.cpp mxclamp.cpp mxinterface.cpp mxpng.cpp mxsurface.cpp mx_types.cpp mxwnd.cpp colorVector.cpp mxfont.cpp pixelbuffer.cpp \
 mxjoystick.cpp mxthread.cpp mx3d_graphics_sdl.cpp mxvector.cpp mxtimer.cpp mx_font.cpp mxscreen.cpp mxfunction.cpp mxsemaphore.cpp  mxpixel.cpp mxeternal.cpp mxtree.h mxsocket.cpp mxmemory.cpp mxjpeg.cpp mximage.cpp mximg.cpp mxttf.cpp






libmx_la_CXXFLAGS= $(stretchInclude) $(CFLAGPREFIX) -O2 
libmx_la_LDFLAGS= $(LDPREFIX) $(PLATFORMZ) $(stretchLink) $(TTF_LINK)  -ljpeg
include_dir=mx/
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ../mx.pc
