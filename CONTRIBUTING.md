# Contributing

This document is a ***First Draft*** and missing many items.
Please provide feedback and ask questions so I can improve it.

Contributions are more than welcome! Please read this guide to help ensure your
effort has more effect.

## Submitting Issues

Feel free to [add an issue](https://github.com/johnmcfarlane/cnl/issues/) via
[email](cnl.contributing@john.mcfarlane.name).

You are encouraged to choose one of the [issue templates](.github/ISSUE_TEMPLATE).
Bugs submitted in the form of pull requests with failing tests are the best way
to submit an issue. Alternatively, examples of code using CNL can be shared
using Compiler Explorer ([example](https://godbolt.org/z/fnrrxY7q4)).

## Help Wanted

There is a label, [help wanted](https://github.com/johnmcfarlane/cnl/labels/help%20wanted),
which mostly marks issues about which I have limited expertise. If you have the
specific know-how needed to contribute to any of these tasks, please let me
know.

Other ways to improve the codebase (and learn the project) include:

* removing the exceptions in the list of checks in the [Clang-Tidy configuration](.clang-tidy);
* removing platform-specific code, e.g. sections wrapped in `#if defined(__clang__)`;
  and
* addressing [_TODO_](https://github.com/johnmcfarlane/cnl/search?q=TODO) comments.

Many of these changes are difficult or impossible to make. But some will be
straight-forward.

## Pull Requests

Pull requests should be made against the [main branch](https://github.com/johnmcfarlane/cnl).
When writing commit messages, please follow [these guidelines](https://chris.beams.io/posts/git-commit/)
and try to consider how useful your commit message will be when anyone tries to
use `git log` or `git blame` to understand your change. Single-commit PRs will
be rebased. Multi-commit PRs will be merged. Ideally, all commits should pass
all tests in the CI workflows.

PRs should achieve one coherent *thing* or enhance the library in one single
*way*. Changes and additions to public-facing APIs should be accompanied by tests.
Refactors should tend not to change tests.

When writing commit messages, assume the *cnl* directory and the `cnl`
namespace, e.g.:

> Move _impl::is_fixed_point into its own header
>
> * _impl/fixed_point/is_fixed_point.h

## Workflow

See the _Test_ section of the [README](README.md#test) for details of how to
test that your changes haven't broken anything. A basic understanding of CMake,
Conan and some popular compilers will be helpful. (But if you don't have
these, contributing to an open source project is a great way to acquire them!)

## Tests

Please use the contents of [test/unit](https://github.com/johnmcfarlane/cnl/blob/develop/test/unit)
as an example of how to write CNL tests. It's a little chaotic in there
(sorry) so look at newer commits when determining what example to follow.
Prefer `static_assert` tests to run-time tests, try and keep source files
small and dedicted to a single library component or combination of
components. Follow the project directory structure and code style.

## Markdown

Being a GitHub project, I lean toward formatting with [GitHub flavored
markdown](https://github.github.com/gfm/) but [CommonMark](https://commonmark.org/)
is great too.

Files are not computer code so do not format them with tickmarks (\`). Use
emphasis instead, e.g.

```markdown
To use `cnl::scaled_integer`, include *cnl/scaled_integer.h*.
```

looks like:

> To use `cnl::scaled_integer`, include *cnl/scaled_integer.h*.

## Directory Structure

### *include* directory

This is the library include directory. Client code is expected to feature it
in its system search path (e.g. with `-isystem cnl/include`) and include the
files in [_include/cnl_](include/cnl). The coverall header,
[include/cnl/all.h](include/cnl/all.h), should contain most top-level,
public-facing APIs and is intended to be included:

```C++
#include <cnl/all.h>
```

The exception is the content of [include/cnl/auxiliary](include/cnl/auxiliary)
which contains experimental integration (glue) code for other 3rd-party
libraries and which must be pulled in explicitly.

The contents of [include/cnl/_impl](include/cnl/_impl) are off-limits to
library users. The same is true for the `cnl::_impl` namespace.

## Code Style

### Namespaces

Fully qualify identifiers in *test*. The exceptions is `cnl::_impl::identical`
which is never the subjects of tests. Keep things out of the global namespace
where possible. Wrap individual compile-time tests in a separate
`test_some_feature` as exemplified throughout most of the test suite.

## Philosophy

### Fail Loud

### `grep`ability

`grep`ability is a specific form of traceability, and traceability is a good thing.
Especially important when creating a reference between two things, ask yourself: is this identifier greppable?
In other words, how easy will it be to search for something in order to find out more about it.

### Don't Glob

Absence of a file is not the filing of absense.

If you want to add a CMake library consisting of all of the .cpp files in a directory, list those files explicitly - ideally in an [`add_library`](https://cmake.org/cmake/help/latest/command/add_library.html) call.

Named elements and their points of reference links in a graph.
It should be reasonably easy to traverse this graph.
Globbing obscures the link.

If you need a file but you accidentally deleted it, you want to Fail Loud.
But globbing silences this failure.

### Isolation and Multiplexity

We acknowledge that when you take a simple program with global variables and encapsulate those variables in an object, it allows the program to process multiple items of work without those items interferring with one another. We further acknowledge than where we can treat these items as immutable, it brings many further advantages. These principle extends to software in general. In the case of a software project, informs the following principles:

* You should be able to clone your project multiple times into multiple separate working directories and *view* them in them in isolation from one another. Working directories can be located anywhere on the readable filesystem for *viewing*.
* You should be able to configure, build, test and install your project in multiple separate build directories in isolation from one another and *use* them in isolation. Build directories can be located anywhere on the read/writeable filesystem for *using*.
* You should be able to clone your project multiple times into multiple separate working directories and *develop* them in them in isolation from one another. Working directories can be located anywhere on the readable filesystem for *developing*.

### Isolation + Don't Glob

If you had two repos and none of the file paths were the same, it ought to be possible to store them in the same directory without any possibility of colision. 

### Being Kind to Your Future Self

