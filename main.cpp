#include <curses.h>
#include "lib/randutils.hh"
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
  my_white, gblack, my_white_bg,
  my_green, my_green_bg,
  my_yellow_bg,
  my_blue_bg, my_red_bg, original,
  my_blue, clicked, flagged
};

short color_from_RGB(char r, char g, char b) {
// Takes 3 numbers between 0 and 5 (inclusive) and returns the number a number
// that can be inputed into init_pair.
// R is for Red
// G is for Green
// B is for Blue
// There are in total 6*6*6 different possible colors

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
  init_pair(gblack,     COLOR_WHITE,            COLOR_WHITE          );
  init_pair(my_white_bg,  black,                 grey_from_24(21)     );
  init_pair(my_green,     color_from_RGB(2,3,1), black                );
  init_pair(my_green_bg , black,                 color_from_RGB(2,3,1));
  init_pair(my_yellow_bg, black,                 color_from_RGB(5,4,1));
  init_pair(my_blue_bg,   black,                 color_from_RGB(1,1,5));
  init_pair(my_red_bg,    black,                 color_from_RGB(4.5,0,0));
  init_pair(original,     COLOR_WHITE,           COLOR_BLACK);
  init_pair(my_blue,      COLOR_BLUE,            COLOR_WHITE);
  init_pair(clicked,      COLOR_BLUE,            grey_from_24(15));
  init_pair(flagged,      COLOR_RED,             grey_from_24(15));

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

void print_cursor(point p){
        mvaddch(p.y, p.x, ACS_BLOCK);
        mvaddch(p.y, p.x+1, ACS_BLOCK);
}

void smiley(){
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
}

void draw_mine(){
    color_set(my_black, NULL);

    for(int i=9; i<=10; i++){
     mvaddch(i, 38, ACS_BLOCK);
     mvaddch(i, 39, ACS_BLOCK);
    }
    for(int i=20; i<=21; i++){
     mvaddch(i, 38, ACS_BLOCK);
     mvaddch(i, 39, ACS_BLOCK);
    }
    for(int i=26; i<=29; i++){
     mvaddch(15, i, ACS_BLOCK);
    }
    for(int i=48; i<=51; i++){
     mvaddch(15, i, ACS_BLOCK);
    }
    for(int i =0; i<9; i++){
        for(int j=0; j<17; j++){
            mvaddch(11+i, 30+j, ACS_BLOCK);
            mvaddch(11+i, 31+j, ACS_BLOCK);
        }
    }
    color_set(gblack, nullptr);

    mvaddch(12, 30, ACS_BLOCK);
    mvaddch(12, 31, ACS_BLOCK);
    mvaddch(11, 32, ACS_BLOCK);
    mvaddch(11, 33, ACS_BLOCK);

    mvaddch(18, 30, ACS_BLOCK);
    mvaddch(18, 31, ACS_BLOCK);
    mvaddch(19, 32, ACS_BLOCK);
    mvaddch(19, 33, ACS_BLOCK);

    mvaddch(11, 44, ACS_BLOCK);
    mvaddch(11, 45, ACS_BLOCK);
    mvaddch(12, 46, ACS_BLOCK);
    mvaddch(12, 47, ACS_BLOCK);

    mvaddch(18, 46, ACS_BLOCK);
    mvaddch(18, 47, ACS_BLOCK);
    mvaddch(19, 44, ACS_BLOCK);
    mvaddch(19, 45, ACS_BLOCK);
    color_set(my_white, nullptr);
    mvaddch(13,34, ACS_BLOCK);
    mvaddch(14,34, ACS_BLOCK);
    mvaddch(13,35, ACS_BLOCK);
    mvaddch(14,35, ACS_BLOCK);
    mvaddch(13,36, ACS_BLOCK);
    mvaddch(14,36, ACS_BLOCK);
    mvaddch(13,37, ACS_BLOCK);
    mvaddch(14,37, ACS_BLOCK);


    color_set(my_blue, nullptr);
}
void game(){

    randutils::mt19937_rng rng;

    clear();

    cell gscreen[34][24];

    int rndx;
    int rndy;

    for(int i=0; i<24; i++){

        for(int j=0; j<34.5; j++){
            gscreen[j][i].click=0;
            gscreen[j][i].flag=0;
            gscreen[j][i].mine=0;

            if(gscreen[j][i].click==0){
                mvaddch(i+5, (j+3)*2, '.');
            }
            else if(gscreen[j][i].click==1){
                mvaddch(i+5, (j+3)*2, '#');
            }
        }
    }
        //crea minas aleatorias
        for(int k=0; k<150; k++){
            rndx = rng.uniform(0, 33);
            rndy = rng.uniform(0, 23);
            gscreen[rndx][rndy].mine=1;
            }

//primer frame

    color_set(my_blue, NULL);
    square(4,5,30,74);

    //el ancho es 34*24
    point player;
        player.y =  5;
        player.x = 6;

   print_cursor(player);

//segundo frame en adelante

    bool quit = false;
    while( !quit ) {
    int f = getch();
    int sum = 0;
    switch(f) {
      case KEY_LEFT:        player.x-=2;    break;
      case KEY_RIGHT:       player.x+=2;    break;
      case KEY_UP:          player.y--;     break;
      case KEY_DOWN:        player.y++;     break;
      case 'm':  gscreen[(player.x-3)/2][player.y-5].flag=true;  break;
      case 'n':  gscreen[(player.x-3)/2][player.y-5].flag=false; break;
      case 10:   gscreen[(player.x-3)/2][player.y-5].click=true; break;
                //gscreen[(player.x-3)/2][player.y-5].mine=false;

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

//EL LIENZO MIDE 68(34)*24

for(int i=0; i<25; i++){
    for(int j=0; j<=33; j++){


        /*
            PARA ARREGLAR LO DE LOS BORDES, LLENAR LOS BORDES DE CEROS

        */

        gscreen[j][i].nearby= 48;
        square(4,5,30,74);
        sum=48;
        if(gscreen[j][i].click==0){
            mvaddch(i+5, (j+3)*2, '.');
        }
        else if(gscreen[j][i].click==1){

           if(gscreen[j][i].flag==1 && gscreen[j][i].click==1){
                gscreen[j][i].flag=0;
            }
            else if(gscreen[j][i].mine==1){
                    color_set(my_red_bg, nullptr);
                    mvaddch(i+5, (j+2)*2, ACS_LANTERN);
                    mvaddch(i+5, (j+2.5)*2, ' ');
                    color_set(my_blue, nullptr);
                    mvaddch(i+5, (j+3)*2, '.');
                    gscreen[j][i].flag=0;
                    color_set(my_blue, nullptr);


                    }

                    else {

                              if(gscreen[j+1][i].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j-1][i].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j][i+1].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j][i-1].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j+1][i+1].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j-1][i-1].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j+1][i-1].mine==1){
                                 gscreen[j][i].nearby++;
                            } if(gscreen[j-1][i+1].mine==1){
                                 gscreen[j][i].nearby++;
                            }
                            color_set(clicked, nullptr);
                            if(gscreen[j][i].nearby==48){
                               mvaddch(i+5, (j+2)*2, ' ');
                               mvaddch(i+5, (j+2.5)*2, ' ');
                            }
                            else{
                               mvaddch(i+5, (j+2)*2, gscreen[j][i].nearby );
                               mvaddch(i+5, (j+2.5)*2, ' ');
                            }
                            color_set(my_blue, NULL);
                            mvaddch(i+5, (j+3)*2, '.');

                    }

        }

        if(gscreen[j][i].flag==1){
            color_set(flagged, NULL);
             mvaddch(i+5, (j+2)*2, ' ');
             mvaddch(i+5, (j+2.5)*2,'P');
             color_set(my_blue, nullptr);

        }
     }
    print_cursor(player);
   }
  }
 }
void intro(){
    square(4,5,30,74);
    //los pairs van en (# DEL PAR, COLOR DE TEXTO, COLOR DE FONDO)
    bkgd(COLOR_PAIR(my_blue));

    //smiley();
    draw_mine();

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
  ttytype[2] = 80;  ttytype[3] = (unsigned char)70; // 80 to 130 characters width

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
