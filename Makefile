ARGS = -g3
OBJ = main.o getch.o

keyword-counter: $(OBJ)
	cc $(ARGS) -o keyword-counter $(OBJ)

main.o: main.c getch.h
	cc $(ARGS) -c main.c

getch.o: getch.c
	cc $(ARGS) -c getch.c

.PHONY: clean
clean:
	rm -rf *.o keyword-counter
