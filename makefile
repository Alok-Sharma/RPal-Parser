CC = g++
CFLAGS  = -g

default: parser
	
parser:	parser.o standardizer.o lexer.o cse_machine.o cse_helper.o
	$(CC) $(CFLAGS) -o p1  parser.o standardizer.o lexer.o cse_machine.o cse_helper.o

parser.o:	parser.cpp parser.h lexer.cpp lexer.h standardizer.cpp standardizer.h
	$(CC) $(CFLAGS) -c parser.cpp

standardizer.o:	standardizer.cpp standardizer.h lexer.cpp lexer.h
	$(CC) $(CFLAGS) -c standardizer.cpp

cse_machine.o:	cse_machine.cpp cse_machine.h lexer.cpp lexer.h
	$(CC) $(CFLAGS) -c cse_machine.cpp

cse_helper.o:	cse_helper.cpp cse_machine.h lexer.cpp lexer.h
	$(CC) $(CFLAGS) -c cse_helper.cpp

lexer.o:	lexer.cpp lexer.h parser.cpp parser.h
	$(CC) $(CFLAGS) -c lexer.cpp

clean:
	rm -f *.o p1
