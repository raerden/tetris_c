CC := gcc
CFLAGS = -std=c11 -Wall -Werror -Wpedantic
OBJS_FSMSWITCHCASE := out/frogger.o out/fsm.o out/backend.o out/frontend.o
OBJS_FSMTABLE := out/frogger.o out/fsm_matrix.o out/backend.o out/frontend.o

frogger: $(OBJS_FSMSWITCHCASE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

frogger_fsmtable: $(OBJS_FSMTABLE)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) out/*.o frogger frogger_fsmtable

runbuild:
	@$(RM) a.out
	$(CC) -Werror -std=c11 gui/cli/frontend.c brick_game/tetris/backend.c work.c -lncurses

run: runbuild
	./a.out

mem: runbuild
	valgrind --leak-check=full --suppressions=ncurses.supp ./a.out

fst:
	@$(RM) a.out
	$(CC) -Werror fast.c
	./a.out