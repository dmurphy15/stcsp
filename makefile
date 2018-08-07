UNAME = $(shell uname -s)
ARCH = $(shell uname -m)
CCC = gcc
CPP = g++
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
# LDFLAGS = -Wl, -stack_size,0x40000000, -stack_addr,0xf0000000
LDFLAGS = -Wl
endif
endif

DEBUG = no
ifeq ($(DEBUG), yes)
DFLAGS = -DDEBUG
else
DFLAGS =
endif

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

GPROF = no
ifeq ($(GPROF),yes)
YFLAGS = -g2 -pg $(DFLAGS)
CFLAGS = -g2 -pg -std=gnu99 -Wall -pedantic $(DFLAGS)
else
YFLAGS = -g2 $(ARCH) -O2 -funroll-loops -fomit-frame-pointer $(DFLAGS)
CFLAGS = -std=c++11 -g2 $(ARCH) -O2 -funroll-loops -fomit-frame-pointer -Wall -pedantic -Wno-write-strings $(DFLAGS)
# CFLAGS = -g2 $(ARCH) -O2 -funroll-loops -fomit-frame-pointer -Wall -pedantic -Wno-write-strings -stdlib=libstdc++ $(DFLAGS)
endif

-std=gnu99

.PHONY: clean

execute: stcsp

clean:
	/bin/rm -rf lex.yy.c y.tab.cpp y.tab.h y.tab.c
	/bin/rm -rf *.o
	/bin/rm -rf stcsp

lex.yy.c: stcsp.l
	$(LEX) stcsp.l
lex.yy.o: lex.yy.c y.tab.h
	$(CCC) $(YFLAGS) lex.yy.c -c
y.tab.cpp: stcsp.y
	$(YACC) -d stcsp.y; mv y.tab.c y.tab.cpp
y.tab.h: stcsp.y
	$(YACC) -d stcsp.y
y.tab.o: y.tab.cpp y.tab.h
	$(CPP) $(YFLAGS) y.tab.cpp -c

#GeneratorTemplate.o: GeneratorTemplate.h
#	$(CPP) $(CFLAGS) GeneratorTemplate.h -c

Variable.o: Variable.cpp Variable.h
	$(CPP) $(CFLAGS) Variable.cpp -c
#Expression.o: Expression.cpp Expression.h
#	$(CPP) $(CFLAGS) Expression.cpp -c
#Constraint.o: Constraint.cpp Constraint.h
#	$(CPP) $(CFLAGS) Constraint.cpp -c
SearchNode.o: SearchNode.cpp SearchNode.h
	$(CPP) $(CFLAGS) SearchNode.cpp -c

VariableExpression.o: expressions/VariableExpression.cpp expressions/VariableExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/VariableExpression.cpp -c
ConstantExpression.o: expressions/ConstantExpression.cpp expressions/ConstantExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/ConstantExpression.cpp -c
AddExpression.o: expressions/AddExpression.cpp expressions/AddExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/AddExpression.cpp -c
MultiplyExpression.o: expressions/MultiplyExpression.cpp expressions/MultiplyExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/MultiplyExpression.cpp -c
EqualExpression.o: expressions/EqualExpression.cpp expressions/EqualExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/EqualExpression.cpp -c
NextExpression.o: expressions/NextExpression.cpp expressions/NextExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/NextExpression.cpp -c
FirstExpression.o: expressions/FirstExpression.cpp expressions/FirstExpression.h Expression.h
	$(CPP) $(CFLAGS) expressions/FirstExpression.cpp -c

EqualConstraint.o: constraints/EqualConstraint.cpp constraints/EqualConstraint.h Constraint.h
	$(CPP) $(CFLAGS) constraints/EqualConstraint.cpp -c
PrimitiveNextConstraint.o: constraints/PrimitiveNextConstraint.cpp constraints/PrimitiveNextConstraint.h Constraint.h
	$(CPP) $(CFLAGS) constraints/PrimitiveNextConstraint.cpp -c
PrimitiveUntilConstraint.o: constraints/PrimitiveUntilConstraint.cpp constraints/PrimitiveUntilConstraint.h Constraint.h
	$(CPP) $(CFLAGS) constraints/PrimitiveUntilConstraint.cpp -c
PrimitiveFirstConstraint.o: constraints/PrimitiveFirstConstraint.cpp constraints/PrimitiveFirstConstraint.h Constraint.h
	$(CPP) $(CFLAGS) constraints/PrimitiveFirstConstraint.cpp -c
UntilConstraint.o: constraints/UntilConstraint.cpp constraints/UntilConstraint.h Constraint.h
	$(CPP) $(CFLAGS) constraints/UntilConstraint.cpp -c

GACSearchNode.o: searchNodes/GACSearchNode.cpp searchNodes/GACSearchNode.h SearchNode.h
	$(CPP) $(CFLAGS) searchNodes/GACSearchNode.cpp -c

Solver.o: Solver.cpp Solver.h
	$(CPP) $(CFLAGS) Solver.cpp -c

mtest: Variable.o SearchNode.o VariableExpression.o ConstantExpression.o AddExpression.o MultiplyExpression.o EqualExpression.o NextExpression.o FirstExpression.o EqualConstraint.o PrimitiveNextConstraint.o PrimitiveUntilConstraint.o PrimitiveFirstConstraint.o UntilConstraint.o GACSearchNode.o Solver.o mtest.cpp
	$(CPP) $(CFLAGS) $^ -o $@ -lboost_coroutine -lboost_context

clean:
	rm -rf *.o

stcsp: lex.yy.o y.tab.o Variable.o Expression.o Constraint.o VariableExpression.o ConstantExpression.o AddExpression.o EqualConstraint.o
	$(CPP) $(CFLAGS) $(LDFLAGS) lex.yy.o y.tab.o Variable.o Expression.o Constraint.o VariableExpression.o ConstantExpression.o AddExpression.o EqualConstraint.o -o stcsp

#util.o: util.cpp util.h
#	$(CPP) $(CFLAGS) util.cpp -c
#node.o: node.cpp node.h
#	$(CPP) $(CFLAGS) node.cpp -c
#solver.o: solver.cpp solver.h
#	$(CPP) $(CFLAGS) solver.cpp -c
#variable.o: variable.cpp variable.h
#	$(CPP) $(CFLAGS) variable.cpp -c
#graph.o: graph.cpp graph.h
#	$(CPP) $(CFLAGS) graph.cpp -c
#token.o: token.cpp token.h
#	$(CPP) $(CFLAGS) token.cpp -c
#constraint.o: constraint.cpp constraint.h
#	$(CPP) $(CFLAGS) constraint.cpp -c
#solveralgorithm.o: solveralgorithm.cpp solveralgorithm.h
#	$(CPP) $(CFLAGS) solveralgorithm.cpp -c
##automaton.o: automaton.c automaton.h
##	$(CPP) $(CFLAGS) automaton.c -c
#stcsp: lex.yy.o y.tab.o util.o node.o solver.o variable.o graph.o token.o constraint.o solveralgorithm.o
#	$(CPP) $(CFLAGS) $(LDFLAGS) lex.yy.o y.tab.o node.o util.o solver.o variable.o graph.o token.o constraint.o solveralgorithm.o -o stcsp
#
## solverup.o ac.o automaton.o
#
## solverup.o ac.o automaton.o -o stcsp