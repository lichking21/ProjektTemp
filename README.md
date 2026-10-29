# Console Rule Engine

A small C++20 console prototype. Rules are kept in memory and are lost on exit.

## Build

Requires CMake 3.14+ and a C++20 compiler.

With MinGW GCC and mingw32-make on PATH:

```powershell
cmake -S . -B build -G "MinGW Makefiles"
cmake --build build
.\build\app.exe
```

With Visual Studio C++ build tools, use a separate build directory:

```powershell
cmake -S . -B build-vs
cmake --build build-vs --config Debug
.\build-vs\Debug\app.exe
```

## Commands

- `add`: enter scope (`global` or `local`), match type (`exact` or `wildcard`),
  pattern, and folder. Leave the folder empty for global rules.
- `list`: show all rules and their fields.
- `remove`: enter a rule ID.
- `check`: enter the parent folder and an entry name to see `Hidden` or `Visible`.
- `exit`: quit.

Invalid input prints an error and returns to the prompt.

## How it works

`main` creates one `RuleStore`, shared by the CLI and `RuleEngine`.
`Add` returns a positive 64-bit ID, or 0 for an invalid rule.
`Remove` returns whether a rule was removed.

Patterns cannot be empty. Local rules require a folder and apply only to its
direct children. Global rules require an empty folder and apply everywhere.
A name is hidden if any rule matches it.

Exact matching compares the whole name. Wildcard matching supports `*` for zero
or more bytes and `?` for one byte. Comparisons ignore ASCII letter case.
Folder comparisons accept both slash styles and ignore trailing separators
without changing drive roots such as `C:/` into `C:`.

Unicode case folding is not supported, and `?` matches a byte rather than a
Unicode character. Paths are compared as text; aliases, symlinks, and dot
segments are not resolved.

## Later: kernel integration

- Define the user/kernel communication protocol for rule updates.
- Add a Windows minifilter to filter directory entries.
- Handle Windows Unicode names and filesystem paths correctly.
- Add kernel-safe memory management and synchronization.
- Set up driver signing and verify the driver in a virtual machine.

This version only reports matching decisions. It has no driver, Qt UI, or disk
persistence.
