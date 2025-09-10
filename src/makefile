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
	$(CC) -Werror gui/cli/frontend.c work.c -lncurses

run: runbuild
	./a.out

mem: runbuild
	valgrind --leak-check=full ./a.out
