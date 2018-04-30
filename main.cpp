#include <curses.h>
#include <iostream>

/*
 to compile (linux):
 clang++ -lcurses -Wall -Werror -std=c++11 snake.cc -o snake

 to compile (windows) (could be done with clang too, but it's a little more cumbersome):
 i686-w64-mingw32-g++ -Iwin/PDCurses -Wall -Werror -static -lpthread -o snake.exe snake.cc pdcurses.a
*/

struct coor {

int w1=6;
int w2=74;
int h1=5;
int h2=29;

};

struct point{
int x; int y;
};

struct cell {
    bool mine;
    bool click;
    bool flag;
    int nearby;

};

enum color {
  my_black = 1,
  my_white, my_white_bg,
  my_green, my_green_bg,
  my_yellow_bg,
  my_blue_bg, my_red_bg, original,
  my_blue
};

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

void gcolors(){

// Setting color pairs
//los pairs van en (# DEL PAR, COLOR DE TEXTO, COLOR DE FONDO)
  short black = grey_from_24(0);
  init_pair(my_black,     black,                 black                );
  init_pair(my_white,     grey_from_24(23),      black                );
  init_pair(my_white_bg , black,                 grey_from_24(21)     );
  init_pair(my_green,     color_from_RGB(2,3,1), black                );
  init_pair(my_green_bg , black,                 color_from_RGB(2,3,1));
  init_pair(my_yellow_bg, black,                 color_from_RGB(5,4,1));
  init_pair(my_blue_bg,   black,                 color_from_RGB(1,1,5));
  init_pair(my_red_bg,   COLOR_BLUE,                 color_from_RGB(5,1,0));
  init_pair(original, COLOR_WHITE, COLOR_BLACK);
  init_pair(my_blue, COLOR_BLUE, COLOR_WHITE);

}
void square (int y1, int x1, int y2, int x2){

    mvaddch(y1, x1, ACS_DS_ULCORNER);

    //Crea la primera linea
    for(int i=x1+1; i<x2; i++){
        mvaddch(y1, i, ACS_D_HLINE);
    }
    mvaddch(y1, x2, ACS_DS_URCORNER);

    //lados verticales
    for(int j=y1+1; j<y2-1; j++){
        mvaddch(j, x1, ACS_D_VLINE);
        mvaddch(j, x2, ACS_D_VLINE);
    }

    //ultima linea
    mvaddch(y2-1, x1, ACS_DS_LLCORNER);
    for(int k=x1+1; k<x2; k++){
        mvaddch(y2-1, k, ACS_D_HLINE);
    }
    mvaddch(y2-1, x2, ACS_DS_LRCORNER);
}

void print_cells(){
    //el ancho es 34*24
   for(int j=5; j<29; j++){
           for(int i=6;  i<74; i+=2){
                mvaddch(j, i, '.');
            }
    }

}

void print_cursor(point p){
        mvaddch(p.y, p.x, ACS_BLOCK);
        mvaddch(p.y, p.x+1, ACS_BLOCK);
}

void game(){
    //ACS_LANTERN PARA LAS MINAS
    clear();

    cell gscreen[33][23];
    //color_set(4, NULL);


    for(int i=0; i<23; i++){
        for(int j=0; j<33; j++){
            gscreen[i][j].mine=0;
        }
    }


    square(4,5,30,74);

    //el ancho es 33*23
    point player;
        player.y =  5;
        player.x = 6;

    print_cells();

    print_cursor(player);

    bool quit = false;
    while( !quit ) {
    int f = getch();

    point oldpos;
        oldpos.y=player.y;
        oldpos.x=player.x;

    switch(f) {
      case KEY_LEFT:        player.x-=2;    break;
      case KEY_RIGHT:     player.x+=2;   break;
      case KEY_UP:           player.y--;       break;
      case KEY_DOWN:    player.y++;     break;
      case 10: gscreen[(player.x-6)/2][player.y+5].click=true; break;

      //case KEY_ENTER:     gscreen[player.x+33][player.y+23].click=true;
      //case 'k':    gscreen[player.x+33][player.y+23].click=true;
      //             color_set(my_red_bg, NULL);      break;

      case 'q': quit = true; break;
    }

    //napms(50);
    if(player.x<=4){
        player.x=72;
    }
    else if(player.x>=73){
        player.x=6;
    }
    else if(player.y>=29){
        player.y=5;
    }
    else if(player.y<=4){
        player.y=28;
    }

    clear();
    color_set(my_blue, NULL);
    square(4,5,30,74);
    print_cells();
    if(gscreen[(player.x-6)/2][player.y+5].click==true){

        color_set(my_red_bg, NULL);
    }
    print_cursor(player);

}

}
void intro(){
    square(4,5,30,74);
    //los pairs van en (# DEL PAR, COLOR DE TEXTO, COLOR DE FONDO)
    bkgd(COLOR_PAIR(my_blue));
    //estos son los ojos
    color_set(my_blue, nullptr);
    mvaddch(10, 24, ACS_BLOCK);
    mvaddch(10, 25, ACS_BLOCK);
    mvaddch(11, 24, ACS_BLOCK);
    mvaddch(11, 25, ACS_BLOCK);
    mvaddch(10, 23, ACS_BLOCK);
    mvaddch(11, 23, ACS_BLOCK);

    mvaddch(10, 54, ACS_BLOCK);
    mvaddch(10, 55, ACS_BLOCK);
    mvaddch(11, 54, ACS_BLOCK);
    mvaddch(11, 55, ACS_BLOCK);
    mvaddch(11, 56, ACS_BLOCK);
    mvaddch(10, 56, ACS_BLOCK);

    //la boca, supongo
    for(int b=20; b<=25; b++){
        mvaddch(20, b, ACS_BLOCK);
    }

    for(int c=54; c<=59; c++){
        mvaddch(20, c, ACS_BLOCK);
    }
    for(int d=25; d<=31; d++){
        mvaddch(21, d, ACS_BLOCK);
    }
    for(int e=48; e<=54; e++){
        mvaddch(21, e, ACS_BLOCK);
    }
    for(int e=31; e<=48; e++){
        mvaddch(22, e, ACS_BLOCK);
    }
    mvaddch(26, 18 , '*');
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
  noecho();                                                            // consume characters without showing them in screen
  curs_set(0);                                                         // setting cursor as invisible
  keypad(stdscr, true);                                         // allowing ncurses to convert press keys into curses representation (useful for KEY_UP)
  start_color();                                                      // asking ncurses to use colors
  clear();                                                               // cleaning screen
  nodelay(stdscr, false);                                      // deactivating blocking getch
  gcolors();                                                          // starting color pairs

  //para agregar caracteres en pantalla se insertan de la forma (y,x)

  // ______________Acá comienza mi código en main_____________________

    intro();

//endwin(); // closing window
  getch();
  return 0;
}
