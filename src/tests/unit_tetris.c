#include "unit_tetris.h"

START_TEST(startGame_test) {
  startGame();
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  
  ck_assert_int_eq(TetrisGameInfo->level, 1);
}
END_TEST

START_TEST(tetrisToGameInfo_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  GameInfo_t gameInfo = {0};
  TetrisGameInfo->score = 328;
  gameInfo = updateCurrentState();
  ck_assert_int_eq(gameInfo.score, 328);
}
END_TEST

START_TEST(FSM_Spawn_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();

  setState(FSM_Spawn);
  clearMatrix(TetrisGameInfo->figure, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
  updateCurrentState();

  int check = 0;
  for (int j=0; check == 0 && j<FIGURE_FIELD_SIZE; j++)
    if (TetrisGameInfo->figure[0][j] > 0)
      check = 1;
  ck_assert_int_eq(check, 1);
}
END_TEST

START_TEST(gamePause_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  gamePause();
  ck_assert_int_eq(TetrisGameInfo->pause, 1);
  ck_assert_int_eq(TetrisGameInfo->state, FSM_GamePause);
  gamePause();
  ck_assert_int_eq(TetrisGameInfo->pause, 0);
  ck_assert_int_eq(TetrisGameInfo->state, FSM_Moving);
}
END_TEST

START_TEST(gameWin_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  clearMatrix(TetrisGameInfo->field, FIELD_H, FIELD_W);
  TetrisGameInfo->score = 6000;
  TetrisGameInfo->high_score = 6000;

  for (int y = FIELD_H - 1; y > FIELD_H - 5; y--) {
    for (int x = 0; x < FIELD_W; x++)
      TetrisGameInfo->field[y][x] = 1;
  }

  deleteLines();
  ck_assert_int_eq(TetrisGameInfo->score, 7500);
  ck_assert_int_eq(TetrisGameInfo->high_score, 7500);
  ck_assert_int_eq(TetrisGameInfo->pause, 2);
}
END_TEST

START_TEST(gameOver_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  for (int i=0; i < FIELD_W; i++)
    TetrisGameInfo->field[0][i] = 1;
  spawnFigure();
  ck_assert_int_eq(TetrisGameInfo->pause, 3);
}
END_TEST

START_TEST(isFullRow_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  for (int i=0; i < FIELD_W; i++)
    TetrisGameInfo->field[0][i] = 1;

  ck_assert_int_eq(isFullRow(TetrisGameInfo->field,0), true);
}
END_TEST

START_TEST(setState_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  ck_assert_int_eq(TetrisGameInfo->state, FSM_Start);
  setState(FSM_Spawn);
  ck_assert_int_eq(TetrisGameInfo->state, FSM_Spawn);
}
END_TEST

START_TEST(removeRow_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  for (int i=0; i < FIELD_W; i++) {
    TetrisGameInfo->field[0][i] = 1;
    TetrisGameInfo->field[1][i] = 1;
  }

  removeRow(TetrisGameInfo->field,1);

  ck_assert_int_eq(TetrisGameInfo->field[0][0], 0);
}
END_TEST

START_TEST(calcScore_test) {
  ck_assert_int_eq(calcScore(1), 100);
  ck_assert_int_eq(calcScore(2), 300);
  ck_assert_int_eq(calcScore(3), 700);
  ck_assert_int_eq(calcScore(4), 1500);
}
END_TEST

START_TEST(genNextFigure_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  for (int i=0; i < 20; i++) {
    clearMatrix(TetrisGameInfo->next, FIGURE_FIELD_SIZE, FIGURE_FIELD_SIZE);
    genNextFigure();
    int check = 0;
    for (int j=0; check == 0 && j<FIGURE_FIELD_SIZE; j++)
      if (TetrisGameInfo->next[0][j] > 0)
        check = 1;
    ck_assert_int_eq(check, 1);
  }
}
END_TEST

START_TEST(deleteLines_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  clearMatrix(TetrisGameInfo->field, FIELD_H, FIELD_W);
  TetrisGameInfo->score = 0;
  TetrisGameInfo->high_score = 0;

  for (int y = FIELD_H - 1; y > FIELD_H - 5; y--) {
    for (int x = 0; x < FIELD_W; x++)
      TetrisGameInfo->field[y][x] = 1;
  }

  deleteLines();
  ck_assert_int_eq(TetrisGameInfo->score, 1500);
  ck_assert_int_eq(TetrisGameInfo->high_score, 1500);
}
END_TEST

START_TEST(swapInt_test) {
  int x = 10;
  int y = 20;
  swapInt(&x,&y);
  ck_assert_int_eq(x, 20);
  ck_assert_int_eq(y, 10);
}
END_TEST

START_TEST(rotateFigure_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  //горизонтальня палка
  TetrisGameInfo->figure[0][0] = 1;
  TetrisGameInfo->figure[0][1] = 1;
  TetrisGameInfo->figure[0][2] = 1;
  TetrisGameInfo->figure[0][3] = 1;
  TetrisGameInfo->figure_h = 1;
  TetrisGameInfo->figure_w = 4;
  rotateFigure();
  ck_assert_int_eq(TetrisGameInfo->figure_h, 4);
  ck_assert_int_eq(TetrisGameInfo->figure_w, 1);
  ck_assert_int_eq(TetrisGameInfo->figure[0][0], 1);
  ck_assert_int_eq(TetrisGameInfo->figure[0][1], 0);
  ck_assert_int_eq(TetrisGameInfo->figure[0][2], 0);
  ck_assert_int_eq(TetrisGameInfo->figure[0][3], 0);
  ck_assert_int_eq(TetrisGameInfo->figure[1][0], 1);
  ck_assert_int_eq(TetrisGameInfo->figure[2][0], 1);
  ck_assert_int_eq(TetrisGameInfo->figure[3][0], 1);
}
END_TEST

START_TEST(moveLeft_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  spawnFigure();
  int x = TetrisGameInfo->pos_x;
  moveLeft();
  ck_assert_int_eq(TetrisGameInfo->pos_x, x-1);
  for (int i=0; i < 15; i++)//двигаем влево 15 раз.
    moveLeft();
  ck_assert_int_eq(TetrisGameInfo->pos_x, 0);
}
END_TEST

START_TEST(moveRigth_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  spawnFigure();
  int x = TetrisGameInfo->pos_x;
  moveRigth();
  ck_assert_int_eq(TetrisGameInfo->pos_x, x+1);
  for (int i=0; i < 15; i++)//двигаем вправо 15 раз.
    moveRigth();
  ck_assert_int_eq(TetrisGameInfo->pos_x, FIELD_W - TetrisGameInfo->figure_w);
}
END_TEST

START_TEST(moveDown_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  spawnFigure();
  int y = TetrisGameInfo->pos_y;
  moveDown();
  ck_assert_int_eq(TetrisGameInfo->pos_y, y+1);
  for (int i=0; i < 30; i++)//двигаем вниз 30 раз.
    moveDown();
  ck_assert_int_eq(TetrisGameInfo->pos_y, FIELD_H - TetrisGameInfo->figure_h);
}
END_TEST

START_TEST(dropDown_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  dropDown();
  ck_assert_int_eq(TetrisGameInfo->fastDrop, true);
}
END_TEST

Suite *suite_tetris(void) {
  Suite *s = suite_create("suite_tetris");
  TCase *tc = tcase_create("case_tetris_tests");

  tcase_add_test(tc, startGame_test);
  tcase_add_test(tc, tetrisToGameInfo_test);
  tcase_add_test(tc, FSM_Spawn_test);
  tcase_add_test(tc, gamePause_test);
  tcase_add_test(tc, gameWin_test);
  tcase_add_test(tc, gameOver_test);
  tcase_add_test(tc, isFullRow_test);
  tcase_add_test(tc, setState_test);
  tcase_add_test(tc, removeRow_test);
  tcase_add_test(tc, calcScore_test);
  tcase_add_test(tc, genNextFigure_test);
  tcase_add_test(tc, deleteLines_test);
  tcase_add_test(tc, swapInt_test);
  tcase_add_test(tc, rotateFigure_test);
  tcase_add_test(tc, moveLeft_test);
  tcase_add_test(tc, moveRigth_test);
  tcase_add_test(tc, moveDown_test);
  tcase_add_test(tc, dropDown_test);
  suite_add_tcase(s, tc);
  return s;
}

int main(void) {
  srand(time(NULL));

  Suite *test = suite_tetris();

  SRunner *sr = srunner_create(test);
  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);


  srunner_free(sr);
  terminateGame();
  return 0;
}