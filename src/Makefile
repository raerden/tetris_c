#This project made by woodsjul
CC = gcc
CFLAGS = -Werror -Wall -Wextra -Wpedantic -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage
CHECK_FLAGS = -lcheck -lm -lpthread
UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
    CHECK_FLAGS += -lsubunit
	OPEN_CMD:=xdg-open
else ifeq ($(shell uname), Darwin)
	OPEN_CMD:=open
	CC := clang
endif

BUILD_DIR = build
LIB_NAME = brick_game.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

SRC_TETRIS = $(wildcard brick_game/tetris/*.c)
SRC_CLI = $(wildcard gui/cli/*.c)
SRC = $(SRC_TETRIS) $(SRC_CLI)

OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))

SRC_TESTS = $(SRC_TETRIS) tests/unit_tetris.c
OBJ_TESTS = $(patsubst %.c, $(OBJ_DIR)/%-test.o, $(SRC_TESTS))
TEST_EXEC = tetris_tests

GCOV_DIR = gcov_report
REPORT_PATH = $(GCOV_DIR)/report/

print:
	@echo $(OBJ)
	@echo $(LIB_PATH)

all: $(LIB_PATH)

$(LIB_PATH): $(OBJ)
	mkdir -p $(BUILD_DIR)
# 	echo "*\c";
	ar rcs $@ $^
# 	echo "*\c";
	ranlib $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
# 	echo "*\c";
	$(CC) $(CFLAGS) -c $< -o $@

install: $(LIB_PATH)
	gcc $(LIB_PATH) $(CFLAGS) -lncurses -g -o tetris

uninstall:
	rm -rf ./tetris

clean: 
	@rm -rf $(BUILD_DIR) $(OBJ_DIR) dvi $(TEST_EXEC) $(GCOV_DIR)

dist:
	tar czf brick_game.tar.gz brick_game/ gui/ brick_game.h *.uml Makefile

dvi:
	mkdir -p dvi
	xelatex -output-directory=dvi documentation.tex

run: install
	./tetris

mem: install
	valgrind --leak-check=full --suppressions=ncurses.supp ./tetris

$(TEST_EXEC): $(OBJ_TESTS)
	@$(CC) $(CFLAGS) -o $@ $^ $(CHECK_FLAGS)

$(OBJ_DIR)/%-test.o: %.c
	@mkdir -p $(@D)
	@echo "*\c";
	@$(CC) $(CFLAGS) -c $< -o $@

test: $(TEST_EXEC)
	@echo "*";
	@echo "	\033[0;92mStart of all the tests...\033[0m"
	@./$(TEST_EXEC)

gcov_report: clean
	mkdir -p $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(GCOV_DIR)/$(TEST_EXEC) $(SRC) $(CHECK_FLAGS) -lncurses
	./$(GCOV_DIR)/$(TEST_EXEC)
	lcov -t "$(GCOV_DIR)/$(TEST_EXEC)" -o $(GCOV_DIR)/coverage.info -c -d .
	lcov --remove $(GCOV_DIR)/coverage.info "*/tests/*" -o $(GCOV_DIR)/coverage.info --ignore-errors unused
	genhtml -o $(REPORT_PATH) $(GCOV_DIR)/coverage.info
	echo "<table width="100%"><td class=\"title\" style=\"color: lightblue; -webkit-text-stroke: 1px black;\">#Project was made by woodsjul</td></table>" >> $(REPORT_PATH)index.html
	@echo "=============================================="
	@echo "Отчёт покрытия успешно сгенерирован!"
	@echo "Откройте в браузере:"
	@echo "file://$(CURDIR)/$(REPORT_PATH)index.html"
	@echo "=============================================="
# 	$(OPEN_CMD) ./$(REPORT_PATH)index.html







fst:
	@$(RM) a.out
	$(CC) -Werror -std=c11 fast.c -lncurses -lm












cpptest:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h
	cppcheck --enable=all --suppress=missingIncludeSystem *.c units/*.c

clang_check:
	cp ../materials/linters/.clang-format .
	clang-format -n *.h tests/*.c tests/*.h units/*.c

clang_fix:
	clang-format -i *.h tests/*.c tests/*.h units/*.c

leaks: buildtest
	leaks -atExit -- ./$(TEST_EXEC) 

valgrind: buildtest
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TEST_EXEC)

.PHONY: all clean test gcov_report

# install xelatex: sudo apt install texlive-xetex texlive-lang-cyrillic fonts-dejavu