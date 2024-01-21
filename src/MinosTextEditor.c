/* includes n stuff*/
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <ctype.h>
#include <stdio.h>

/* define stuff*/
#define CTRL_KEY(k) ((k) & 0x1f)
/* data */
struct termios og_termios;

/* terminal */
void die(const char *s)
{
  write(STDOUT_FILENO,"\x1b[2J",4);
  write(STDOUT_FILENO, "\x1b[H", 3);

  perror(s);
  exit(1);
}
void disableRawMode()
{
  if( tcsetattr(STDIN_FILENO,TCSAFLUSH,&og_termios) == -1)
    die("tcsetattr");

}

void enableRawMode() 
{
  if( tcgetattr(STDIN_FILENO, &og_termios) == -1) 
    die("tcgetattr");
  atexit(disableRawMode);

  struct termios raw = og_termios;

  raw.c_oflag &= ~(OPOST);
  raw.c_iflag &= ~(IXON | ICRNL | INPCK | ISTRIP | BRKINT);
  raw.c_iflag &= ~(CS8);
  raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if( tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}


char editorReadKey() 
{
  int nread;
  char c;

  while ((nread = read(STDIN_FILENO, &c, 1)) != 1 ) 
  {
    if (nread == -1 && errno != EAGAIN) 
      die("read");
  }
  return c;
}
/*  output stuff*/
void editorRefreshScreen()
{
  write(STDOUT_FILENO,"\x1b[2J",4);
  write(STDOUT_FILENO, "\x1b[H", 3);
}
/* input stuff */
void editorProcessKeypress()
{
  char c = editorReadKey();
 
  switch (c) {
    case CTRL_KEY('q'):
    write(STDOUT_FILENO,"\x1b[2J",4);
    write(STDOUT_FILENO, "\x1b[H", 3);
    exit(0);
    break;
  }
}

/* init mate */
int main() {
  enableRawMode();


  while (1)
  {
    editorRefreshScreen();
    editorProcessKeypress();
  }

  return 0;
}
