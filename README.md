Limitations:
- The program will only read a file named exactly "Makefile"
- A single line in the Makefile can be at most 1023 characters long (including \n but not including \0)
- A target cannot have multiple commands yet (this will be supported later)
- Whitespaces are allowed on any line as long as it follows this rule: the line containing the command for a target MUST start with a tab (\t character);
- No variables, conditionals, etc.