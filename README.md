# The Aerell Programming Language

Aerell is an open-source programming language project.

<p align="center">
    <img alt="Aerell Programming Language Logo" src="https://raw.githubusercontent.com/aerell-lang/logo/refs/heads/main/logo.png" width="250">
</p>

## Status

Aerell is currently **under active development and experimentation**.
It is not stable yet, and breaking changes are expected as the language and tooling evolve.

## Requirements

To build Aerell, you need the following dependencies:

* LLVM
* LLD

This project currently uses **LLVM version 21.1.7**, built manually from the official LLVM GitHub repository.

## Build Preparation

Before building the project:

1. Copy the environment configuration file:

   ```bash
   .env.build.example -> .env.build
   ```

2. Edit `.env.build` and configure:

   * `CMAKE_BUILD_TYPE` (Debug or Release)
   * `CMAKE_INSTALL_PREFIX` (installation directory)

The environment file is used by the build scripts located in the `scripts/` directory.

## Build Scripts

The `scripts/` directory contains several helper scripts:

* `build.sh` — build the project
* `install.sh` — install the built artifacts
* `build_and_install.sh` — build and install in one step

Each script behaves according to its name and relies on the configuration from `.env.build`.

## Usage

After building, you can run the `aerell` executable. It provides a command-line interface:

```
A command-line utility for Aerell development.

Usage: aerell <command>

Available commands:
 help                Print this usage information.
 version             Print the Aerell compiler version.
 run <file>          Execute the source file using JIT compilation.
 build <file>        Build the source file into a standalone executable.

Available internal commands:
 lex <file>          Tokenize the source file and print the lexer output.
 parse <file>        Parse the source file and print the AST structure.
 analyze <file>      Perform semantic analysis on the source file.
 generate <file>     Generate LLVM IR from source and print the IR output.
 compile <file>      Compile the source file into an object file.
```

You can see example source code that can be run or compiled in the [`examples`](./examples) folder. These examples demonstrate the language features and provide a good starting point for learning how to write Aerell file.

The project is currently developed and tested on **Windows**. Other platforms are not fully supported yet, as some parts of the codebase are Windows-specific.

## License

This project is licensed under the [Apache License 2.0](./LICENSE).
See the LICENSE file for details.

You are free to use, modify, and distribute this project under the terms of the Apache License 2.0.