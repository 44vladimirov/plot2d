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

install : $O libplot2d.a

deinstall :
	rm -f libplot2d.a

tests : $O $O/sin

$O :
	mkdir $O

$O/plot2d.o : $S/plot2d.cpp $I/plot2d.hpp
	$(BUILD) -c -o $O/plot2d.o $S/plot2d.cpp

$O/plot2d.a : $O/plot2d.o
	ar -rsc $O/plot2d.a $O/plot2d.o

libplot2d.a : $O/plot2d.a
	cp $O/plot2d.a libplot2d.a

$O/sin : $T/sin.cpp libplot2d.a
	$(BUILD) $(LIB) -o $O/sin $T/sin.cpp
