CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11 -g
LFLAGS_COVERAGE=-lgcov --coverage
OS := $(shell uname)
CHECK_FLAGS = $(shell pkg-config --cflags --libs check)

all: test install open_app

rebuild: uninstall install

clean:
	rm -rf *.o *.a *.out *.info *.log *.gc*
	rm -rf test test.c 3dviewer.tar.gz
	rm -rf out test.dSYM
	rm -rf settings.ini

install: clean
	-mkdir build
	cd build; qmake ../3dviewer/3dviewer.pro && make && rm -f Makefile

uninstall:
	rm -rf build*

dist: clean
	tar -czf 3dviewer.tar.gz ./*

dvi:
	open dvi.html

dvi_chrome:
	open -a "Google Chrome" dvi.html

test: clean test.o
	$(CC) $(CFLAGS) parser.c affine.c test.o -o test $(CHECK_FLAGS)
	./test

test.o: test.c
	$(CC) $(CFLAGS) -c test.c -o test.o

test.c:
	checkmk clean_mode=1 *.check > test.c
	clang-format -i -style=google test.c

gcov_report: test.c
	$(CC) $(LFLAGS_COVERAGE) parser.c affine.c test.c -o test_cov.out $(CHECK_FLAGS)
	./test_cov.out
	lcov --no-external --capture --directory . --output-file coverage.info
	genhtml coverage.info --output-directory out
	open out/index.html

open:
ifeq ($(OS),Darwin)
	open -a "Google Chrome" report/src/index.html
else
	xdg-open report/src/index.html
endif

open_app:
ifeq ($(OS),Darwin)
	./build/3dviewer.app/Contents/MacOS/3dviewer
else
	./build/3dviewer
endif

clang:
	clang-format -n -style=google *.c *.h ./3dviewer/mainwindow.cpp ./3dviewer/mainwindow.h
	clang-format -n -style=google *.c *.h ./3dviewer/glmodel.cpp ./3dviewer/glmodel.h

clangI:
	clang-format -i -style=google *.c *.h ./3dviewer/mainwindow.cpp ./3dviewer/mainwindow.h
	clang-format -i -style=google ./3dviewer/glmodel.cpp ./3dviewer/glmodel.h ./3dviewer/main.cpp

cppcheck:
	cppcheck --enable=all *.c *.h

clean_images:
	rm -rf *.jpeg *.bmp *.gif

leak: test
ifeq ($(OS),Darwin)
	CK_FORK=no leaks -atExit -- ./test
else
	CK_FORK=no valgrind --leak-check=full -s --track-origins=yes --log-file=leak.log ./test
	cat leak.log
endif

.PHONY: all rebuild clean install uninstall dist dvi dvi_chrome leak clang clangI cppcheck open open_app