# Minimake
A multithreaded build tool similar to GNU Make. The number of threads Minimake uses is equal to the number of logical processors on the machine running it. Only Linux/Unix-like operating systems are supported so far.

## Usage:
To build Minimake,
1. set your current directory to the directory where you have the source code files in your terminal;
2. have make installed then run `make` or `make all`.

After building the "minimake" executable, move it anywhere you like and add it to your PATH if you would like.

To use Minimake, run the program in a directory that has a Minimakefile. You can enter one or zero command line arguments in addition to the executable path to choose what target to build. If you don't specify a target, Minimake will choose the first target in your Minimakefile.

Although Minimake is inspired by Make, it does not behave exactly like Make. It might be a good idea to take a look at the syntax and limitations sections.

## Syntax:
The syntax for a Minimakefile is very similar to Makefile syntax.

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

Whitespaces or the lack of whitespaces are allowed on any line as long as it follows these rules that were previously stated:\
    - each command must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)\
    - dependencies must be separated from each other by at least one space character (' ')

## Implemented:
- Makefile parsing (+invalid syntax detection)
- Support for multiple commands per target (recipe)
- Dependency graph construction
- Circular dependency detection
- Failure propagation
- Scheduler

## Limitations:
- The program will only read a file named exactly "Minimakefile"
- A single line in the Makefile can be at most 1023 characters long (including \n but not including \0)
- A command can have at most 63 arguments including the program name (perhaps I will change this in the future)
- Only the features described in the syntax section are supported (i.e. no variables, conditionals, etc.)
- Unlike make, duplicate targets are not supported
- Inputting multiple command line arguments to make multiple targets at once is not yet supported (ex. make test main)
- Circular dependencies will only be detected if those dependencies are descendant dependencies of the target provided as a command line argument
- Likewise, missing files (i.e. dependencies that aren't targets and are missing) will only be detected if the missing files are dependencies of targets that are descendant dependencies of the target provided as a command line argument
