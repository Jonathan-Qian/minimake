# Minimake
A multithreaded build tool similar to GNU Make. The number of threads Minimake uses is equal to the number of logical processors on the machine running it. Only Unix-like operating systems are supported so far. 

Although Minimake is inspired by Make, it does not behave exactly like Make. It might be a good idea to take a look at the syntax and limitations sections.

## Usage
### Build
To build Minimake, ensure you have `make` and `gcc` installed. Run the following in your terminal:
```bash
git clone https://github.com/Jonathan-Qian/minimake.git
cd minimake
make
```

### Install on Linux/macOS
After building the `minimake` executable, move it to a location in your system's PATH to avoid having to type out the entire file path of the executable everytime you want to use Minimake.

#### Option A: Install system-wide
Move the executable to `/usr/local/bin`. This usually requires administrator privileges but makes the tool available to all users.

```bash
sudo mv minimake /usr/local/bin/
```

#### Option B: Install for current user only
If you prefer not to use sudo, move it to a local bin folder:

```bash
mkdir -p ~/bin
mv minimake ~/bin/
```

Note: If you choose Option B, ensure `~/bin` is in your PATH by adding `export PATH="$HOME/bin:$PATH"` to your .bashrc or .zshrc file.

### Run
To use Minimake, run the program in a directory that has a Minimakefile.

```bash
minimake [target]
```

If you don't specify a target, Minimake will choose the first target in your Minimakefile.

## Syntax
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

Whitespaces or the lack of whitespaces are allowed on any line with these exceptions as previously st:\
    - each command must start with a tab character ('\t') (i.e. there cannot be any other whitespace in front of the tab)\
    - dependencies must be separated from each other by at least one space character (' ')

## Implemented
- Makefile parsing (+ invalid syntax detection)
- Support for multiple commands per target (recipe)
- Dependency graph construction
- Circular dependency detection
- Failure propagation
- Scheduler

## Limitations
- The program will only read a file named exactly "Minimakefile"
- A single line in the Makefile can be at most 1023 characters long (including \n but not including \0)
- A command can have at most 63 arguments including the program name (perhaps I will change this in the future)
- Only the features described in the syntax section are supported (i.e. no variables, conditionals, etc.)
- Unlike make, duplicate targets are not supported
- Inputting multiple command line arguments to make multiple targets at once is not yet supported (ex. make test main)
- Circular dependencies will only be detected if those dependencies are descendant dependencies of the target provided as a command line argument
- Likewise, missing files (i.e. dependencies that aren't targets and are missing) will only be detected if the missing files are dependencies of targets that are descendant dependencies of the target provided as a command line argument

## To Do
- Allow the use of variables
- Use '\\' as a line continuation and escape character
- Allow multiple targets to be built at once (will need hash set)
- Implement implicit rules
- Implement phony target behaviour to be like make
