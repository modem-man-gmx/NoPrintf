# build SampleDemo

.PHONY: all exe lib remake clean info

MyEXE     := np_sample1.exe
MyEXE_SRC := src/no_main.cpp

MyLIB     := libnoprint.a
MyLIB_SRC := src/no_printf.cpp

INC_PATHS ?= ./inc ./src
LIB_PATHS ?= ./lib .
LIB_NAMES ?= libnoprint.a
##############################################################
MyEXE_OBJS := $(MyEXE_SRC:.cpp=.o)
MyLIB_OBJS := $(MyLIB_SRC:.cpp=.o)

all: lib exe

exe: $(MyEXE)

lib: $(MyLIB)

$(MyLIB): $(MyLIB_OBJS)
	@echo "linking $^ to $@   by invoking:"
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

$(MyEXE): $(MyEXE_OBJS) $(MyLIB)
	@echo "linking $^ (and $(strip $(LIB_LINK))) to $@   by invoking:"
#	@echo "$(strip $(CXX) $(CXXFLAGS) -o $@ $(LDFLAGS) $(LIBSEARCH) $^ $(LDLIBS))"
	$(CXX) $(CXXFLAGS) $^ $(LDFLAGS) $(LIBSEARCH) $(LIB_LINK) -o $@


%.o: %.cpp
#	@echo "compiling and linking $< to $@   by invoking:"
	$(CXX) -o $@ $(INCSEARCH) -c $<

remake: clean all

clean:
	rm -f $(MyEXE) $(MyEXE_OBJS) $(LIB_NAMES)

info:
	@echo "compiling $(MyEXE_SRC) (and linking against $(LIB_NAMES))"
	@echo "to get result: $(MyEXE)"
