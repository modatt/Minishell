# Minishell

A lightweight implementation of a Unix shell, inspired by Bash. Minishell provides a functional command-line interpreter with support for built-in commands, pipelines, redirections, and environment variable handling.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Built-in Commands](#built-in-commands)
- [Technical Details](#technical-details)
- [Project Structure](#project-structure)
- [Requirements](#requirements)
- [Authors](#authors)

## Overview

Minishell is a Unix shell implementation written in C that mimics the behavior of Bash. It provides an interactive command-line interface where users can execute commands, manage processes, and manipulate the environment. This project demonstrates core shell concepts including parsing, process management, signal handling, and I/O redirection.

## Features

### Core Functionality
- **Interactive Mode**: Displays a prompt and waits for user commands
- **Non-Interactive Mode**: Executes commands from files or piped input
- **Command Parsing**: Robust tokenization and syntax validation
- **Quote Handling**: Support for single (`'`) and double (`"`) quotes
- **Environment Variables**: Expansion of `$` variables and special parameters
- **Exit Status**: Track and access the last command's exit status via `$?`

### I/O Redirection
- **Input Redirection** (`<`): Redirect input from files
- **Output Redirection** (`>`): Redirect output to files
- **Append Mode** (`>>`): Append output to files
- **Here Documents** (`<<`): Multi-line input with delimiter

### Process Management
- **Pipelines** (`|`): Chain multiple commands together
- **External Commands**: Execute binaries from PATH
- **Process Control**: Proper fork/exec handling and cleanup

### Signal Handling
- **SIGINT** (Ctrl+C): Interrupt current command
- **SIGQUIT** (Ctrl+\\): Quit signal handling
- **EOF** (Ctrl+D): Clean shell exit

## Installation

### Prerequisites
- GCC or compatible C compiler
- GNU Make
- GNU Readline library
- Unix-like operating system (Linux, macOS, WSL)

### Build Instructions

1. Clone the repository:
```bash
git clone <repository-url>
cd Minishell
```

2. Compile the project:
```bash
make
```

3. The executable `minishell` will be created in the root directory.

### Additional Make Targets
```bash
make clean   # Remove object files
make fclean  # Remove object files and executable
make re      # Recompile the entire project
```

## Usage

### Interactive Mode
Launch the shell and start entering commands:
```bash
./minishell
minishell$ echo "Hello, World!"
Hello, World!
minishell$ pwd
/home/user/Minishell
minishell$ exit
```

### Non-Interactive Mode
Execute commands from a file or pipe:
```bash
./minishell < script.sh
echo "ls -la" | ./minishell
```

### Example Commands
```bash
# Basic commands
minishell$ ls -l
minishell$ cat file.txt

# Pipelines
minishell$ ls | grep minishell | wc -l

# Redirections
minishell$ echo "test" > output.txt
minishell$ cat < input.txt >> output.txt

# Here documents
minishell$ cat << EOF
> line 1
> line 2
> EOF

# Environment variables
minishell$ echo $HOME
minishell$ export MY_VAR="value"
minishell$ echo $MY_VAR

# Built-in commands
minishell$ cd /tmp
minishell$ pwd
minishell$ env
```

## Built-in Commands

| Command | Description |
|---------|-------------|
| `echo` | Display a line of text (supports `-n` flag) |
| `cd` | Change the current directory |
| `pwd` | Print the current working directory |
| `export` | Set environment variables |
| `unset` | Remove environment variables |
| `env` | Display all environment variables |
| `exit` | Exit the shell (supports exit codes) |

## Technical Details

### Architecture

The project is organized into two main modules:

1. **Parser Module** (`parse/`)
   - Lexical analysis and tokenization
   - Syntax validation
   - Environment variable expansion
   - Command structure creation
   - Signal handling setup

2. **Executor Module** (`execute/`)
   - Built-in command execution
   - External command execution
   - Pipeline management
   - I/O redirection handling
   - Process lifecycle management

### Key Components

- **Environment Management**: Linked list structure for environment variables
- **Command Structure**: Supports complex command chains with redirections
- **Memory Management**: Comprehensive cleanup and leak prevention
- **Error Handling**: Proper error reporting and recovery
- **Signal Handling**: Interactive and non-interactive signal modes

### Parsing Flow
1. Read input line using GNU Readline
2. Tokenize input respecting quotes and special characters
3. Validate syntax (unclosed quotes, invalid operators, etc.)
4. Expand environment variables
5. Build command structure with redirections and arguments
6. Execute command or pipeline

### Execution Flow
1. Check if command is a built-in
2. For built-ins: Execute directly in the shell process
3. For external commands: Fork and exec with proper environment
4. For pipelines: Create pipe chains and manage multiple processes
5. Handle all redirections before command execution
6. Update exit status and clean up resources

## Project Structure

```
Minishell/
├── Makefile                 # Build configuration
├── README.md                # This file
├── readline_curses.supp     # Valgrind suppression file
├── test.bash                # Test script
├── includes/
│   └── minishell.h          # Main header file
├── libft/                   # Custom C library
│   ├── *.c                  # Standard C functions
│   ├── libft.h              # Library header
│   └── Makefile             # Library build file
├── parse/                   # Parsing and input handling
│   ├── minishell.c          # Main entry point
│   ├── parser.c             # Command parser
│   ├── parse_utils*.c       # Parsing utilities
│   ├── syntax_check*.c      # Syntax validation
│   ├── env_handler*.c       # Environment handling
│   ├── signals*.c           # Signal management
│   └── *.c                  # Other parsing modules
└── execute/                 # Command execution
    ├── executor.c           # Main executor
    ├── exec_buildins*.c     # Built-in commands
    ├── pipeline*.c          # Pipeline handling
    ├── redirections*.c      # I/O redirection
    ├── exce_external*.c     # External command execution
    ├── export*.c            # Export command
    ├── unset.c              # Unset command
    ├── exit*.c              # Exit command
    ├── cd.c                 # Change directory
    └── *.c                  # Other execution modules
```

## Requirements

- **Compiler**: GCC with C99 standard support
- **Compiler Flags**: `-Wall -Wextra -Werror`
- **Libraries**: 
  - GNU Readline (`-lreadline`)
  - Standard C library
- **Operating System**: Unix-like (Linux, macOS, BSD, WSL)

## Authors

This project was developed as part of the 42 School curriculum.

- **modat** - modat@student.42.fr

## License

This project is created for educational purposes as part of the 42 School curriculum.

---

**Note**: This shell is designed for learning purposes and may not include all features of production shells like Bash or Zsh.
