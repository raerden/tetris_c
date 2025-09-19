#include <stdio.h>
//#include <locale.h>
#include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"
// #include "./brick_game/tetris/defines.h"
#include "./brick_game/tetris/backend.h"

// #define KEY_LEFT

typedef enum {
    Fsm_Start,
    Fsm_Spawn,
    Fsm_Moving,   //перемещение фигуры игроком
    Fsm_Shifting, //Сдвиг фигуры вниз по таймеру
    Fsm_Attaching,
    Fsm_PauseGame,
    Fsm_GameOver
} fsm_state;

typedef struct {
  int **field;
  int **next;
  int score;
  int high_score;
  int level;
  int speed;
  int pause;
  int **figure;
  int pos_x;
  int pos_y;
  long long time;
} TetrisGameInfo_t;

TetrisGameInfo_t *GetTetrisGameInfo() {
    static TetrisGameInfo_t TetrisGameInfo = {0};
    if (TetrisGameInfo.field == NULL) {
        TetrisGameInfo.field = create_matrix(20, 10);
        TetrisGameInfo.next = create_matrix(4, 4);
        TetrisGameInfo.figure = create_matrix(4, 4);
    }
    return &TetrisGameInfo;
}


void userInput(UserAction_t action, bool hold) {
    TetrisGameInfo_t *TetrisGameInfo = GetTetrisGameInfo();
    switch (action)
    {
    case Pause:
        if (TetrisGameInfo->pause == 1) {
            TetrisGameInfo->pause = 0;
            timeout(10);
        }
        else if (TetrisGameInfo->pause == 0) {
            TetrisGameInfo->pause = 1;
            timeout(-1);
        }
        break;
    case Terminate:
        //TetrisGameInfo->is_playing = false;
        break;
    default:
        break;
    };
}

GameInfo_t updateCurrentState() {
    static GameInfo_t game = {0};
    if (game.field == NULL) {
        game.field = create_matrix(20, 10);
        game.next = create_matrix(4, 4);
    }
    TetrisGameInfo_t *TetrisGameInfo = GetTetrisGameInfo();
    game.pause = TetrisGameInfo->pause;
    if (game.pause == 0)
        game.score += 1;
    return game;
}

int main() {

/*
//Я правильно понимаю логику main()?

// проинциализировали ncurses;
// Отрисовали поле.

//Cоздаем переменные:
UserAction_t action;
bool hold = 0;
bool is_playing = true;
GameInfo_t game = {0};
// иницилазируем структуру game (создать матрицы, уровень, счет и тд)

//Описываю структуру fsm с полями

//Крутимся в бесконечном цикле 
while(is_playing) {
    следим за клавиатурой,
    если нажали влево, {
        присваиваем action = Left;
        вызываем userInput(action, hold);
        userInput меняет статус конечно автомата на move_left
    }
    game = updateCurrentState();
    print_board(&game);
}
*/
    win_init(-1);
    print_board();

    UserAction_t action = 0;
    GameInfo_t game = {0};
    fsm_state fsm;
    
    game = updateCurrentState();

    bool hold = false;
    fsm = Start;
    timeout(10);

    while(action != Terminate) {
        if (process_key(&action, &hold))
            userInput(action, hold);
        

        game = updateCurrentState();
        print_stats(&game);
        if (game.pause == 1) {
            print_pause(&game);
        }
        else
            print_field(&game);
    }


    free_matrix(game.field, 20);
    free_matrix(game.next, 4);
    TetrisGameInfo_t *TetrisGameInfo = GetTetrisGameInfo();
    free_matrix(TetrisGameInfo->field, 20);
    free_matrix(TetrisGameInfo->next, 4);
    free_matrix(TetrisGameInfo->figure, 4);


    win_close();
    return 0;

    game.high_score = 9999;
    game.score = 768;
    game.level = 2;



    srand(time(NULL)); // сброс рандомайзера текущим временем
    // srand(get_time()); // сброс рандомайзера текущим временем


    // рандомная мозайка из всех цветов 1-7
    for (int i=0;i<20;i++)
        for (int j=0;j<10;j++)
            game.field[i][j] = 1 + rand() % 7;// 1 - 7
   
    game.next[0][0] = 1;
    game.next[0][1] = 1;
    game.next[0][2] = 1;
    game.next[0][3] = 1;

    print_stats(&game);
    print_field(&game);
    getch();

    game.pause = 2;
    print_pause(&game);

    free_matrix(game.field, 20);
    free_matrix(game.next, 4);

    
    getch();
    // print_field();
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
            break;
        case 10:
            printw("Key enter");
            break;
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
            // pause_game(fall_delay);
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