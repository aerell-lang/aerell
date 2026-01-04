/*
 * This file is part of The Aerell Programming Language
 *
 * Copyright 2025 The Aerell Programming Language Authors
 * Licensed under the Apache License, Version 2.0.
 * See the LICENSE file for details.
 */

#ifndef AERELL_CLI_CLI_H
#define AERELL_CLI_CLI_H

namespace aerell
{

class CLI
{
  public:
    int main(int argc, const char* argv[]);

  private:
    void printVersion();
    void printHelp();
};

} // namespace aerell

#endif