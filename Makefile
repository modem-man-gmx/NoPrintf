# build SampleDemo

.PHONY: all exe unittest lib remake clean check distcheck info stylecheck

# 1st target is the target we build with no args, so i want to have 'all' here.
all: lib exe

MyDemoExe      := np_sample1.exe
MyDemoExe_SRC  := demo1src/no_main.cpp
MyUnitTExe     := test_runner_lt1.exe
MyUnitTExe_SRC := test_lightest/test_runner_lt1.cpp
MyLIB          := libnoprint.a
MyLIB_SRC      := src/no_printf.cpp
MyLIB_INC      := src/no_printf.hpp

INC_PATHS ?= ./inc ./src ./lib/lightest/include
LIB_PATHS ?= ./lib .
LIB_NAMES ?= $(MyLIB) #libnoprint.a
##############################################################
MyDemoExe_OBJS  := $(MyDemoExe_SRC:.cpp=.o)
MyUnitTExe_OBJS := $(MyUnitTExe_SRC:.cpp=.o)
MyLIB_OBJS      := $(MyLIB_SRC:.cpp=.o)

AllSrc = $(MyDemoExe_SRC) $(MyUnitTExe_SRC) $(MyLIB_SRC) $(MyLIB_INC)

$(MyLIB): $(MyLIB_OBJS)
	@echo "now linking $^ to $@   by invoking:"
	@echo "$(strip $(AR) rcv $(ARFLGS) $@ $?)"
	$(AR) rcsv $(ARFLGS) $@ $?

# --- auto assigning inc paths (space separated) to appropriate list of -I directives
#
INCSEARCH := $(foreach d, $(INC_PATHS), -I$d)

# --- auto assigning lib paths (space separated) to appropriate list of -L directives
#
LIBSEARCH      := $(foreach d, $(LIB_PATHS), -L$d)
LIB_SHORTNAMES := $(LIB_NAMES:lib%=%)
LIB_LINK       := $(foreach d, $(LIB_SHORTNAMES:.a=), -l$d)

$(MyDemoExe): $(MyDemoExe_OBJS) $(MyLIB)
	@echo "demo: linking $^ (and $(strip $(LIB_LINK))) to $@ by invoking:"
#	@echo "$(strip $(CXX) $(CXXFLAGS) -o $@ $(LDFLAGS) $(LIBSEARCH) $^ $(LDLIBS))"
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBSEARCH) $(LIB_LINK) -o $@


$(MyUnitTExe): $(MyUnitTExe_OBJS) $(MyLIB)
	@echo "unit: MyUnitTExe     =$(MyUnitTExe)"
	@echo "unit: MyUnitTExe_OBJS=$(MyUnitTExe_OBJS)"
	@echo "unit: MyLIB          =$(MyLIB)"
	@echo "unit: linking $^ (and $(strip $(LIB_LINK))) to $@ by invoking:"
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBSEARCH) $(LIB_LINK) -o $@


%.o: %.cpp
#	@echo "compiling and linking $< to $@   by invoking:"
	$(CXX) $(CXXFLAGS) -o $@ $(INCSEARCH) -c $<


exe: $(MyDemoExe)

unittest: $(MyUnitTExe)

lib: $(MyLIB)

remake: clean all

clean:
	rm -f $(MyDemoExe) $(MyDemoExe_OBJS) $(MyUnitTExe) $(MyUnitTExe_OBJS) $(MyLIB) $(MyLIB_OBJS)

info:
	@echo "compiling $(MyDemoExe_SRC) (and linking against $(LIB_NAMES))"
	@echo "to get result: $(MyDemoExe)"
	@echo "known Makefile arguments are:"
	@echo "all  		build all (lib+exe+...)"
	@echo "exe   		build only the Demo executable"
	@echo "unittest 	build only the Unittest executable"
	@echo "lib  		build only the lib"
	@echo "remake   	clean + all"
	@echo "clean    	clean up result + intermediate files"
	@echo "check		run tests"
	@echo "distcheck	run delivery tests"
	@echo "stylecheck	run coding conventions"
	@echo "info 		this one"

check: clean unittest
	chmod +x $(MyUnitTExe)
	./$(MyUnitTExe) && echo "unit tests OK."

distcheck: exe
	@echo "nothing useful implemented here, so we just see, if the Demo works"
	chmod +x $(MyDemoExe)
	./$(MyDemoExe)

stylecheck:
	@clang-format -n --Werror $(AllSrc) && echo "Style Guide check OK."

