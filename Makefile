.POSIX:

include config.mk

# search src/ and libs/ for c files
# ignore any path that contains '/test_' or 'src/main.c"
SRC = $(shell find src libs -name '*.c' | grep -v src/main.c | grep -v "/test_")
OBJ = ${SRC:.c=.o}

# tests live in t/ and libs
# tests must begin with test_
# we only want to automatically build and run t/unit and any tests in libs
# t/custom is managed manually by test_custom
TESTS = $(shell find t/unit libs -name 'test_*.c' )
# output location for tests
TESTOUT = bin
TESTO = $(patsubst %.c, $(TESTOUT)/%, $(TESTS))

# only defined when `make debug`
EXTRAFLAGS =

# default to compiling without debug symbols
all: icarus

# compile each component, linking is handled by `icarus` rule
%.o: %.c
	@echo COMPILING CC $< with extra flags \"${EXTRAFLAGS}\"
	@${CC} -g -c ${CFLAGS} $< ${EXTRAFLAGS} -o $@

# compile and link main executable
icarus: ${OBJ}
	@echo more compiling CC -o $@ with extra flags \"${EXTRAFLAGS}\"
	@${CC} src/main.c -o $@ ${LDFLAGS} ${EXTRAFLAGS} ${OBJ}
	@make -s cleanobj

# build icarus with debug output
debug:
	@make -s EXTRAFLAGS="${DEBUGFLAGS}" icarus

# only clean up temporary files (.o)
cleanobj:
	@echo cleaning objects and temporary files
	@find . -iname '*.o' -delete

# clean up everything managed by make
clean: cleanobj
	@echo cleaning executables
	@rm -f icarus
	@echo cleaning tests
	@rm -rf $(TESTOUT)
	@echo removing gcov files
	@find . -iname '*.gcda' -delete
	@find . -iname '*.gcov' -delete
	@find . -iname '*.gcno' -delete
	@rm -f out.c

# run icarus over our simple example
example: icarus
	./icarus example/simple.ic

# clean
# build all OBJ
# run all TESTO
# cleanobj
#   this ensures that we do a fresh build every time
test: clean $(OBJ) $(TESTO) test_custom test_success cleanobj


test_custom: $(OBJ)
	@echo -e "\n\ncompiling t/custom/test_token_print.c to bin/t/custom/test_token_print"
	@mkdir -p `dirname bin/t/custom/test_token_print`
	@${CC} t/custom/test_token_print.c -o bin/t/custom/test_token_print ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_token_print.pl
	t/custom/test_token_print.pl

	@echo -e "\n\ncompiling t/custom/test_lex_simple.c to bin/t/custom/test_lex_simple"
	@mkdir -p `dirname bin/t/custom/test_lex_simple`
	@${CC} t/custom/test_lex_simple.c -o bin/t/custom/test_lex_simple ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_lex_simple.pl
	t/custom/test_lex_simple.pl

	@echo -e "\n\ncompiling t/custom/test_parse_simple.c to bin/t/custom/test_parse_simple"
	@mkdir -p `dirname bin/t/custom/test_parse_simple`
	@${CC} t/custom/test_parse_simple.c -o bin/t/custom/test_parse_simple ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_parse_simple.pl
	t/custom/test_parse_simple.pl

	@echo -e "\n\ncompiling t/custom/test_analyse_current.c to bin/t/custom/test_analyse_current"
	@mkdir -p `dirname bin/t/custom/test_analyse_current`
	@${CC} t/custom/test_analyse_current.c -o bin/t/custom/test_analyse_current ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_analyse_current.pl
	t/custom/test_analyse_current.pl

	@echo -e "\n\ncompiling t/custom/test_analyse_invalid.c to bin/t/custom/test_analyse_invalid"
	@mkdir -p `dirname bin/t/custom/test_analyse_invalid`
	@${CC} t/custom/test_analyse_invalid.c -o bin/t/custom/test_analyse_invalid ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_analyse_invalid.pl
	t/custom/test_analyse_invalid.pl

	@echo -e "\n\ncompiling t/custom/test_transform_simple.c to bin/t/custom/test_transform_simple"
	@mkdir -p `dirname bin/t/custom/test_transform_simple`
	@${CC} t/custom/test_transform_simple.c -o bin/t/custom/test_transform_simple ${CFLAGS} ${LDFLAGS} ${OBJ}
	@echo running test_transform_simple.pl
	t/custom/test_transform_simple.pl

# compile and run each test
$(TESTO) : $(TESTOUT)/% : %.c
	@echo -e "\n\ncompiling $< to $@"
	@mkdir -p `dirname $@`
	@${CC} $< -o $@ ${LDFLAGS} ${OBJ}
	@echo running test
	$@

# signal test success
test_success:
	@echo -e "\n\ntesting success\n"

readme:
	@echo -e "\n\ngenerating README.md"
	perl scripts/update_readme.pl

compile: clean icarus
	rm -f out.c
	./icarus example/simple.ic out.c

format:
	find src/ t/ -iname '*.[ch]' | xargs clang-format -style=file -i

man:
	man -l icarus.1

.PHONY: all clean cleanobj icarus test test_custom example readme compile format man

