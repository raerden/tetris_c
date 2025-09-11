#include <ncurses.h>
#include <time.h>
//#include <locale.h>
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

void freegame(GameInfo_t *game) {
    if (game->field != NULL) {
        for (int i = 0; i < 20; i++)
            free(game->field[i]);
        free(game->field);
    }
    if (game->next != NULL) {
        for (int i = 0; i < 4; i++)
            free(game->next[i]);
        free(game->next);
    }
}

int main() {

    win_init(-1);

    GameInfo_t game = {0};
    print_overlay();

    game.high_score = 9999;
    game.score = 768;
    game.level = 2;

    game.field = (int **)calloc(20, sizeof(int*));
    for(int i=0; i < 20; i++)
        game.field[i] = (int *)calloc(10, sizeof(int));

    // for (int i=0; i < 10; i++)
    //     game.field[i][i] = 1;
    
    // for (int i = 9; i >= 0; i--)
    //       game.field[i+10][i] = 1;

 srand(time(NULL)); // сброс рандомайзера текущим временем

    // рандомная мозайка из всех цветов 1-7
    for (int i=0;i<20;i++)
        for (int j=0;j<10;j++)
            game.field[i][j] = 1 + rand() % 7;// 1 - 7

    

    game.next = (int **)calloc(4, sizeof(int*));
    for(int i=0; i < 4; i++)
        game.next[i] = (int *)calloc(4, sizeof(int));

    game.next[0][0] = 1;
    game.next[0][1] = 1;
    game.next[0][2] = 1;
    game.next[0][3] = 1;

    // print_stats(&game);

    // print_board(&game);
    // getch();

    game.pause = 3;
    print_pause(&game);

    freegame(&game);

    
    // printw("Speed = \n");
    getch();

    // clear_board();

    // getch();

    win_close();
    return 0;

    // напечатать заданные цветовые пары
    // for (int i = 1; i < 8; i++){
    //     MVPRINTW(i, 0 , "%d", i);
    //     attron(COLOR_PAIR(i));
    //     MVADDCH(i, 4 , ' ');
    //     MVADDCH(i, 5 , ' ');
    //     attroff(COLOR_PAIR(i));
    // }

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