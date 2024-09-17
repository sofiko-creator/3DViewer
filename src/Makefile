CFLAGS = -Wall -Werror -Wextra -std=c11 -g 
OS := $(shell uname -s)
MYFLAGS = -lcheck -lsubunit -pthread -lrt -lm

ifeq ($(OS),Linux)
	CFLAGS+=-D LINUX
else
	ifeq ($(OS),Darwin)
		CFLAGS+=-D MAC
	endif
endif

all: tests gcov_report 
	./Result/Qt_3Dviewer
	
dvi:
	open Doc.md

uninstall:
	rm -rf build

install: clean 
	mkdir build
	cd build && qmake ../Qt_3D_Viewer && make
	mkdir Result
	cp build/Qt_3D_Viewer Result/

dist: uninstall clean
	rm -rf dist-pacanianime-viewer.tar.gz    
	tar -czf dist-pacanianime-viewer.tar.gz *

tests: Core/Viewer.c Core/s21_test.c 
	mkdir -p test_files
	gcc $^ -o test_files/$@ $(CFLAGS) $(MYFLAGS) --coverage
	./test_files/tests

gcov_report: tests
	./test_files/tests
	gcovr --html-details test_files/gcov_result.html
	open test_files/gcov_result.html

check_format:
	cp ../materials/linters/.clang-format ../src/.clang-format
	clang-format -n --style=Google Core/*.c
	clang-format -n --style=Google Core/*.h
	clang-format -n --style=Google Qt_3D_Viewer/*.cpp
	clang-format -n --style=Google Qt_3D_Viewer/*.h

clean:
	rm -rf *.o *.gcda *.a *.html *.gcno *.css .clang-format build 
	rm -rf Qt_3D_Viewer/*.o
	rm -rf test_files
	rm -rf Result

rebuild: clean all