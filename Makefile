all: minos

minos: src/MinosTextEditor.c 
	@$(CC) src/MinosTextEditor.c -o Minos -Wall -Wextra -pedantic -std=c99

