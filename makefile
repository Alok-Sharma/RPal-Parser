CC = g++
CFLAGS  = -g

default: parser
	
parser:	parser.o standardizer.o lexer.o
	$(CC) $(CFLAGS) -o p1  parser.o standardizer.o lexer.o

parser.o:	parser.cpp parser.h lexer.cpp lexer.h standardizer.cpp standardizer.h
	$(CC) $(CFLAGS) -c parser.cpp

standardizer.o:	standardizer.cpp standardizer.h lexer.cpp lexer.h
	$(CC) $(CFLAGS) -c standardizer.cpp

lexer.o:	lexer.cpp lexer.h parser.cpp parser.h
	$(CC) $(CFLAGS) -c lexer.cpp

clean:
	rm -f *.o p1
