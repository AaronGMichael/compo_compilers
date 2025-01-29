# Parser Branch

This branch consists of the code to make our scanner and parser. We will now be able to see what structure our document has after reading the HTML file. Currently, the parser only supports Bold and H1 tags. You will have to add compatibility for the other tags you want your compiler to support.

To compile the parser file, use the command:

```bash
make re
```

and to run the scanner on a particular file, we can run it using:

```bash
out/compo tests/basic.md 
```

You should be able to see the structure of the document on the Terminal on running this command.

## Abstract Syntax Tree (AST)
There is a very basic implementation of an Abstract Syntax tree, that just supports text. You should improve the funcationality to include some parameters (E.g, Href for links, or co-ordinates for SVGs).

The parser builds the AST from the recieved tokens and generates the structure of the document.

You can test whether your AST implementation works by completing the ast_tests.c with the parameters you want to support. Note the functions used to add elements to a particular DOM node.

The tests can be compiled with:

```bash
make ast_tests
```

and run with 
```bash
out/ast_tests
```

Please ensure you have installed `gcc`, `flex` and `bison` before running this command.
