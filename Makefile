#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
# 'make test'   build and run test executables
#

# define the C compiler to use
CC = gcc

# define any compile-time flags
CFLAGS	:= -Wall -Wextra -Wno-unused-parameter -Wno-unused-value -Wno-missing-field-initializers -Wno-narrowing -Wno-pointer-arith

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS =

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

# define test directory
TEST	:= test

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
TESTDIRS	:= $(TEST)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
TESTDIRS	:= $(shell find $(TEST) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# define the C source files
SOURCES		:= $(wildcard $(patsubst %,%/*.c, $(SOURCEDIRS)))

# define the C object files
OBJECTS		:= $(SOURCES:.c=.o)

# define the dependency output files
DEPS		:= $(OBJECTS:.o=.d)

# define the test source files
TEST_SOURCES	:= $(wildcard $(patsubst %,%/*.c, $(TESTDIRS)))

# define the test object files
TEST_OBJECTS	:= $(TEST_SOURCES:.c=.o)

# define the test dependency output files
TEST_DEPS		:= $(TEST_OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

$(MAIN): $(OBJECTS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# -MMD generates dependency output files same name as the .o file
# (see the gnu make manual section about automatic variables)
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	$(RM) $(call FIXPATH,$(TEST_OBJECTS))
	$(RM) $(call FIXPATH,$(TEST_DEPS))
	@echo Cleanup complete!

run: all
	./$(OUTPUTMAIN)
	@echo Executing 'run: all' complete!

# Test target
test: $(TEST_OBJECTS)
	$(foreach test,$(TEST_SOURCES),$(CC) $(CFLAGS) $(INCLUDES) -o $(call FIXPATH,$(OUTPUT)/$(notdir $(test:.c=))) $(test:.c=.o) $(LFLAGS) $(LIBS); ./$(call FIXPATH,$(OUTPUT)/$(notdir $(test:.c=)));)
	@echo Executing 'test' complete!

# include all test .d files
-include $(TEST_DEPS)

# this is a suffix replacement rule for building test .o's and .d's from .c's
.c.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c -MMD $<  -o $@
