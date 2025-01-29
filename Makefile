RM		= rm -f

ifdef VERBOSE
DEBUG ?= $(VERBOSE)
endif

OUT_DIR = out
SRC_DIR = src
TEST_DIR = tests

# Binary names
MAIN_PROG = $(OUT_DIR)/compo
TEST_PROG = $(OUT_DIR)/ast_tests

# Compiler and flags
CC = gcc
CFLAGS = -g -Wall -I$(SRC_DIR)
LDFLAGS = -lm

all: compo

scanner: just_scanner

compo: out
	bison $(if $(VERBOSE),-tv -Wcounterexamples) -o src/y.tab.c -d src/parser.y
	flex $(if $(VERBOSE),-d) -o src/lex.yy.c src/scanner.l
	gcc $(if $(DEBUG),) -g -o out/compo src/lex.yy.c src/y.tab.c src/ast.c src/code_generation.c -lm

just_scanner: out
	flex -o src/lex.yy.c src/scanner.l
	gcc src/lex.yy.c -lfl -o out/compo_scanner


out:
	mkdir -p out

.PHONY: clean re

clean:
	$(RM) out/compo out/result.html src/lex.yy.c src/y.tab.c src/y.tab.h

re: clean all

re-scanner: clean scanner
