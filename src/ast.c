#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"


// void yyerror(const char* msg) {
//     fprintf(stderr, "Parser error: %s\n", msg);
// }


/**
 * @brief Creates a new DOM node with the specified HTML element and children.
 *
 * This function allocates memory for a new DOM node, sets its HTML element type,
 * and assigns its children. The text and attributes of the DOM node are initialized to NULL.
 *
 * @param dom_el The HTML element type for the new DOM node.
 * @param children A pointer to the children of the new DOM node.
 * @return A pointer to the newly created DOM node.
 */
DOM* create_dom(HTMLElement dom_el, DOMChildren* children) {
    DOM* dom = malloc(sizeof(DOM));
    dom->html_elem = dom_el;
    dom->children = children;
    dom->text = NULL;
    return dom;
}

/**
 * @brief Creates a new DOMChildren node with the specified DOM node and NULL next pointer.
 *
 * This function allocates memory for a new DOMChildren node, sets its DOM node to the
 * specified value, and sets its next pointer to NULL.
 *
 * @param dom The DOM node to be stored in the new DOMChildren node.
 * @return A pointer to the newly created DOMChildren node.
 */
DOMChildren* new_dom_children(DOM* dom) {
    DOMChildren* dom_children = malloc(sizeof(DOMChildren));
    dom_children->dom = dom;
    dom_children->next = NULL;
    return dom_children;
}

/**
 * @brief Adds a child node to the specified parent element.
 *
 * This function appends the specified child node to the end of the parent element's children list.
 *
 * @param dom_element The parent element to which to add the child node.
 * @param dom_child The child node to be added.
 * @return The parent element with the child appended, or the parent element if either argument is NULL.
 */
DOM* add_child_to_element(DOM *dom_element, DOM *dom_child) {
    if (!dom_element || !dom_child) return dom_element;

    // Create a new DOMChildren node for the child
    DOMChildren *new_child = new_dom_children(dom_child);

    if (!dom_element->children) {
        // If the parent element has no children, set the new child as the first child
        dom_element->children = new_child;
    } else {
        // Traverse to the end of the children list
        DOMChildren *current = dom_element->children;
        while (current->next) {
            current = current->next;
        }
        // Append the new child to the end of the list
        current->next = new_child;
    }
    return dom_element;
}
/* 
    Memory Management required with C
*/

/**
 * @brief Frees the memory associated with a DOMChildren list.
 *
 * This function takes the first node of a DOMChildren list and iterates through
 * the list, freeing the memory associated with each node's DOM element
 * (if present) and the node itself.
 *
 * @param children The first node of the DOMChildren list to be freed.
 */
void free_dom_children(DOMChildren *children) {
    while (children) {
        DOMChildren *next = children->next;
        if (children->dom) {
            destroy_dom(children->dom);
        }
        free(children);
        children = next;
    }
}

/**
 * @brief Frees the memory associated with a DOM node and its children.
 *
 * This function takes a DOM node and iterates through its children, freeing the
 * memory associated with each of them. It then frees the memory associated with
 * the original node.
 *
 * @param dom The DOM node to be destroyed.
 */
void destroy_dom(DOM *dom) {
    if (!dom) return;
    free_dom_children(dom->children);

    if (dom->text) free(dom->text);

    free(dom);
}

