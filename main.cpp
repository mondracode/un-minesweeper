#include <curses.h>
#include <iostream>

/*
 to compile (linux):
 clang++ -lcurses -Wall -Werror -std=c++11 snake.cc -o snake

 to compile (windows) (could be done with clang too, but it's a little more cumbersome):
 i686-w64-mingw32-g++ -Iwin/PDCurses -Wall -Werror -static -lpthread -o snake.exe snake.cc pdcurses.a
*/


void cuadrado (int y1, int x1, int y2, int x2){

    mvaddch(y1, x1, ACS_ULCORNER);

    //Crea la primera linea
    for(int i=x1+1; i<x2; i++){
        mvaddch(y1, i, ACS_HLINE);
    }
    mvaddch(y1, x2, ACS_URCORNER);

    //lados verticales
    for(int j=y1+1; j<y2-1; j++){
        mvaddch(j, x1, ACS_VLINE);
        mvaddch(j, x2, ACS_VLINE);
    }

    //ultima linea
    mvaddch(y2-1, x1, ACS_LLCORNER);
    for(int k=x1+1; k<x2; k++){
        mvaddch(y2-1, k, ACS_HLINE);
    }
    mvaddch(y2-1, x2, ACS_LRCORNER);
}

void juego(){
    clear();
    cuadrado(0,0,36,99);

}

void intro(){
    cuadrado(0, 0, 36, 99);


    beep();

    //estos son los ojos
    mvaddch(10, 34, ACS_BLOCK);
    mvaddch(10, 35, ACS_BLOCK);
    mvaddch(11, 34, ACS_BLOCK);
    mvaddch(11, 35, ACS_BLOCK);
    mvaddch(10, 33, ACS_BLOCK);
    mvaddch(11, 33, ACS_BLOCK);

    mvaddch(10, 64, ACS_BLOCK);
    mvaddch(10, 65, ACS_BLOCK);
    mvaddch(11, 64, ACS_BLOCK);
    mvaddch(11, 65, ACS_BLOCK);
    mvaddch(11, 66, ACS_BLOCK);
    mvaddch(10, 66, ACS_BLOCK);

    //la boca, supongo
    for(int b=30; b<=35; b++){
        mvaddch(20, b, ACS_BLOCK);
    }

    for(int c=64; c<=69; c++){
        mvaddch(20, c, ACS_BLOCK);
    }
    for(int d=35; d<=41; d++){
        mvaddch(21, d, ACS_BLOCK);
    }
    for(int e=58; e<=64; e++){
        mvaddch(21, e, ACS_BLOCK);
    }
    for(int e=41; e<=58; e++){
        mvaddch(22, e, ACS_BLOCK);
    }
    mvaddch(29, 28, '*');
    addstr("Presiona la barra espaciadora para empezar*");

    char i;

    while(true){

      i = getch();

        if(i==' '){
          juego();
        }

    }

}



int main()
{

  // Configurations for PDCurses in windows (this doesn't affect regular ncurses in linux)
  ttytype[0] = 35;  ttytype[1] = 45; // 35 to 45 lines height
  ttytype[2] = 80;  ttytype[3] = (unsigned char)130; // 80 to 130 characters width

  // Initializing window to print in
  initscr();   // starting screen
  cbreak();
  noecho();    // consume characters without showing them in screen
  curs_set(0); // setting cursor as invisible
  keypad(stdscr, true); // allowing ncurses to convert press keys into curses representation (useful for KEY_UP)
  start_color(); // asking ncurses to use colors

  clear();     // cleaning screen
  nodelay(stdscr, false); // deactivating blocking getch

  //para agregar caracteres en pantalla se insertan de la forma y,x

/*

 ______________Acá comienza mi código en main_____________________


*/

  intro();

  //endwin(); // closing window

  return 0;
}
