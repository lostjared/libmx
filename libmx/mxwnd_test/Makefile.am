bin_PROGRAMS=mxwnd_test surface_test subclass_test resize jpgTest


mxwnd_test_SOURCES=mxwnd_test.cpp
surface_test_SOURCES=surface_test.cpp
subclass_test_SOURCES=subclass_test.cpp
resize_SOURCES=resize.cpp
jpgTest_SOURCES=jpgTest.cpp

mxwnd_test_LDADD=  $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)
surface_test_LDADD=  $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)
subclass_test_LDADD=  $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)
resize_LDADD= $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)
jpgTest_LDADD  = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)

AM_CPPFLAGS = $(CFLAGPREFIX)  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS = $(LIBSDL_RPATH) $(LDPREFIX) #-L../mx -lmx -lpng -ljpeg  -lz $(PLATFORMZ)
 
