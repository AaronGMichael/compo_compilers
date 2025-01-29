# Compo Project

This repository consists of a framework of code to generate your own Markdown to HTML compiler.

There are 3 branches in this repository:
1. main : Consists of all code to be able to parse a markdown file and write to HTML. It consists of the scanner, parser, asts and code_generation
2. scanner_only: This branch consists of the code for just the scanner. This will read the provided file and emit the tokens and the texts into the console.
3. scanner_parser: This branch consists of the code for the scanner, parser and the ast. The interconnections between them have also been made. This branch will read a md file and print out the AST into the console to understanc the document structure.

Please switch to the required branch according to the stage of your project to get the required code. you can switch branches after cloning the repository by using

```bash
git switch branch_name
```

Please remember to `git fetch` if you get the error that branch name does not exist.

## Code generation

In this branch, we have a basic code generator. We call the code generation function on the generated AST and we should get the html written.

To compiler this section of the code:

```bash
make re
```

and to run it
```bash
out/compo tests/basic.md out/{filename}.html
```

replace `{filename}` with your desired filename.

## Dependencies

Since this project is based on C, we must be running this on a UNIX based system. Windows users will have to use either a linux based VM or use [WSL](https://learn.microsoft.com/en-us/windows/wsl/install).

After getting a UNIX machine, please install `gcc`, `bison`, `flex`.
