%{
/*********** C CODE (YOU DO NOT HAVE TO MODIFY IT) ******************/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"
#include "y.tab.h"

/* BEGIN MODE SYSTEM DECLARATION */
int mode_caller[8];
int mode_caller_len = 0;

void mode_caller_push(int mode);
int mode_caller_peek(void);
int mode_caller_pop(void);

#define BEGIN_MODE(mode) mode_caller_push(YY_START); BEGIN((mode));
#define END_MODE BEGIN(mode_caller_pop());
/* END MODE SYSTEM DECLARATION */

/* BEGIN STRING MANIPULATION DECLARATION */
#define STRBUFCAP 1024

char* strbuf = NULL;
size_t strbuflen = 0;

void strbufapp(char c);
char* strbufclr(void);
char* str_copy(char *str);
/* END STRING MANIPULATION DECLARATION */

void print_token(char *token);
void print_token_with_str(char *token, char *str);
void print_token_with_number(char *token, int number);

/*********** END OF C CODE ******************/

/*
Here is my personal list of tokens for my grammar.
You are welcome to use it if your grammar does not match what was expected:

NEWLINE
BLANK_LINE
BOLD
ITALIC
UNDERLINE
STRIKETHROUGH
H1
H2
H3
H4
H5
H6
QUOTE
EXCLAM
LPAREN
RPAREN
LBRACKET
RBRACKET
HR
INLINE_CODE
BLOCK_CODE
XSVG_BEGIN
XSVG_END
COMMA
LINE
POLYLINE
POLYGON
CIRCLE
ELLIPSE
RECT
XSVG_TEXT
TEXT
STR
NUMBER
*/

%}

newline "\n"
bold "**"
h1 "#"[[:blank:]]*

%x BOLD_MODE TEXT_MODE

%%

<INITIAL>[[:blank:]]*{newline} {
    print_token("NEWLINE");
    return NEWLINE;
};

([[:blank:]]*{newline}){2} {
    print_token("BLANK_LINE");
    return BLANK_LINE;
};

^{h1} {
    print_token("H1");
    return H1;
}

<INITIAL>{bold}	{
    BEGIN_MODE(BOLD_MODE);

    print_token("BOLD");
    return BOLD;
};

<BOLD_MODE>{bold} {
    END_MODE;

    print_token("BOLD");
    return BOLD;
};

<INITIAL,BOLD_MODE>. {
    BEGIN_MODE(TEXT_MODE);
    strbufapp(*yytext);
};

<TEXT_MODE>({newline}|{bold}|^{h1})(.|\n)* {
    END_MODE;

    yy_scan_string(yytext);

    char* txt = strbufclr();
    char *copy = str_copy(txt);
    print_token_with_str("TEXT", copy);
    yylval.text = copy;
    return TEXT;
};

<TEXT_MODE><<EOF>> {
    END_MODE;

    char* txt = strbufclr();
    char *copy = str_copy(txt);
    print_token_with_str("TEXT", copy);
    yylval.text = copy;
    return TEXT;
};

<TEXT_MODE>. {
    strbufapp(*yytext);
};

%%
/*********** C CODE (YOU DO NOT HAVE TO MODIFY IT) ******************/
void mode_caller_push(int mode) {
    mode_caller[mode_caller_len++] = mode;
}

int mode_caller_peek(void) {
    return mode_caller_len == 0 ? INITIAL : mode_caller[mode_caller_len - 1];
}

int mode_caller_pop(void) {
    int caller = mode_caller_peek();
    if (mode_caller_len > 0) {
        mode_caller_len--;
    }
    return caller;
}

void strbufapp(char c) {
    if (strbuf == NULL) {
        strbuf = malloc(sizeof(char) * STRBUFCAP);
    }
    if (strbuflen >= STRBUFCAP - 2) {
        char* new_strbuf = malloc(sizeof(char) * 2 * STRBUFCAP);
        strncpy(new_strbuf, strbuf, strbuflen);
        free(strbuf);
        strbuf = new_strbuf;
    }
    strbuf[strbuflen++] = c;
}

char* strbufclr(void) {
    strbuf[strbuflen] = '\0';
    strbuflen = 0;
    return strbuf;
}

char* str_copy(char *str) {
    char* copy = malloc(strlen(str) + 1);

    strcpy(copy, str);

    return copy;
}

void print_token(char *token) {
    printf("TOKEN: %s\n", token);
}

void print_token_with_str(char *token, char *str) {
    printf("TOKEN: %s | String: \"%s\"\n", token, str);
}

void print_token_with_number(char *token, int number) {
    printf("TOKEN: %s | Number: \"%d\"\n", token, number);
}
