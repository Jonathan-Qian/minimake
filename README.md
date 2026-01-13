# Minimake
A multithreaded build tool similar to GNU Make.

## Syntax:
The syntax is very similar to Makefile syntax.

### 1. Target
    - target lines should begin with the target name, then a colon, then the dependencies
    - dependencies must be separated from each other by at least one space character (' ')
    - target lines must contain a colon character (':') separating the target name and the dependencies
    - any number of dependencies is ok including zero
    - targets without commands are allowed
    - a target cannot have the same name as another target
### 2. Command
    - each command must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)
    - recipes without a target are not allowed
    - multiple commands for one target are allowed

### 3. Comments
    - comments are denoted by the '#' character
    - inline comments are not allowed

Whitespaces or the lack of whitespaces are allowed on any line as long as it follows these rules previously stated:
    - each command must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)
    - dependencies must be separated from each other by at least one space character (' ')

## Implemented:
- Makefile parsing (+invalid syntax detection)
- Support for multiple commands per target (recipe)
- Dependency graph construction
- Circular dependency detection
- Failure propagation
- Scheduler

### Note:
- Circular dependencies will only be detected if those dependencies are dependencies of the target provided as a command line argument
- Likewise, missing files (i.e. dependencies that aren't targets and are missing) will only be detected if the missing files are dependencies of targets that are dependencies of the target provided as a command line argument
- Like make, if the program is run without a specified target, it will pick the first target

## Limitations:
- The program will only read a file named exactly "Minimakefile"
- A single line in the Makefile can be at most 1023 characters long (including \n but not including \0)
- A command can have at most 63 arguments including the program name (perhaps I will change this in the future)
- No variables, conditionals, etc.
- Unlike make, duplicate targets are not supported
- Inputting multiple command line arguments to make multiple targets at once is not yet supported (ex. make test main)