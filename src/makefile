# set the C compiler, for C++ use g++
# use this setting on most sane machines
CC = gcc

# use this line on the engin linux machines
# CC = gcc296

# set the path to the include directory
INCDIR =../include

# set the flags for the C and C++ compiler to give lots of warnings
CFLAGS = -I$(INCDIR) -I/opt/local/include -g -O2 -Wall -Wstrict-prototypes -Wnested-externs -Wmissing-prototypes -Wmissing-declarations
CPPFLAGS = $(CFLAGS)

# path to the object file directory
ODIR = obj

# path to the lib directory
LIBDIR =../lib

# path to the bin directory
BINDIR =../bin

# libraries to include
LIBS = -lm -limageIO
LFLAGS = -L$(LIBDIR) -L/usr/local/lib

# put all of the relevant include files here
_DEPS = ppmIO.h image.h gif.h color.h point.h line.h shape.h list.h polygon.h plyRead.h vector.h matrix.h view.h lighting.h drawstate.h bezier.h module.h graphics.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = test-lighting-shading test9bez test9a test9b test9c test9d test9e test9f test9g

# put a list of all the object files here for all executables (with .o endings)
_OBJ = test-lighting-shading.o test9bez.o test9a.o test9b.o test9c.o test9d.o test9e.o test9f.o test9g.o

# convert them to point to the right place
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

# patterns for compiling source code
# $< is the file that caused the action to occur
$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.C $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all:
	for i in $(EXECUTABLES); do (make $$i) ; done

# put the specific executable rules here

test-lighting-shading: $(ODIR)/test-lighting-shading.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9bez: $(ODIR)/test9bez.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

# test9teapot: $(ODIR)/test9teapot.o
# 	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9a: $(ODIR)/test9a.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9b: $(ODIR)/test9b.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9c: $(ODIR)/test9c.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9d: $(ODIR)/test9d.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9e: $(ODIR)/test9e.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9f: $(ODIR)/test9f.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test9g: $(ODIR)/test9g.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

