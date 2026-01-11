# Syntax:
The syntax is very similar to Makefile syntax.

1. Target
    - target lines should begin with the target name, then a colon, then the dependencies
    - dependencies must be separated from each other by at least one space character (' ')
    - target lines must contain a colon character (':') separating the target name and the dependencies
    - any number of dependencies is ok including zero
    - targets without commands are allowed
2. Command
    - commands must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)
    - commands without a target are not allowed
    - multiple commands for one target are allowed

3. Comments
    - comments are denoted by the '#' character
    - inline comments are not allowed

- Whitespaces or the lack of whitespaces are allowed on any line as long as it follows these rules previously stated:
    - commands must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)
    - dependencies must be separated from each other by at least one space character (' ')

# Implemented:
- Makefile parsing (+invalid syntax detection)
- Support for multiple commands per target

# Limitations:
- The program will only read a file named exactly "Makefile"
- A single line in the Makefile can be at most 1023 characters long (including \n but not including \0)
- No variables, conditionals, etc.

# To do:
- Dependency graph construction
- Circular dependency detection
- Scheduler