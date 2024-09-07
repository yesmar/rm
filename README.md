# RM — Resource Manager

Copyright © 2018, Ramsey Dow. All rights reserved.

Inspired by Alexey Kutepov's talk, [How I Manage Resources in Pure C -- Life Time](https://www.youtube.com/watch?v=ncRv9fCxt_w), I set out to build a C resource manager of my own. Unlike Alexey's implementation, which makes use of the heap, my implementation is purely stack based. There will always be user-specified allocations and deallocations. That is, after all, what the resource manager is managing. However, from a framework perspective, there are no allocations or deallocations. Everything is managed on the call stack.

To build:

```bash
mkdir -p build && pushd !#:2
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=On -GNinja ..
ninja
```

If you don't use [Ninja](https://ninja-build.org) (or simply prefer to use `make`), get rid of the `-GNinja` switch and replace `ninja` with `make`.

Note that the project is configured to compile against C11, since that is what I use day-to-day. However, the code is C99 compliant. If you wish to compile it against the C99 standard, just edit the `CMakeLists.txt` file in the root directory. Change the value of `C_STANDARD` from `11` to `99` and rebuild the project.

Neither the framework nor the test driver emit output under normal circumstances. In fact, you should only see output if an error occurred. You can always `echo $?` to verify that things ran correctly (in which case you will see a `0`).

Assuming you have [PVS-Studio](https://www.viva64.com/en/pvs-studio/) installed, you can perform static analysis thusly:

```bash
pvs-studio-analyzer analyze
plog-converter -a GA:1,2 -t tasklist -o pvs.tasks PVS-Studio.log
```

Any findings will be detailed in the `pvs.tasks` file.

To count source lines of code:

```bash
cloc --exclude-dir build,.vscode --exclude-lang CMake .
```

RM is released under the [MIT License](https://opensource.org/licenses/MIT).
