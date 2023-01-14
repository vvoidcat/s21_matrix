UNAME = $(shell uname -s)
PROJECTNAME = s21_matrix_cpp
FLAGSS = "noflags"
CC = g++ -Wall -Werror -Wextra -std=c++17 -g
TMPDIR = tmp
TESTDIR = unit-tests
COVDIR = unit-tests/coverage
BUILDDIR = build
BUILDDIR_LIB = build/$(PROJECTNAME)-lib
BUILDDIR_RELEASE = build/$(PROJECTNAME)-build-release
BUILDDIR_TESTS = build/$(PROJECTNAME)-tests
SOURCES_CPP = src/s21_matrix_oop.cc src/s21_matrix_oop.h
SOURCES_COMPILED = $(TMPDIR)/s21_fortests_matrix_oop.o
SOURCES_TESTS = $(TESTDIR)/s21_matrix_tests.cc
OUTNAME = $(PROJECTNAME)
OUTNAME_TESTS = $(PROJECTNAME)_test.out

ifeq ($(UNAME),Darwin)
	FLAGSS = -lgtest -lgtest_main -lm -lpthread -fprofile-arcs
endif
ifeq ($(UNAME),Linux)
	FLAGSS = -lgtest -lgtest_main -lsubunit -lrt -lm -lpthread -fprofile-arcs
endif

all: rebuild
.PHONY : all

start:
	if ! [ -d "$(TMPDIR)" ]; then mkdir $(TMPDIR); fi
	if ! [ -d "$(COVDIR)" ]; then mkdir $(COVDIR); fi
	if ! [ -d "$(BUILDDIR)" ]; then mkdir $(BUILDDIR); fi
	if ! [ -d "$(BUILDDIR_TESTS)" ]; then mkdir $(BUILDDIR_TESTS); fi
	if [ ! -d "$(BUILDDIR_LIB)" ]; then mkdir $(BUILDDIR_LIB); fi
.PHONY : start

build: s21_matrix_oop.a
.PHONY : build

rebuild: cleanall s21_matrix_oop.a
.PHONY : rebuild

s21_matrix_oop.a: start $(SOURCES_CPP)
	ar -rcs $(BUILDDIR_LIB)/s21_matrix_oop.a $(SOURCES_CPP)
	ranlib $(BUILDDIR_LIB)/s21_matrix_oop.a
.PHONY : s21_matrix_oop.a


launch: $(BUILDDIR_RELEASE)/$(OUTNAME)
ifeq ($(UNAME),Darwin)
	open $(BUILDDIR_RELEASE)/$(OUTNAME)
endif
ifeq ($(UNAME),Linux)
	./$(BUILDDIR_RELEASE)/$(OUTNAME)
endif
.PHONY : launch

test: clean start $(SOURCES_TESTS) $(SOURCES_CPP)
	rm -rf $(TMPDIR)/s21_fortests* $(TMPDIR)/*.gcda $(TMPDIR)/*.gcno
	rm -rf $(COVDIR)/*.css $(COVDIR)/*.html
	$(CC) -c --coverage src/s21_matrix_oop.cc -o $(TMPDIR)/s21_fortests_matrix_oop.o
	g++ $(SOURCES_TESTS) $(SOURCES_COMPILED) $(FLAGSS) -o $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	@echo "-------------------------------------------------"
	./$(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	@echo "-------------------------------------------------"
	mv ./$(BUILDDIR_TESTS)/*gcda $(TMPDIR)
.PHONY : test

gcov_report: $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	rm -rf $(COVDIR)/*.css $(COVDIR)/*.html
	@echo ""
	gcov -b -l -p -c $(TMPDIR)/*.gcno
	gcovr -g -k -r . --html --html-details -o $(COVDIR)/coverage_report.html
	mv *.gcov $(TMPDIR)
	open $(COVDIR)/coverage_report.html
.PHONY : gcov_report

style: $(SOURCES_CPP)
	cp materials/".clang-format" ".clang-format"
	clang-format -i src/*.cc
	clang-format -i src/*.h
	clang-format -n src/*.cc
	clang-format -n src/*.h
	rm -rf ".clang-format"
.PHONY : style

valgrind: $(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BUILDDIR_TESTS)/$(OUTNAME_TESTS)
.PHONY : valgrind

clean:
	rm -rf *.o *.gcno *.gcda *.gcov *.dSYM a.out test.out app src/app .clang-format
	rm -rf $(TMPDIR)
.PHONY : clean

cleanall: clean
	rm -rf $(BUILDDIR)
	rm -rf $(COVDIR)
.PHONY : cleanall
