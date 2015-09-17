#########################################################
#                                                       #
#                 Makefile for linux gcc                #
#                                                       #
#########################################################

#########################################################
##
##               specify the include paths
##
#########################################################

INCLUDE_DIR= -I/usr/X11R6/include -I./include

#########################################################
##
##              specify the lib directories
##
#########################################################

LIB_DIR= -L/usr/X11R6/lib -L./lib/

#########################################################
##
##             specify the librarys to link
##
#########################################################

LIBS= -lfmod -lGL -lGLU -lglut -lXmu -lXi -lstdc++ -lm

#########################################################
##
## specify a line to compile the source files. The first
## version produces a release build. The second produces
## a debug build
##
#########################################################

COMPILE= gcc -O2 $(INCLUDE_DIR)

# COMPILE= gcc -g -I  $(INCLUDE_DIR)


#########################################################
##
##               specify the output file
##
#########################################################

OUTPUT=fmodFrequencies

#########################################################
##
##              specify the input objects
##
#########################################################

OBJECTS= main.o


#########################################################
##
##      Everything is dependant on the output file
##
#########################################################

all : $(OUTPUT)


#########################################################
##
##  The output program is dependant on the input objects
##
#########################################################

$(OUTPUT) : $(OBJECTS)
	$(COMPILE) $(OBJECTS) $(LIB_DIR) $(LIBS)   -o $(OUTPUT)


#########################################################
##
##     Compile each *.cpp file into a *.o file.
##
#########################################################

main.o : main.cpp
	$(COMPILE) -c main.cpp

FrameTimer.o : FrameTimer.c FrameTimer.h
	$(COMPILE) -c FrameTimer.c

#########################################################
#
#           A rule to delete all temp files
#
#########################################################

clean:
	rm $(OUTPUT) $(OBJECTS)

