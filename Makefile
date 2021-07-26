COMP    ?= c++
FLAGS   += -Wall -std=c++17
INCLUDE ?= -I /usr/local/include
INCLUDE_ = ${INCLUDE:-I%=-isystem%}
LIB     ?= -L /usr/local/lib -l X11 -l GL -l m -L . -l plot2d
BUILD    = $(COMP) $(FLAGS) $(INCLUDE_) -MMD
BUILD_T  = $(COMP) $(FLAGS) $(INCLUDE) $(LIB)

O ?= work
I  = include
S  = src
T  = tests

MODNAMES = errors functions geometry gl lines plotter view
TESTS    = sin sincos

TESTS := ${TESTS:%=$O/%}
OBJ    = ${MODNAMES:%=$O/%.o}

build : $O $O/plot2d.a

clean :
	rm -Rf $O

install : $O libplot2d.a plot2d.hpp

uninstall :
	rm -f libplot2d.a plot2d.hpp

tests : $O $(TESTS)

-include ${OBJ:.o=.d}

$O :
	mkdir $O

$(OBJ) :
	$(BUILD) -c -o $@ ${@:$O/%.o=$S/%.cpp}

$O/plot2d.a : $(OBJ)
	ar -rsc $O/plot2d.a $(OBJ)

libplot2d.a : $O/plot2d.a
	cp $O/plot2d.a libplot2d.a

plot2d.hpp : $I/plot2d.hpp
	cp $I/plot2d.hpp plot2d.hpp

$(TESTS) : ${@:$O/%=$T/%.cpp} libplot2d.a plot2d.hpp
	$(BUILD_T) -o $@ ${@:$O/%=$T/%.cpp}
