%{
/*********** C CODE (YOU DO NOT HAVE TO MODIFY IT) ******************/
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "ast.h"


#define CHECK_YYNOMEM(ptr) if ((ptr) == NULL) YYNOMEM;
#define CHECK_LENGTH(var, num) if ((num) >= 0) var = num; else { yyerror("negative length"); YYERROR; }

int yylex(void);
void yy_scan_string(char* s);
int yylex_destroy(void);
void yyerror(const char*);

extern char* strbuf;


// Define the tokens you will receive from the parser. This should match the tokens defined in ast.h
const char *tokens[] = {
    "Document",
    "TextElement",
    "Paragraph",
    "Bold",
    "Header1",
};

DOM* dom_root = NULL;

/*********** END OF C CODE ******************/
/** YOU WILL HAVE TO COMPLETE THE DOCUMENT BELOW **/
%}

%define parse.lac full
%define parse.error detailed

%union {
    char* text;
    int number;
    DOM* dom;
    DOMChildren* dom_children;
}

%destructor { free($$); } <text>
%destructor { destroy_dom($$); } <dom>
%destructor { free_dom_children($$); } <dom_children>


%token NEWLINE BLANK_LINE
%token BOLD
%token H1
%token <text> TEXT

%type <dom> document block
%type <dom_children> block_list paragraph line text

%precedence NEWLINE
%start document

%%

text:
    TEXT {
        DOM* dom = create_dom(TextElement, NULL);
        dom->text = $1;
        $$ = new_dom_children(dom);
    };
    | BOLD text BOLD {
        DOM* dom = create_dom(Bold, $2);
        $$ = new_dom_children(dom);
    };
line:
    text line {
        $$ = $1;
        $$->next = $2;
    }
    | text { $$ = $1; };
paragraph:
    line NEWLINE paragraph {
        $$ = $1;
        DOMChildren* curr = $$;
        while (curr->next != NULL) curr = curr->next;
        curr->next = $3;
    }
    | line {
        $$ = $1;
    };
block:
    H1 TEXT {
        $$ = create_dom(Header1, NULL);
        $$->text = $2;
    }
    | paragraph {
        $$ = create_dom(Paragraph, $1);
    }
block_list:
    block BLANK_LINE block_list {
        if ($1 == NULL) {
            $$ = $3;
        } else {
            $$ = new_dom_children($1);

            $$->next = $3;
        }
    }
	| block NEWLINE block_list {
		if ($1 == NULL) {
			$$ = $3;
		} else {
			$$ = new_dom_children($1);

			$$->next = $3;
		}
	}
    | block {
        $$ = new_dom_children($1);
    }
	| NEWLINE block_list {
		$$ = $2;
	}
	| BLANK_LINE block_list {
		$$ = $2;
	}
	

document: block_list {
    dom_root = $$ = create_dom(Document, $1);
    YYACCEPT;
};

%%

/*********** C CODE (YOU DO NOT HAVE TO MODIFY IT) ******************/

void dom_display(DOM* dom, int depth) {
    if (dom == NULL) return;
    int i = 0;

    if (depth == 1) {
        printf("%s", tokens[dom->html_elem]);
    } else {
        for (i = 0; i < depth - 2; i++) {
            printf("│   ");
        }
        printf("├─── %s", tokens[dom->html_elem]);
    }
    if (dom->text != NULL) {
        printf(" (%s)", dom->text);
    }

    printf("\n");

    DOMChildren* current_child = dom->children;

    while (current_child != NULL) {
        dom_display(current_child->dom, depth + 1);

        current_child = current_child->next;
    }
}

void yyerror(const char* s) {
    extern int yylineno;
    extern char* yytext;
    fprintf(stderr, "Error at line %d near '%s': %s\n", yylineno, yytext, s);
}

int yywrap(void) {
    return 1;
}

int main(int argc, char* argv[]) {
#if YYDEBUG
    extern int yydebug;
    yydebug = 1;
#endif

    char* input = NULL;

    /* // Ensure proper usage with both input and output file arguments
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return -1;
    } */

    // Open the input file
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
        return -1;
    }

    // Read the input file into memory
    fseek(f, 0, SEEK_END);
    int length = ftell(f);
    rewind(f);
    input = malloc(sizeof(char) * (length + 1));
    if (input == NULL) {
        fprintf(stderr, "Cannot allocate enough memory for input file\n");
        fclose(f);
        return -2;
    }
    fread(input, sizeof(char), length, f);
    fclose(f);
    *(input + length) = 0;

    // Trim leading and trailing whitespace
    unsigned int i = 0;
    while (i < length && isspace(*input)) {
        input++;
        i++;
    }
    unsigned int j = length - i - 1;
    while (j > i && isspace(*(input + j))) *(input + j--) = 0;

    yy_scan_string(input);

    // Parse the input
    int ret = yyparse();

    // Cleanup input buffer
    if (input != NULL) {
        free(input);
    }

    if (argc == 3) {
        yylex_destroy();
    }

    if (strbuf != NULL) {
        free(strbuf);
    }

    if (ret > 0) {
        return ret;
    } else {
        printf("Starting to Print\n");
        dom_display(dom_root, 1);

        // Generate code from the parsed DOM
        /* string code = code_generation(dom_root); */

        // Open the output file
        /* FILE* fres = fopen(argv[2], "w");
        if (!fres) {
            fprintf(stderr, "Cannot open output file, writing to out/results: %s\n", argv[2]);
            fres = fopen("out/results.html", "w");
        }

        // Write the generated code to the output file
        fprintf(fres, "%s", code);
        fclose(fres); */

        destroy_dom(dom_root);
    }

    return 0;
}


/*********** END OF C CODE ******************/
