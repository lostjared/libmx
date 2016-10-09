bin_PROGRAMS=mx3d_math
mx3d_math_SOURCES=main.cpp

AM_CPPFLAGS = $(CFLAGPREFIX) -Wall  $(LIBSDL_CFLAGS) $(all_includes) -I../mx
AM_LDFLAGS  =$(LIBSDL_RPATH) $(LIBSDL_LIBS) $(LDPREFIX) -L../mx -lmx -lpng  -lz $(PLATFORMZ)
