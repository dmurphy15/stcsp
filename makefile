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
YACC = yacc
LDFLAGS = -Wl
endif
endif

YFLAGS = -g2 $(ARCH) -O2 -funroll-loops -fomit-frame-pointer $(DFLAGS)

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
CXX_FLAGS = -std=c++14 -g2 $(ARCH) -Wno-write-strings -Wfatal-errors -Wall -Wextra -Wpedantic -Wconversion -Wshadow

OPT = yes
ifeq ($(OPT),yes)
CXX_FLAGS += -O2 -funroll-loops -fomit-frame-pointer -flto
endif

# name your own custom script here if you want
BIN = mtest
# dir for binaries
BIN_DIR = bin
# Put all auto generated stuff to this build dir.
BUILD_DIR = build
SRC = src
INCLUDE = include

# List of all .cpp source files.
CPP = $(shell find $(SRC) -name "*.cpp")
# All .o files go to build dir.
OBJ = $(CPP:%.cpp=$(BUILD_DIR)/%.o)
# Gcc/Clang will create these .d files containing dependencies.
DEP = $(OBJ:%.o=%.d)


scanner : convenience $(BIN_DIR)/stcsp

$(BIN_DIR)/stcsp: $(BUILD_DIR)/$(SRC)/lex.yy.c $(BUILD_DIR)/$(SRC)/y.tab.cpp $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXX_FLAGS) $^ -o $@ -lboost_coroutine -lboost_context

build : convenience $(BIN_DIR)/$(BIN)

# Actual target of the binary - depends on all .o files.
$(BIN_DIR)/$(BIN) : $(BIN).cpp $(OBJ)
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


$(BUILD_DIR)/$(SRC)/lex.yy.c: $(SRC)/stcsp.l
	mkdir -p $(BUILD_DIR)/$(SRC)
	$(LEX) -o $@ $(SRC)/stcsp.l
$(BUILD_DIR)/$(SRC)/y.tab.cpp: $(SRC)/stcsp.y
	mkdir -p $(BUILD_DIR)/$(SRC)
	$(YACC) -o $@ -d $(SRC)/stcsp.y

.PHONY : clean convenience

# generate convenience header
convenience :
	echo "#pragma once" > $(INCLUDE)/all.h
	for file in $(shell cd include; find constraints \
		expressions -name "*.h") SearchNodeTypes.h Solver.h Variable.h; do \
		echo "#include \"$$file\"" >> $(INCLUDE)/all.h; \
	done

clean :
    # This should remove all generated files.
	-rm $(BIN_DIR)/* $(OBJ) $(DEP)
	-rm $(INCLUDE)/all.h
	-rm $(BUILD_DIR)/$(SRC)/lex.yy.c
	-rm $(BUILD_DIR)/$(SRC)/y.tab.cpp
	-rm $(BUILD_DIR)/$(SRC)/y.tab.hpp
