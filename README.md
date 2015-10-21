# RIP-C

Yet another small program I wrote.

This program recusivley looks through all C-style files in the current directory and all sub directories. (included files are: .c .cc .cs .cpp .cxx .cbp .h .hpp .hxx)
Every semicolon in these files is then replaced with a greek question mark (which looks identical to the semicolon)
This will stop any C/C++/C# files from being able to be properly used as neither the compiler, nor an IDE will be able to properly recognize the greek question mark. This process is reversible by running the program with the "-r" option.

Use responsibly (or not, it's up to you)
