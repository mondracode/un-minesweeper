#include <curses.h>
#include <iostream>

/*
 to compile (linux):
 clang++ -lcurses -Wall -Werror -std=c++11 snake.cc -o snake

 to compile (windows) (could be done with clang too, but it's a little more cumbersome):
 i686-w64-mingw32-g++ -Iwin/PDCurses -Wall -Werror -static -lpthread -o snake.exe snake.cc pdcurses.a
*/

struct coor {

int w1=1;
int w2=35;
int h1=1;
int h2=98;

};

struct point{
int x; int y;
};
//enum color {
//  my_black = 1,
//  my_white, my_white_bg,
//  my_green, my_green_bg,
//  my_yellow_bg,
//  my_blue_bg
//};

// Takes 3 numbers between 0 and 5 (inclusive) and returns the number a number
// that can be inputed into init_pair.
// R is for Red
// G is for Green
// B is for Blue
// There are in total 6*6*6 different possible colors
short color_from_RGB(char r, char g, char b) {
    return (r*36 + g*6 + b*1) + 16;
}

short grey_from_24(char g) {
    return 216 + g + 16;
}

void square (int y1, int x1, int y2, int x2){

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

void print_cursor(int y, int x){
        point cursor;
        cursor.x=x;
        cursor.y=y;
        mvaddch(cursor.y, cursor.x, ACS_BLOCK);
        mvaddch(cursor.y, cursor.x+1, ACS_BLOCK);
        mvaddch(cursor.y+1, cursor.x, ACS_BLOCK);
        mvaddch(cursor.y+1, cursor.x+1, ACS_BLOCK);
       // mvaddch(cursor.y+1, cursor.x+2, ACS_BLOCK);
       // mvaddch(cursor.y, cursor.x+2, ACS_BLOCK);
}

void game(){


    clear();

   // int cells[gwindow.w2*gwindow.h2];

    square(4,0,36,99);

    point current_pos;

    print_cursor(5,1);

    current_pos.y = 1;
    current_pos.x = 5;

   // print_cursor(current_pos.y+2,1);

    int c = getchar();


   /*while(true){
    switch (c) {
        case KEY_UP:    print_cursor(current_pos.y+2, current_pos.x); break;
        case KEY_DOWN:  print_cursor(current_pos.y-2, current_pos.x); break;
        case KEY_RIGHT: print_cursor(current_pos.y, current_pos.x+2); break;
        case KEY_LEFT:  print_cursor(current_pos.y, current_pos.x-2); break;
    }
   }*/

}


void intro(){
    square(0, 0, 36, 99);
    //los pairs van en (# DEL PAR, COLOR DE TEXTO, COLOR DE FONDO
    init_pair(1, COLOR_BLUE, COLOR_WHITE);
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
          game();
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

  // Setting color pairs
/*  short black = grey_from_24(0);
  init_pair(my_black,     black,                 black                );
  init_pair(my_white,     grey_from_24(23),      black                );
  init_pair(my_white_bg , black,                 grey_from_24(21)     );
  init_pair(my_green,     color_from_RGB(2,3,1), black                );
  init_pair(my_green_bg , black,                 color_from_RGB(2,3,1));
  init_pair(my_yellow_bg, black,                 color_from_RGB(5,4,1));
  init_pair(my_blue_bg,   black,                 color_from_RGB(1,1,5));*/

  //para agregar caracteres en pantalla se insertan de la forma y,x

/*

 ______________Acá comienza mi código en main_____________________


*/

  intro();

  //endwin(); // closing window

  return 0;
}
