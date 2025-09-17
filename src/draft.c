/*
официальная форму расчета скорости для уровней
seconds = round(1000 * (0.8 - ((level-1)*0.007) )^(level-1));
выдает такие миллисекунды на уровни от 1-10
1000
793
618
473
355
262
190
135
94
64

эта мне нравится больше
seconds = round(1250 * (0,8 ^ level));
1000
800
640
512
410
328
262
210
168
134

void pause_game(int fall_delay) {
    timeout(-1); // Блокирующий ввод
    mvprintw(10, 10, "PAUSED - Press any key to continue");
    refresh();
    getch();//ожидание любой клавиши. можно сделать только p
    timeout(fall_delay); // Возвращаем таймаут
    clear();//очистка экрана
}


    initscr();
    keypad(stdscr, TRUE);
    curs_set(0);
    int ch2;
    ch2 = getch();
    while (ch2 != 'q') {

        switch (ch2) {

            case KEY_LEFT:
                printw("Key left\n");
                break;
            case KEY_RIGHT:
                printw("Key rigth\n");
                break;
            case KEY_DOWN:
                printw("Key down\n");
                break;
            case 10:
                printw("Key enter 10\n");
                break;
            case KEY_ENTER:
                printw("Key enter KEY_ENTER\n");
                break;
        }
        ch2 = getch();
    }
    endwin();
    return 0;
*/