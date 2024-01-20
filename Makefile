all: minos

minos: MinosTextEditor.c 
	@$(CC) MinosTextEditor.c -o Minos -Wall -Wextra -pedantic -std=c99

