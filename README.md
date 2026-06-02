JSON-Parser
===========

Overview
--------

This project is a compact, hand-written JSON parser and interactive command-line shell implemented in modern C++. It exposes a small but powerful programmatic representation of JSON values (primitive types, arrays and objects) through a class hierarchy rooted at `Statement`. The shell lets users load a JSON document, inspect and modify it, search for keys, and persist changes back to disk. The code favors explicit ownership and manual memory management: objects and arrays own their children and delete them in destructors, and the parser/tokenizer allocate temporary Statement objects that get transferred or freed deterministically.

Purpose and audience
--------------------

This repository is meant for readers who want a clear example of building a JSON parser from first principles in C++: how to tokenize input, build an AST-like structure, represent JSON semantics with types, and provide an imperative command interface for editing JSON. It is useful as a learning reference, a small utility for quick JSON editing, and a starting point for enhancements (streaming parsing, UTF-8 handling, schema validation, etc.).

Design and architecture
-----------------------

At the core is a minimal runtime representation for JSON values. The abstract `Statement` class defines the interface every JSON node implements. Concrete subclasses include `ObjectStatement`, `ArrayStatement`, `StringStatement`, `NumberStatement`, `BoolStatement`, and `NullStatement`. Objects contain a vector of `KeyValuePair` objects; arrays contain a vector of `Statement*` pointers. The ownership model is simple: when a container stores a child pointer, it owns it and deletes it in its destructor. `KeyValuePair` also owns its `Statement*` value and deletes or transfers it as needed.

Parsing is split into a tokenizer (lexer) and a recursive-descent parser. The tokenizer reads input and produces `Token` values; some tokens represent structural symbols (braces, brackets, commas), while value tokens carry newly-allocated `Statement*` instances (for numbers, strings, booleans and null). The parser consumes tokens and assembles the object/array structures, taking ownership of returned `Statement*` nodes. Error handling is mostly exception-based: when an error occurs the parser/lexer clean up any temporary heap allocations they own before propagating errors.

Memory and ownership notes
-------------------------

This project uses explicit `new` and `delete` rather than smart pointers. That makes ownership straightforward to understand at a glance: containers delete their children, and copy/assignment operations either copy deeply or transfer ownership as appropriate. Because manual memory management is brittle, the code carefully frees temporary allocations produced by the tokenizer on error paths and after their values have been consumed. If you plan to extend the code, consider introducing `std::unique_ptr<Statement>` to enforce ownership invariants and reduce accidental leaks.

Command-line shell: how it works
--------------------------------

The interactive shell exposes an imperative interface to a single in-memory JSON document. Typical session flow is: open a file, inspect or modify data, then save changes back to disk. Commands are textual and parsed by the shell; each operation maps to functions that manipulate the `Statement*` tree.

Key commands

The shell provides a compact set of operations for common tasks. Instead of enumerating each command as a checklist, the following paragraphs explain the most important ones and how they behave.

Open and close

Use the open command to load a JSON file into memory. The parser will validate the JSON as it reads; if parsing fails the shell reports an error and does not change the current in-memory document. Closing clears the current document from memory and frees all nodes.

Print and validate

Printing pretty-prints the in-memory document with sensible indentation. Validation is trivial: because parsing builds the in-memory representation or throws on syntax errors, a non-empty `root` means the document is syntactically valid.

Searching and extraction

The search operation walks the entire JSON tree and returns small object matches for keys that match a pattern. Patterns accept a trailing `*` for prefix matching. Search results are returned as an array of single-property objects, making it easy to inspect or save matched fragments.

Path-based mutations: set, create, delete, move

Paths use a slash-delimited syntax similar to a file path. Each segment can be a property name or a numeric index for arrays. `set` replaces the value at a path; `create` inserts a new value at a path, creating intermediate containers if needed; `delete` removes a property or array element. `move` copies a node from one path to another and deletes the original; the operation is careful not to allow moving an element into its own descendant.

Saving

`save` writes the current document (or a selected subnode) to disk, formatting the JSON in a human-friendly style. `saveas` lets you write to a new filename.

Examples of usage
-----------------

Start the shell and open a file:

```
./main
open example.json
print
```

Set a nested value (example):

```
set /users/2/name "Alice"
```

Create a new nested object, creating containers along the way:

```
create /config/display/brightness 0.8
```

Move an element from one path to another:

```
move /items/3 /archive/0
```

Design trade-offs and limitations
--------------------------------

This parser is intentionally small and synchronous. It does not support streaming incremental parsing for very large files, and it uses manual memory management which requires careful handling of ownership on exceptional flows. There is limited support for Unicode escape sequences in strings (the code handles common escapes like `\n`, `\t`), but full Unicode decoding and normalization is not implemented.

The shell is synchronous and single-document: it keeps one `root` in memory. Concurrency, concurrent editing, JSON schema validation, and JSON Pointer (RFC 6901) compatibility are out-of-scope for the current implementation.

Building and running
--------------------

The code is platform portable and builds with any standards-compliant C++17 compiler. A typical build invocation using g++ from the project root is:

```
g++ -std=c++17 -O2 -Wall -Wextra -pedantic *.cpp -o main

./main
```

There is no build system included—if you prefer CMake or a small Makefile, adding one is straightforward: list the .cpp sources and compile into an executable.

Troubleshooting
---------------

If parsing fails with a message about invalid tokens, check that the JSON file is syntactically valid (matching braces/brackets, properly quoted strings).

License
-------

This repository includes a LICENSE file. Refer to it for the exact terms under which the project is released.

Contact and further notes
-------------------------

This project is for educational purposes. To contact, go to https://linktr.ee/gergana.hristanova.