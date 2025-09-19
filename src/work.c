#include <stdio.h>
//#include <locale.h>
#include "./gui/cli/defines.h"
#include "./gui/cli/frontend.h"
// #include "./brick_game/tetris/defines.h"
#include "./brick_game/tetris/backend.h"


FSM_State_t *getCurrentFSMState() {
    static FSM_State_t State = FSM_Start;
    return &State;
}

TetrisGameInfo_t *getTetrisGameInfo() {
    static TetrisGameInfo_t TetrisGameInfo = {0};
    if (TetrisGameInfo.field == NULL) {
        TetrisGameInfo.field = create_matrix(FIELD_H, FIELD_W);
        TetrisGameInfo.next = create_matrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
        TetrisGameInfo.figure = create_matrix(FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    }
    return &TetrisGameInfo;
}


void userInput(UserAction_t action, bool hold) {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
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

void copyTetrominoToField(GameInfo_t *GameInfo) {
    const TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    for (int y = 0; y < TetrisGameInfo->figure_size; y++)
        for (int x = 0; x < TetrisGameInfo->figure_size; x++) {
            int field_x = x + TetrisGameInfo->pos_x;
            int field_y = y + TetrisGameInfo->pos_y;
// mvprintw(25+y, 1, "pos_x=%d, pos_y=%d", TetrisGameInfo->pos_x, TetrisGameInfo->pos_y);
            if (TetrisGameInfo->figure[y][x] > 0 && field_x < FIELD_W &&  field_y < FIELD_H)
                GameInfo->field[field_y][field_x] = TetrisGameInfo->figure[y][x];
        }
}

void copyTetrisToGameInfo(GameInfo_t *dst) {
    const TetrisGameInfo_t *src = getTetrisGameInfo();
    copy_matrix(dst->field, src->field, FIELD_H, FIELD_W);
    copyTetrominoToField(dst);
    dst->next       = src->next;
    dst->score      = src->score;
    dst->high_score = src->high_score;
    dst->level      = src->level;
    dst->speed      = src->speed;
    dst->pause      = src->pause;
}

GameInfo_t updateCurrentState() {
    static GameInfo_t GameInfo = {0};
    if (GameInfo.field == NULL) {
        GameInfo.field = create_matrix(FIELD_H, FIELD_W);
    }
    copyTetrisToGameInfo(&GameInfo);
    return GameInfo;
}

void spawnFigure() {
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    copy_matrix(TetrisGameInfo->figure, TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    TetrisGameInfo->figure_size = TetrisGameInfo->next_size;
    TetrisGameInfo->pos_x = SPAWN_X - (TetrisGameInfo->figure_size == 4 ? 1 : 0);
    TetrisGameInfo->pos_y = SPAWN_Y;
    GenerateNextFigure(TetrisGameInfo);
}

int main() {
    win_init(-1);

    print_board();

    UserAction_t action = 0;
    GameInfo_t game = {0};
    FSM_State_t *state;
    srand(get_time()); //сброс rand() текущим временем

    GenerateNextFigure(getTetrisGameInfo());
    spawnFigure();
    // game = updateCurrentState();

    bool hold = false;
    
    timeout(10);
    

    // mvprintw(25, 1, "suka");

    while(action != Terminate) {
        state = getCurrentFSMState();// получаем текущее состояние конечного автомата

        if (process_key(&action, &hold))//читаем клавиатуру
            userInput(action, hold);//если было нажатие, меняем FSM или вызываем соотв. функции

        game = updateCurrentState();
        print_stats(&game);
        if (game.pause == 1) {
            print_pause(&game);
        }
        else {
            getTetrisGameInfo()->score += 1;
            print_field(&game);
        }
    }


    free_matrix(game.field, FIELD_H);
    TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
    free_matrix(TetrisGameInfo->field, FIELD_H);
    free_matrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE);
    free_matrix(TetrisGameInfo->figure, FIGURE_FIELD_SIZE);


    win_close();
    return 0;




    // srand(time(NULL)); // сброс рандомайзера текущим временем
    // srand(get_time()); // сброс рандомайзера текущим временем


    // рандомная мозайка из всех цветов 1-7
    // for (int i=0;i<20;i++)
    //     for (int j=0;j<10;j++)
    //         game.field[i][j] = 1 + rand() % 7;// 1 - 7
   
    // game.next[0][0] = 1;
    // game.next[0][1] = 1;
    // game.next[0][2] = 1;
    // game.next[0][3] = 1;

    // print_stats(&game);
    // print_field(&game);
    // getch();

    // game.pause = 2;
    // print_pause(&game);

    // free_matrix(game.field, 20);
    // free_matrix(game.next, 4);

    
    // getch();
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