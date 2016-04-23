bin_PROGRAMS=mxwnd_test surface_test subclass_test resize jpgTest


mxwnd_test_SOURCES=mxwnd_test.cpp
surface_test_SOURCES=surface_test.cpp
subclass_test_SOURCES=subclass_test.cpp
resize_SOURCES=resize.cpp
jpgTest_SOURCES=jpgTest.cpp


AM_CPPFLAGS = -Wall -pedantic $(CFLAGPREFIX)  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS = $(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng -ljpeg  -lz
 
