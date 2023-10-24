# File-Expansion
Author: Rimsha Rizvi

## Description:
The program performs "file expansion" by taking an input file and producing its "expanded version." This process involves reading the contents of the file and expanding any #include directives found within it.

## Input File Format:
The program reads an input file containing text and #include directives in the form of #include "filename". The #include directive indicates that the contents of "filename" should be expanded at that location.

## Output:
The program outputs the expanded version of the input file, with the contents of any included files inserted in place of the #include directives.

## Error Handling:
The program handles the following errors:
1. If an #include directive does not specify a valid file, an error message is printed and the expansion terminates.
2. If the specified file in an #include directive does not exist, an error message is printed and the expansion terminates.
3. If infinite recursion is detected (i.e., a file includes itself), an error message is printed and the expansion terminates.

## Copyright and Plagiarism Notice
All content in this repository belongs to the author. If you would like to use this work for educational or other non-commercial purposes, don't hesitate to get in touch with the author for permission.
