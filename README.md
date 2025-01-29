# Scanner Branch

This branch consists of just the code to make our scanner. Currently, the scanner only supports Bold and H1 tags. You will have to add compatibility for the other tags you want your compiler to support.

To compile the parser file, use the command:

```bash
make re-scanner
```

and to run the scanner on a particular file, we can run it using:

```bash
out/compo_scanner tests/basic.md 
```

Please ensure you have installed `gcc` and `flex` before running this command.
