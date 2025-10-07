#This project made by woodsjul
CC = gcc
CFLAGS = -Werror -Wall -Wextra -Wpedantic -std=c11
GCOV_FLAGS = -fprofile-arcs -ftest-coverage -D NO_RUN
CHECK_FLAGS = -lm -lpthread
UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
    CHECK_FLAGS += -lsubunit
	OPEN_CMD:=xdg-open
else ifeq ($(shell uname), Darwin)
	OPEN_CMD:=open
	CC := clang
endif
# Auto-detect check library
CHECK_STATIC := $(wildcard /usr/local/lib/libcheck.a /opt/homebrew/lib/libcheck.a /usr/lib/libcheck.a)
ifneq ($(CHECK_STATIC),)
    CHECK_LIBS := $(firstword $(CHECK_STATIC))
else ifneq ($(shell pkg-config --exists check 2>/dev/null && echo yes),)
    CHECK_LIBS := $(shell pkg-config --libs check)
else
    CHECK_LIBS := -lcheck
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
OBJ_TESTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC_TESTS))
TEST_EXEC = tetris_tests

GCOV_DIR = gcov_report
GCOV_SRC = $(filter-out gui/cli/main.c, $(SRC))
GCOV_OBJ = $(patsubst %.c, $(GCOV_DIR)/%.o, $(GCOV_SRC))
REPORT_PATH = $(GCOV_DIR)/report/

all: $(LIB_PATH)

$(LIB_PATH): $(OBJ)
	mkdir -p $(BUILD_DIR)
	ar rcs $@ $^
	ranlib $@

$(OBJ_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

install: $(LIB_PATH)
	$(CC) $(LIB_PATH) $(CFLAGS) -lncurses -g -o tetris

uninstall:
	rm -rf ./tetris score.dt

clean: 
	@rm -rf $(BUILD_DIR) $(OBJ_DIR) dvi $(TEST_EXEC) $(GCOV_DIR) brick_game.tar.gz

dist:
	tar czf brick_game.tar.gz brick_game/ gui/ tests/ brick_game.h *.png documentation.tex Makefile

dvi:
	mkdir -p dvi
	xelatex -output-directory=dvi documentation.tex

run: install
	./tetris

mem: install
	valgrind --leak-check=full --suppressions=ncurses.supp ./tetris

$(TEST_EXEC): $(OBJ_TESTS)
	$(CC) $(CFLAGS) -o $@ $^ $(CHECK_LIBS) $(CHECK_FLAGS) 

test: clean $(TEST_EXEC)
	@echo "	\033[0;92mStart of all the tests...\033[0m"
	./$(TEST_EXEC)

$(GCOV_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -c $< -o $@

gcov_report: clean $(GCOV_OBJ)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(GCOV_DIR)/$(TEST_EXEC) $(GCOV_OBJ) tests/unit_tetris.c $(CHECK_LIBS) $(CHECK_FLAGS) -lncurses
	./$(GCOV_DIR)/$(TEST_EXEC)
	lcov -t "$(GCOV_DIR)/$(TEST_EXEC)" -o $(GCOV_DIR)/coverage.info -c -d .
	lcov --remove $(GCOV_DIR)/coverage.info "*/tests/*" "*/gui/*" -o $(GCOV_DIR)/coverage.info --ignore-errors unused
	genhtml -o $(REPORT_PATH) $(GCOV_DIR)/coverage.info
	@echo "<table width="100%"><td class=\"title\" style=\"color: lightblue; -webkit-text-stroke: 1px black;\">#Project was made by woodsjul</td></table>" >> $(REPORT_PATH)index.html
	@echo "=============================================="
	@echo "Отчёт покрытия успешно сгенерирован!"
	@echo "Откройте в браузере:"
	@echo "file://$(CURDIR)/$(REPORT_PATH)index.html"
	@echo "=============================================="
	$(OPEN_CMD) ./$(REPORT_PATH)index.html

cpptest:
	cppcheck --enable=all --suppress=missingIncludeSystem *.h brick_game/tetris/*.h gui/cli/*.h tests/*.h
	cppcheck --enable=all --suppress=missingIncludeSystem brick_game/tetris/*.c gui/cli/*.c tests/*.c

clang_check:
	cp ../materials/linters/.clang-format .
	clang-format -n *.h brick_game/tetris/*.h gui/cli/*.h tests/*.h
	clang-format -n *.h brick_game/tetris/*.c gui/cli/*.c tests/*.c

clang_fix:
	clang-format -i *.h brick_game/tetris/*.h gui/cli/*.h tests/*.h
	clang-format -i *.h brick_game/tetris/*.c gui/cli/*.c tests/*.c

leaks: $(TEST_EXEC)
	leaks -atExit -- ./$(TEST_EXEC) 

valgrind: $(TEST_EXEC)
	CK_FORK=no valgrind --suppressions=ncurses.supp --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(TEST_EXEC)

.PHONY: all clean test gcov_report run install uninstall dvi dist

# install xelatex: sudo apt install texlive-xetex texlive-lang-cyrillic fonts-dejavu