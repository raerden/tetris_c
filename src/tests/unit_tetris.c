#include "unit_tetris.h"

START_TEST(TetrisGameInfo_init) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  
  ck_assert_int_eq(TetrisGameInfo->field[0][0], 3);
  ck_assert_int_eq(TetrisGameInfo->state, FSM_Start);
}
END_TEST

START_TEST(gamePause_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  gamePause();
  ck_assert_int_eq(TetrisGameInfo->pause, 1);
  ck_assert_int_eq(TetrisGameInfo->state, FSM_GamePause);
}
END_TEST

START_TEST(gameWin_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  gameWin();
  ck_assert_int_eq(TetrisGameInfo->pause, 2);
}
END_TEST

START_TEST(gameOver_test) {
  TetrisGameInfo_t *TetrisGameInfo = getTetrisGameInfo();
  gameOver();
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


Suite *suite_tetris(void) {
  Suite *s = suite_create("suite_tetris");
  TCase *tc = tcase_create("case_tetris_tests");

  tcase_add_test(tc, TetrisGameInfo_init);
  tcase_add_test(tc, gamePause_test);
  tcase_add_test(tc, gameWin_test);
  tcase_add_test(tc, gameOver_test);
  tcase_add_test(tc, isFullRow_test);
  tcase_add_test(tc, removeRow_test);

// #ifdef TEST_MEMORY_FAILURE
//   tcase_add_test(tc, create_matrix_05);
//   tcase_add_test(tc, create_matrix_06);
// #endif
//   tcase_add_test(tc, s21_remove_matrix_01);
  
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