COMP    ?= c++
FLAGS   += -Wall
INCLUDE ?= -I /usr/local/include
BUILD    = $(COMP) $(FLAGS) $(INCLUDE)
LIB     ?= -L /usr/local/lib -l X11 -l GL -l m -L . -l plot2d

O ?= work
I  = include
S  = src
T  = tests

build : $O $O/plot2d.a

clean :
	rm -Rf $O

install : $O libplot2d.a plot2d.hpp

deinstall :
	rm -f libplot2d.a plot2d.hpp

tests : $O $O/sin

$O :
	mkdir $O

$O/errors.o : $S/errors.cpp $I/errors.hpp
	$(BUILD) -c -o $O/errors.o $S/errors.cpp

$O/functions.o : $S/functions.cpp $I/functions.hpp $I/geometry.hpp
	$(BUILD) -c -o $O/functions.o $S/functions.cpp

$O/geometry.o : $S/geometry.cpp $I/geometry.hpp $I/errors.hpp
	$(BUILD) -c -o $O/geometry.o $S/geometry.cpp

$O/gl.o : $S/gl.cpp $I/gl.hpp $I/errors.hpp
	$(BUILD) -c -o $O/gl.o $S/gl.cpp

$O/lines.o : $S/lines.cpp $I/lines.hpp $I/functions.hpp $I/gl.hpp
	$(BUILD) -c -o $O/lines.o $S/lines.cpp

$O/plotter.o : $S/plotter.cpp $I/plotter.hpp $I/scope.hpp $I/gl.hpp \
               $I/errors.hpp
	$(BUILD) -c -o $O/plotter.o $S/plotter.cpp

$O/scope.o : $S/scope.cpp $I/scope.hpp $I/lines.hpp $I/errors.hpp
	$(BUILD) -c -o $O/scope.o $S/scope.cpp

$O/plot2d.a : $O/errors.o $O/functions.o $O/geometry.o $O/gl.o $O/lines.o \
              $O/plotter.o $O/scope.o
	ar -rsc $O/plot2d.a \
	    $O/errors.o $O/functions.o $O/geometry.o $O/gl.o $O/lines.o \
	    $O/plotter.o $O/scope.o

libplot2d.a : $O/plot2d.a
	cp $O/plot2d.a libplot2d.a

plot2d.hpp : $I/plot2d.hpp
	cp $I/plot2d.hpp plot2d.hpp

$O/sin : $T/sin.cpp libplot2d.a plot2d.hpp
	$(BUILD) $(LIB) -o $O/sin $T/sin.cpp
