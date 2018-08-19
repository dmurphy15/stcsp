UNAME = $(shell uname -s)
ifeq ($(UNAME),SunOS)
LEX = lex
YACC = yacc
LDFLAGS = -ly -ll
else
ifeq ($(UNAME),Linux)
LEX = lex
YACC = yacc
LDFLAGS =
else
LEX = flex
YACC = yacc #byacc
LDFLAGS = -Wl
endif
endif

ARCH = $(shell uname -m)
ifeq ($(UNAME),SunOS)
ARCH = -mcpu=ultrasparc
else
ifeq ($(UNAME),Linux)
ifeq ($(ARCH),i686)
ARCH = -march=pentium4
else
ARCH = -march=nocona
endif
else
ARCH =
endif
endif

CXX = g++
CXX_FLAGS = -std=c++11 -g2 $(ARCH) -Wno-write-strings -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow

OPT = yes
ifeq ($(OPT),yes)
CXX_FLAGS += -O2 -funroll-loops -fomit-frame-pointer
endif

# Final binary
BIN = mtest
# Put all auto generated stuff to this build dir.
BUILD_DIR = build
SRC = src

# List of all .cpp source files.
CPP = $(shell find $(SRC) -name "*.cpp")
# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)

# Default target named after the binary.
#$(BIN) : $(BUILD_DIR)/$(BIN)

# Actual target of the binary - depends on all .o files.
$(BIN) : $(BIN).cpp $(OBJ)
	mkdir -p $(@D)
	# Just link all the object files.
	$(CXX) $(CXX_FLAGS) $^ -o $@ -lboost_coroutine -lboost_context

# Include all .d files
-include $(DEP)

# Build target for every single object file.
# The potential dependency on header files is covered
# by calling `-include $(DEP)`.
$(BUILD_DIR)/%.o : %.cpp
	mkdir -p $(@D)
    # The -MMD flags additionaly creates a .d file with
    # the same name as the .o file.
	$(CXX) $(CXX_FLAGS) -MMD -c $< -o $@

.PHONY : clean
clean :
    # This should remove all generated files.
	-rm $(BIN) $(OBJ) $(DEP)