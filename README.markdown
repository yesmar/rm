# RM — Resource Manager

Copyright © 2018, Ramsey Dow. All rights reserved.

Inspired by Alexey Kutepov's talk, [How I Manage Resources in Pure C -- Life Time](https://www.youtube.com/watch?v=ncRv9fCxt_w), I set out to build a C resource manager of my own.

To build:

```bash
mkdir -p build && pushd !#:2
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On -GNinja ..
ninja -v
```

To analyze:

```bash
pvs-studio-analyzer analyze
plog-converter -a GA:1,2 -t tasklist -o pvs.tasks PVS-Studio.log
```

Any findings will be detailed in the `pvs.tasks` file.

To count source lines of code:

```bash
cloc --exclude-dir build,.vscode --exclude-lang CMake .
```
