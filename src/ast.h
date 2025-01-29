#ifndef __H_AST
#define __H_AST

/*
YOU DO NOT HAVE TO CHANGE THIS FILE.
YOU MAY IF YOU WANT TO ADD ANY IMPROVEMENT.
*/

typedef enum html_element
{
	Document,
	TextElement,
	Paragraph,
	Bold,
	Header1,
} HTMLElement;

// DOM Children Struct
typedef struct dom_children {
    struct dom *dom;
    struct dom_children *next;
} DOMChildren;

// DOM Node Struct
typedef struct dom {
    HTMLElement html_elem;
    struct dom_children *children;
    char *text;
} DOM;

DOM *create_dom(HTMLElement dom_el, DOMChildren *children);
DOMChildren *new_dom_children(DOM *dom);
DOM* add_child_to_element(DOM *dom_element, DOM *dom_child);
void free_dom_children(DOMChildren *children);
void destroy_dom(DOM *dom);

#endif
