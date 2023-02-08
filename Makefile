# build SampleDemo

.PHONY: all exe unittest lib remake clean check distcheck info style stylecheck

# 1st target is the target we build with no args, so i want to have 'all' here.
all: lib exe

MyDemoExe      := np_sample1.exe
MyDemoExe_SRC  := demo1src/no_main.cpp
MyUnitTExe     := test_runner_lt1.exe
MyUnitTExe_SRC := test_lightest/test_runner_lt1.cpp
MyLIB          := libnoprint.a
MyLIB_SRC      := src/no_printf.cpp
MyLIB_INC      := src/no_printf.hpp

INC_PATHS ?= ./inc ./src ./lib/Lightest/include
LIB_PATHS ?= ./lib .
LIB_NAMES ?= $(MyLIB)
##############################################################
MyDemoExe_OBJS  := $(MyDemoExe_SRC:.cpp=.o)
MyUnitTExe_OBJS := $(MyUnitTExe_SRC:.cpp=.o)
MyLIB_OBJS      := $(MyLIB_SRC:.cpp=.o)

AllSrc = $(MyDemoExe_SRC) $(MyUnitTExe_SRC) $(MyLIB_SRC) $(MyLIB_INC)

# -- todo ---
#prep:
#    @mkdir -p ./debug ./release
#clean
#    @rm -rf ./debug ./release
# BLD_DIR=./debug ## ./release
# BLD_OBJS = $(addprefix $(BLD_DIR)/, $(EXE_OBJS))
# BLD_LOBS = $(addprefix $(BLD_DIR)/, $(LIB_OBJS))
# BLD_EXE = $(addprefix $(BLD_DIR)/, $(MyDemoExe))
# BLD_LIB = $(addprefix $(BLD_DIR)/, $(MyLIB))
# $(BLD_DIR)/%.o: %.c
# $(BLD_DIR)/%.o: %.cpp
#
# --- command line: make DEBUG=6 remake // can work with  "export DEBUG=2; make" or "DEBUG=2 make" or "make DEBUG= all" ...
DEBUG ?= 0
ifeq ($(shell expr $(DEBUG) \>= 1), 1)
	CFLAGS=-gdwarf -g3 -DDEBUG=$(DEBUG)
	CXXFLAGS=$(CFLAGS)
	STRIP=touch
else
	CFLAGS=-DNDEBUG
	CXXFLAGS=$(CFLAGS)
	STRIP=strip --strip-debug --strip-unneeded
endif

## just to remember:
## $@ The name of the target file (the one before the colon)
## $< The name of the first (or only) prerequisite file (the first one after the colon)
## $^ The names of all the prerequisite files (space separated)
## $* The stem (the bit which matches the % wildcard in the rule definition like %.o: %.cpp).
## $? ... forgotten the meaning :-(

$(MyLIB): $(MyLIB_OBJS)
	@echo "now linking $^ to $@   by invoking:"
	$(AR) rcsv $(ARFLGS) $@ $?
	$(STRIP) $@

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
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBSEARCH) $(LIB_LINK) -o $@
	$(STRIP) $@


$(MyUnitTExe): $(MyUnitTExe_OBJS) $(MyLIB)
	@echo "unit: MyUnitTExe     =$(MyUnitTExe)"
	@echo "unit: MyUnitTExe_OBJS=$(MyUnitTExe_OBJS)"
	@echo "unit: MyLIB          =$(MyLIB)"
	@echo "unit: linking $^ (and $(strip $(LIB_LINK))) to $@ by invoking:"
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBSEARCH) $(LIB_LINK) -o $@
	$(STRIP) $@


%.o: %.cpp
#	@echo "compiling and linking $< to $@   by invoking:"
	$(CXX) $(CXXFLAGS) -o $@ $(INCSEARCH) -c $<

%.o: %.c
#	@echo "compiling and linking $< to $@   by invoking:"
	$(CC) $(CFLAGS) -o $@ $(INCSEARCH) -c $<


exe: $(MyDemoExe)

unittest: $(MyUnitTExe)

lib: $(MyLIB)

remake: clean all

clean:
	rm -f $(MyDemoExe) $(MyDemoExe_OBJS) $(MyUnitTExe) $(MyUnitTExe_OBJS) $(MyLIB) $(MyLIB_OBJS)

info:
	@echo "compiling $(MyDemoExe_SRC) (and linking against $(LIB_NAMES))"
	@echo -e "to get result: $(MyDemoExe)\n"
	@echo -e "possible invocations:"
	@echo -e "  make DEBUG=3 remake\n  export DEBUG=2; make all\n  DEBUG=1 make exe\n  make DEBUG= all"
	@echo -e "\nknown Makefile arguments are:"
	@echo "all        build all (lib+exe+...)"
	@echo "exe        build only the Demo executable"
	@echo "unittest   build only the Unittest executable"
	@echo "lib        build only the lib"
	@echo "remake     clean + all"
	@echo "clean      clean up result + intermediate files"
	@echo "check      run tests"
	@echo "distcheck  run delivery tests"
	@echo "style      reformat source code to coding conventions"
	@echo "stylecheck run coding conventions"
	@echo "info       this one"

check: clean unittest
	chmod +x $(MyUnitTExe)
	./$(MyUnitTExe) && echo "unit tests OK."

distcheck: exe
	@echo "nothing useful implemented here, so we just see, if the Demo works"
	chmod +x $(MyDemoExe)
	./$(MyDemoExe)

stylecheck:
	@clang-format --version
	@echo "Style Guide check dry run ..."
	@clang-format -i --verbose --dry-run --output-replacements-xml $(AllSrc)
	@echo "Style Guide check verify ..."
	@clang-format -n --Werror $(AllSrc) && echo "Style Guide check OK."

# ignore result, if git has clang 14 (ubuntu-latest / 2204) and I have 11 (MSyS2, ubuntu 2004):
# @clang-format -n --Werror $(AllSrc) && echo "Style Guide check OK." || exit 0

style:
	@clang-format -i --verbose $(AllSrc) && echo "Style Guide reformatting OK."
