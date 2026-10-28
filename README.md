# shell-c

A UNIX shell implementation written in C, built via [CodeCrafters](https://codecrafters.io).

![Language](https://img.shields.io/badge/language-C-blue.svg)
![Status](https://img.shields.io/badge/status-Work%20in%20Progress-orange.svg)

## Overview

`shell-c` is a POSIX-compliant interactive shell built from scratch to explore low-level process lifecycle management, file descriptor manipulation, I/O redirection, and string parsing in C.

## Features

- [x] **REPL Loop**: Interactive shell prompt reading commands from `stdin`.
- [x] **Built-in Commands**: `exit`, `echo`, `pwd`, `cd`, and `type`.
- [x] **PATH Resolution**: Locating and executing system binaries via the `PATH` environment variable.
- [ ] **Quoting & Escaping**: Single quotes `'`, double quotes `"`, and backslash `\` escape handling. *(WIP)*
- [ ] **I/O Redirection**: Redirection support for standard output (`>`), standard error (`2>`), and append mode (`>>`). *(Planned)*
- [ ] **Pipelining**: Command chaining with pipes (`|`). *(Planned)*

## Prerequisites

- `gcc` or `clang`
- POSIX-compliant environment (Linux, macOS, WSL)

## Getting Started

### Building & Running

Clone the repository and compile using `gcc`:

```bash
git clone https://github.com/xFloras/shell-c.git
cd shell-c

# Compile manually
gcc  *.c -o shell

# Run
./shell

```
### Acknowledgments
Built as part of the "Build your own Shell" challenge on [CodeCrafters](https://codecrafters.io/).
