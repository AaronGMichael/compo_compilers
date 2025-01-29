#include "ast.h"

#define SIMPLE_STRINGS_IMPL
#include "simple_strings.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdarg.h>

const char HTML_HEADER[] = "<!DOCTYPE html>\n"
						   "<html>\n"
						   "  <head>\n"
						   "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
						   "    <meta charset=\"utf-8\">\n"
						   "    <title>Rendered HTML</title>\n"
						   "    <link rel=\"stylesheet\" href=\"github-markdown.css\">\n"
						   "  </head>\n"
						   "  <body>\n";

const char HTML_FOOTER[] ="	   </body>\n"
						   "</html>";

string add_indentation(string str, unsigned int indent_lvl)
{
	for (int t = 0; t < indent_lvl; t++)
	{
		APPEND_ARR(str, "  ");
	}
	return str;
}
#define ADD_INDENTATION(str, indent_lvl) str = add_indentation(str, indent_lvl)

/**
 * @brief Append an integer to a string buffer.
 */
string append_int(string buffer, int value)
{
	int len = value != 0 ? log10(abs(value)) + 1 : 1;
	char *buf = malloc(len + 1);
	sprintf(buf, "%d", value);
	APPEND_ARR(buffer, buf);
	return buffer;
}
#define APPEND_INT(buffer, value) buffer = append_int(buffer, value)

string append_format(string buffer, const char *format, ...) {
    // Temporary buffer to store the formatted string
    char temp[256]; 

    // Initialize the variable argument list
    va_list args;
    va_start(args, format);

    // Format the string into the temporary buffer
    vsnprintf(temp, sizeof(temp), format, args);

    // End the variable argument list
    va_end(args);

    // Append the formatted string to the buffer
    APPEND_ARR(buffer, temp);

    return buffer;
}
#define APPEND_FORMAT(buffer, ...) buffer = append_format(buffer, __VA_ARGS__)

/**
 * @brief Append a coordinate to a buffer separated by a delimiter.
 */
// string append_coord(string buffer, SvgCoord *coord, const char *delim)
// {
// 	APPEND_INT(buffer, coord->x);
// 	APPEND_ARR(buffer, delim);
// 	APPEND_INT(buffer, coord->y);
// }
// #define APPEND_COORD(buffer, coord, delim) buffer = append_coord(buffer, coord, delim)

/**
 * @brief Append a list of coordinates to a buffer separated by a delimiter.
 */
// string append_coord_list(string buffer, SvgCoordList *coord_list, const char *delim, const char *coord_delim)
// {
// 	while (coord_list != NULL)
// 	{
// 		APPEND_COORD(buffer, coord_list->coord, coord_delim);

// 		coord_list = coord_list->next;
// 		if (coord_list != NULL)
// 			APPEND_ARR(buffer, delim);
// 	}
// 	return buffer;
// }
// #define APPEND_COORD_LIST(buffer, coord_list, delim, coord_delim) buffer = append_coord_list(buffer, coord_list, delim, coord_delim)

/**
 * @brief Append a property in the form ` prop="value"` to a buffer.
 */
string append_prop(string buffer, const char *prop, char *value)
{
	APPEND_ARR(buffer, " ");
	APPEND_ARR(buffer, prop);
	APPEND_ARR(buffer, "=\"");
	APPEND_ARR(buffer, value);
	APPEND_ARR(buffer, "\"");
	return buffer;
}
#define APPEND_PROP(buffer, prop, value) buffer = append_prop(buffer, prop, value)

/**
 * @brief Append a property in the form ` prop="value"` to a buffer.
 */
string append_int_prop(string buffer, const char *prop, int value)
{
	APPEND_ARR(buffer, " ");
	APPEND_ARR(buffer, prop);
	APPEND_ARR(buffer, "=\"");
	APPEND_INT(buffer, value);
	APPEND_ARR(buffer, "\"");
	return buffer;
}
#define APPEND_INT_PROP(buffer, prop, value) buffer = append_int_prop(buffer, prop, value);


string code_generation_from_dom(DOM *dom, unsigned int indent)
{

	if (dom == NULL)
		return STR("");

	string attributes = STR("");
	switch (dom->html_elem)
	{
	case Document:
	{
		string html = STR("");
		ADD_INDENTATION(html, indent);

		DOMChildren *child = dom->children;

		while (child != NULL)
		{
			string child_html = code_generation_from_dom(child->dom, indent + 1);

			APPEND_STR(html, child_html);

			child = child->next;
		}

		return html;
	}
	case Header1:
	{
		string html = STR("");
		ADD_INDENTATION(html, indent);
		APPEND_FORMAT(html, "<h1%s>", attributes);
		APPEND_ARR(html, dom->text);
		APPEND_ARR(html, "</h1>\n");
		return html;
	}
	case Paragraph:
	{
		string html = STR("");
		ADD_INDENTATION(html, indent);

		APPEND_FORMAT(html, "<p%s>\n", attributes);

		DOMChildren *child = dom->children;
		DOM *previous_child = NULL;

		while (child != NULL)
		{
			// We check here if we have two TextElement besides, it should have a space between.
			if (previous_child != NULL)
			{
				if (previous_child->html_elem == TextElement)
				{
					APPEND_ARR(html, " ");
				}
			}
			else
			{
				ADD_INDENTATION(html, indent + 1);
			}

			string content = code_generation_from_dom(child->dom, indent + 1); // Indentation not relevant here

			APPEND_STR(html, content);

			previous_child = child->dom;
			child = child->next;
		}

		APPEND_ARR(html, "\n");
		ADD_INDENTATION(html, indent);
		APPEND_ARR(html, "</p>\n");

		return html;
	}
	case TextElement:
	{
		string html = STR(dom->text);

		return html;
	}
	case Bold:
	{
		string html = STR("");

		APPEND_FORMAT(html, "<b%s>", attributes);

		if (dom->children != NULL)
		{
			string content = code_generation_from_dom(dom->children->dom, indent + 1); // Identation not relevant here

			APPEND_STR(html, content);
		}

		APPEND_ARR(html, "</b>");

		return html;
	}
	default:
	{
		return STR("");
	}
	}
}

string code_generation(DOM *dom)
{
	string final_code = STR(HTML_HEADER);
	string dom_code = code_generation_from_dom(dom, 2);

	APPEND_STR(final_code, dom_code);
	APPEND_ARR(final_code, HTML_FOOTER);

	spls_free_string(dom_code);

	return final_code;
}