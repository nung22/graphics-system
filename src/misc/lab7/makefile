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
_DEPS = ppmIO.h image.h gif.h color.h point.h line.h shape.h list.h polygon.h vector.h matrix.h view.h drawstate.h bezier.h module.h graphics.h

# convert them to point to the right place
DEPS = $(patsubst %,$(INCDIR)/%,$(_DEPS))

# put a list of the executables here
EXECUTABLES = create-gif test7a test7b test7c lab7a lab7b lab7c lab7d lab7e lab7f

# put a list of all the object files here for all executables (with .o endings)
_OBJ = create-gif.o test7a.o test7b.o test7c.o lab7a.o lab7b.o lab7c.o lab7d.o lab7e.o lab7f.o

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

create-gif: $(ODIR)/create-gif.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test7a: $(ODIR)/test7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test7b: $(ODIR)/test7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

test7c: $(ODIR)/test7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7a: $(ODIR)/lab7a.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7b: $(ODIR)/lab7b.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7c: $(ODIR)/lab7c.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7d: $(ODIR)/lab7d.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7e: $(ODIR)/lab7e.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

lab7f: $(ODIR)/lab7f.o
	$(CC) -o $(BINDIR)/$@ $^ $(CFLAGS) $(LFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 

