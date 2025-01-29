#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


/**
 * @brief Returns the string representation of the given HTML element.
 *
 * This function takes an HTMLElement enumeration value as input and returns
 * the corresponding string name of the HTML element. If the enumeration value
 * does not match any known HTML element, it returns "Unknown".
 *
 * @param elem The HTMLElement enumeration value.
 * @return The string name of the HTML element.
 */
const char *get_element_name(HTMLElement elem) {
    switch (elem) {
        case Document: return "Body";
        case Header1: return "H1";
        case Paragraph: return "Paragraph";
        case Bold: return "Bold";
        default: return "Unknown";
    }
}


/**
 * @brief Recursively prints out the DOM structure with indentation.
 *
 * @param dom The root DOM node to start printing from
 * @param depth The current indentation level
 */
void print_dom(DOM *dom, int depth) {
    if (!dom) return;

    // Print indentation for the current depth
    for (int i = 0; i < depth; i++) printf("  ");
    printf("Element: %s\n", get_element_name(dom->html_elem));

    // Print text and URL if present
    if (dom->text) printf("%*sText: %s\n", depth * 2, "", dom->text);

    // Recursively print child nodes
    DOMChildren *child = dom->children;
    while (child) {
        print_dom(child->dom, depth + 1);
        child = child->next;
    }
}

/**
 * @brief A test program to build a sample DOM and print it out, then destroy it.
 *
 * This program is a test case for the abstract syntax tree (AST) library.
 * It creates a DOM structure with a hierarchy of nodes, prints it out,
 * and then frees all the memory.
 *
 * The DOM structure is meant to mimic the following HTML code:
 *
 * <body>
 *   <p>This is a paragraph.</p>
 *   <a href="www.google.com">This is a link to Google</a>
 *   <img src="imgs/hey.png" />
 *   <svg width="100" height="100" viewBox="0 0 100 100">
 *     <line x1="0" y1="0" x2="100" y2="100" stroke="blue" stroke-width="2" />
 *     <polyline points="10,10 20,30 30,40 40,50 50,60 60,70 70,80 80,90 90,100" stroke="green" stroke-width="2" fill="none" />
 *   </svg>
 * </body>
 * 
 * 
 * We will see the DOM printed out in the console. We also have the functions we need to clear the memory. 
 * Memory usage is tested with valgrind to ensure there are no leaks. 
 *
 * @return 0 on success
 */
int main() {
    // Create a root DOM node (e.g., Body)
    DOM *root = create_dom(Document, NULL);

    // Add the first paragraph as a child node
    DOM *paragraph1 = create_dom(Paragraph, NULL);
    add_child_to_element(root, paragraph1);
    // Print the entire DOM structure
    printf("DOM Structure:\n");
    print_dom(root, 0);

    // Free all allocated memory
    destroy_dom(root);

    printf("Memory has been freed successfully.\n");

    return 0;
}

