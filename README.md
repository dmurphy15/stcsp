# stcsp
Stream Constraint Satisfaction Problem solver. 

To make the program-parsing executable, simply "make" in the directory, resulting in ./bin/stcsp as the executable. Compilation requires working versions of lex and yacc, as well as the boost coroutine and context libraries.

Syntax:

./bin/stcsp input_filename

Solution automata are printed to the terminal as well as written to the file solusions.dot in the DOT graph description language. There are plenty of software freely available to plot the automata from a .dot file. For example, Graphviz is available for Macs, and I find the linux command "sfdp -x -Goverlap=scale -Tpng solutions.dot > solutions.png" to be very helpful.

You can also make your own constraint program directly using the classes provided. Typing "make convenience" will build a convenience header at include/all.h, which includes the necessary classes. Take a look at mtest.cpp to see how you can write your own script without having to use the program parser. Then replace "mtest" in the makefile with the name of your executable, and type "make build" to build your own executable.
