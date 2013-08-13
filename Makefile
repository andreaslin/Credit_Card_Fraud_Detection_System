# Universal makefile for single C++ program
#
# Use gcc flag -MMD (user) or -MD (user/system) to generate dependences among source files.
# Use gmake default rules for commonly used file-name suffixes and make variables names.
#
# % gmake (make)  [ a.out ]

########## Variables ##########

CXX = g++					# compiler
CXXFLAGS = -g -Wall -MMD -DDEBUG			# compiler flags
MAKEFILE_NAME = ${firstword ${MAKEFILE_LIST}}	# makefile name

OBJECTS = main.o Factor.o VEA.o	# object files forming executable
DEPENDS = ${OBJECTS:.o=.d}		# substitute ".o" with ".d"
EXEC = vea						# executable name

########## Targets ##########

.PHONY : clean					# not file names

${EXEC} : ${OBJECTS}				# link step
	${CXX} ${CXXFLAGS} $^ -o $@		# additional object files after FLAGS

${OBJECTS} : ${MAKEFILE_NAME}			# OPTIONAL : changes to this file => recompile

# gmake implicitly generates rules to compile C++ files that generate .o files

-include ${DEPENDS}				# include *.d files containing program dependences

clean :						# remove files that can be regenerated
	rm -f ${OBJECTS}.d ${OBJECTS}.o ${EXEC}
