#include <ncurses.h>
#include <locale.h>
#include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"

// #define KEY_LEFT

void pause_game(int fall_delay) {
    timeout(-1); // Блокирующий ввод
    mvprintw(10, 10, "PAUSED - Press any key to continue");
    refresh();
    getch();//ожидание любой клавиши. можно сделать только p
    timeout(fall_delay); // Возвращаем таймаут
    clear();//очистка экрана
}


int main() {
    WIN_INIT(-1);

    print_overlay();
    print_stats();
/*
#define COLOR_RED	1      палка
#define COLOR_GREEN	2      N
#define COLOR_YELLOW	3  Г
#define COLOR_BLUE	4      Т
#define COLOR_MAGENTA	5  box
*/
    init_pair(1, COLOR_BLACK, COLOR_RED);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_BLACK, COLOR_YELLOW);
    init_pair(4, COLOR_BLACK, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_MAGENTA);

    attron(COLOR_PAIR(1));

    MVADDCH(1, 1, ' ');
    MVADDCH(1, 2, ' ');
    MVADDCH(1, 3, ' ');
    MVADDCH(1, 4, ' ');
    MVADDCH(1, 5, ' ');
    MVADDCH(1, 6, ' ');
    MVADDCH(1, 7, ' ');
    MVADDCH(1, 8, ' ');

    MVADDCH(2, 9, ' ');
    MVADDCH(2, 10, ' ');
    MVADDCH(2, 11, ' ');
    MVADDCH(2, 12, ' ');
    MVADDCH(2, 13, ' ');
    MVADDCH(2, 14, ' ');
    MVADDCH(2, 15, ' ');
    MVADDCH(2, 16, ' ');


    MVADDCH(20, 1, ' ');
    MVADDCH(20, 1, ' ');

    MVADDCH(1, 19, ' ');
    MVADDCH(1, 20, ' ');

    MVADDCH(10, 20, ' ');
    MVADDCH(20, 20, ' ');

attroff(COLOR_PAIR(1));
attroff(COLOR_PAIR(2));
attroff(COLOR_PAIR(3));
attroff(COLOR_PAIR(4));
attroff(COLOR_PAIR(5));

    // printw("Speed = \n");
    getch();
    // reset_shell_mode();
    //curs_set(1);

    // Правильное завершение
    reset_shell_mode();
    echo();
    nocbreak();
    keypad(stdscr, FALSE);

    endwin();
    return 0;

  WINDOW *win = newwin(10, 10, 1, 1); // Создание нового окна
    box(win, 0, 0); // Рисование рамки вокруг окна
    refresh(); // Обновление экрана

     noecho();// не выводить набираемые символы на экран
     curs_set(0); // Скрывает курсор терминала
     keypad(stdscr, TRUE); // Включает обработку функциональных клавиш (стрелки, F1-F12, etc.)


// print_rectangle(0, 10, 0, 20);


    int fall_delay = 500;
    timeout(fall_delay);//таймаут ожидания ввода getch()

  // Измеряем размер экрана в рядах и колонках
  int row, col;
  getmaxyx(stdscr, row, col);
  
  // перемещение курсора в стандартном экране
//   move(row / 2, col / 2);

  int cont = 1;
  int cnt = 0;
  int ch;
  while(cont) {
    // move(row / 2, col / 2);
    // printw("Hello world cnt = %d\n", cnt); // вывод строки
    
    ch = getch();
    // Обработка ввода пользователя
    switch (ch) {
        case 'q':
            cont = 0;
        case KEY_LEFT:
            //move_piece_left();
            // if (can_move_left()) move_piece_left();
            printw("Key left");
            break;
        case KEY_RIGHT:
            //move_piece_right();
            printw("Key rigth");
            break;
        case KEY_DOWN:
            //move_piece_down();
            printw("Key down");
            break;
        case ' ':
            //rotate_piece();
            printw("Key spacebar");
            break;
        case 'S':
        case 's':
            if (fall_delay > 50) fall_delay -= 50;
            printw("Speed = %d ", fall_delay);
            timeout(fall_delay);
            break;
        case 'X':
        case 'x':
            if (fall_delay < 1000) fall_delay += 50;
            printw("Speed = %d ", fall_delay);
            timeout(fall_delay);
            break;

        case 'p':
            pause_game(fall_delay);
            break;
        case ERR: // Таймаут - автоматическое падение
            //move_piece_down();
            break;
    }
    cnt++;
    // clear();
    refresh(); // обновить экран

  }
  
  endwin(); // завершение работы с ncurses

}