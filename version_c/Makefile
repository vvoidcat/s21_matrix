UNAME = $(shell uname -s)
FLAGSS = "noflags"
CC = gcc -g -Wall -Werror -Wextra -std=c11
SOURCES_LIB = $(TMPDIR)/matrix_lib.o $(TMPDIR)/matrix_helper_lib.o src/s21_matrix.h
SOURCES_TEST = $(TMPDIR)/matrix_tests.o $(TMPDIR)/matrix_helper_tests.o src/s21_matrix.h
TMPDIR = tmp
COVDIR = unit_tests/coverage
OUTNAME = "tests"

ifeq ($(UNAME),Darwin)
	FLAGSS = -lcheck -lm -lpthread -fprofile-arcs
endif
ifeq ($(UNAME),Linux)
	FLAGSS = -lcheck -lsubunit -lrt -lm -lpthread -fprofile-arcs
endif


all: cleanall build clean

start:
	if ! [ -d "$(TMPDIR)" ]; then mkdir $(TMPDIR); fi

build: s21_matrix.a test

rebuild: all

gcov_report:
	rm -rf $(COVDIR) && mkdir $(COVDIR)
	@echo ""
	gcov -b -l -p -c $(TMPDIR)/*.gcno
	gcovr -g -k -r . --html --html-details -o $(COVDIR)/coverage_report.html
	mv *.gcov $(TMPDIR)
	@echo "-----------------------------"

test: start unit_tests/s21_matrix_tests.c $(SOURCES_TEST)
	rm -rf $(TMPDIR)/s21_matrix_tests*
	$(CC) unit_tests/s21_matrix_tests.c $(SOURCES_TEST) $(FLAGSS) -o $(OUTNAME)
	@echo "-----------------------------"
	./$(OUTNAME)
	@echo "-----------------------------"
	mv *gcda $(TMPDIR)


s21_matrix.a: start $(SOURCES_LIB)
	ar -rcs s21_matrix.a $(SOURCES_LIB)
	ranlib s21_matrix.a

style:
	cp materials/".clang-format" ".clang-format"
	clang-format -i src/*.c
	clang-format -i src/*.h
	clang-format -n src/*.c
	clang-format -n src/*.h
	rm -rf ".clang-format"
	@echo "-----------------------------"

valgrind:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
			 ./$(OUTNAME)

clean:
	rm -rf $(TMPDIR) *.o *.gcno *.gcda *.html *.gcov *.dSYM a.out ".clang-format"

cleanall: clean
	rm -rf $(OUTNAME) $(COVDIR) *.a


#### object files

$(TMPDIR)/matrix_lib.o: start src/s21_matrix.c
	$(CC) -c src/s21_matrix.c -o $(TMPDIR)/matrix_lib.o

$(TMPDIR)/matrix_helper_lib.o: start src/s21_matrix_helper.c
	$(CC) -c src/s21_matrix_helper.c -o $(TMPDIR)/matrix_helper_lib.o

$(TMPDIR)/matrix_tests.o: start src/s21_matrix.c
	rm -rf $(TMPDIR)/matrix_tests*
	$(CC) -c --coverage src/s21_matrix.c -o $(TMPDIR)/matrix_tests.o

$(TMPDIR)/matrix_helper_tests.o: start src/s21_matrix_helper.c
	rm -rf $(TMPDIR)/matrix_helper_tests*
	$(CC) -c --coverage src/s21_matrix_helper.c -o $(TMPDIR)/matrix_helper_tests.o