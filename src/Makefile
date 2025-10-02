#This project made by woodsjul
CC = gcc
CFLAGS = -Werror -Wall -Wextra -Wpedantic -std=c11
GCOV_FLAGS = -D TEST_MEMORY_FAILURE -fprofile-arcs -ftest-coverage
CHECK_FLAGS = -lcheck -lm -lpthread
UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
    CHECK_FLAGS += -lsubunit
	OPEN_CMD:=xdg-open
else ifeq ($(shell uname), Darwin)
	OPEN_CMD:=open
endif

BUILD_DIR = build
LIB_NAME = brick_game.a
LIB_PATH = $(BUILD_DIR)/$(LIB_NAME)

SRC_TETRIS = $(wildcard brick_game/tetris/*.c)
SRC_CLI = $(wildcard gui/cli/*.c)
SRC = $(SRC_TETRIS) $(SRC_CLI)

SRC_TESTS = $(wildcard tests/*.c)
UNIT_TEST_DIR = tests
TESTS_CHECK = $(wildcard $(UNIT_TEST_DIR)/*.c)
TEST_EXEC = tests_matrix

OBJ_DIR = obj
OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SRC))
OBJ_TESTS1 = $(patsubst %.c, $(OBJ_DIR)/%-test.o, $(SRC))
OBJ_TESTS2 = $(patsubst %.c, $(OBJ_DIR)/%-test.o, $(SRC_TESTS))
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
	@rm -rf $(BUILD_DIR) $(OBJ_DIR) dvi

dist:
	tar czf brick_game.tar.gz brick_game/ gui/ brick_game.h *.uml Makefile

dvi:
	mkdir -p dvi
	xelatex -output-directory=dvi documentation.tex

run: install
	./tetris









$(TEST_EXEC): $(OBJ_TESTS1) $(OBJ_TESTS2)
	@$(CC) $(CFLAGS) -D TEST_MEMORY_FAILURE -o $@ $^ $(CHECK_FLAGS)


$(OBJ_DIR)/%-test.o: %.c
	@mkdir -p $(@D)
	@echo "*\c";
	@$(CC) $(CFLAGS) -D TEST_MEMORY_FAILURE -c $< -o $@

buildtest: all $(TEST_EXEC)

test: buildtest
	@echo "*";
	@echo "	\033[0;92mStart of all the tests...\033[0m"
	@./$(TEST_EXEC)

gcov_report: clean
	mkdir -p $(GCOV_DIR)
	$(CC) $(CFLAGS) $(GCOV_FLAGS) -o $(GCOV_DIR)/$(TEST_EXEC) $(SRC) $(TESTS_CHECK) $(CHECK_FLAGS)
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
	$(OPEN_CMD) ./$(REPORT_PATH)index.html



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