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

We acknowledge that when you take a simple program with global variables and encapsulate those variables in an object, it allows the program to process multiple items of work without those items interferring with one another. We further acknowledge that where we can treat these items as immutable, it brings many further advantages. These principles extend to software in general. In the case of a software project, they inform the following rules:

1. You should be able to clone your project multiple times into multiple separate working directories and *view* them in them in isolation from one another. Working directories can be located anywhere on the readable filesystem for *viewing*.
1. You should be able to configure, build, test and install your project in multiple separate build directories in isolation from one another and *use* them in isolation. Build directories can be located anywhere on the read/writeable filesystem for *using*. Working directories can be located anywhere on the readable filesystem for *using*.
1. You should be able to configure, build, test and install your project in multiple separate build directories in isolation from one another and *test* them in isolation. Build directories can be located anywhere on the read/writeable filesystem for *using*. Working directories can be located anywhere on the readable filesystem for *using* (with the exception below).
1. You should be able to clone your project multiple times into multiple separate working directories and *develop* them in isolation from one another. Working directories can be located anywhere on the read/writeable filesystem for *developing*.

#### Exception: Using the Working Directory as the Expected Result of a Test

Some tests are far easier to implement when the working directory is writeable.
They test an operation that is performed on the contents of the working directory.
They use the state of the working directory before the operation as the *expected result*, and the state after as the *actual result*.
Testing that expected and actual results are the same is a matter of testing whether there are unstaged changes.
This can be performed in a Git repository with `git diff --exit-code`.

These tests represent an exception to rule (3) and do not include unit tests; unit tests should *never* require a writeable working directory.

Like all the best tests, these tests must be deterministic on success.

Examples:

* Some linters work by applying fixes to linted files. If the files are well linted, this operation should not change the working directory, but it *will* write to it.
* Some tests of data transformations are performed at the filesystem level. Acceptance tests in which the code under test performs a large amount of work, may measure success in terms of whether the output is as expected - even if it's not clear *why* the output is exactly so. A functional change to the program may result in widespread change to the expected data. Updating acceptance tests accordingly may be a major effort, unless performing the test is the same task as changing the results.

### Isolation + Don't Glob

If you had two repos and none of the file paths were the same, it ought to be possible to store them in the same directory without any possibility of colision. 

### Be Kind to Your Future Self

### Don't Be Kind to Computers

Does my computer work too hard? Is the work I give my computer as easy as it can be? Can I save a byte here, an instruction there? 
Stop worrying about your computer.
Computers have no feelings (yet) and don't care *what* data goes through them. They are happy to execute poorly-organised programs full of spaghetti code and badly-named variables. Clearly they have poor taste and should not be consulted when making software design choices.

Yes, if you an algorithm with the wrong complexity guarantees, you computer will grind to a halt as datasets grow.
But even your choice of algorithm is a message to your future self: "this operation is random access", "the order of this sequence's must remain stable", etc..
If your function is _O(n^2)_, it's helpful to everyone (both your computer and and your fellow androids) if you care about that _(n^2)_.
But don't dwell on the _O_.

Write code which is easy to maintain and easy for your collaborators (and future self) to read.
It's easy to write code that is optimised for matinainability and then make it fast tomorrow.
It's hard to write code that is optimised for performance and then make it maintainable later.

It's even hard to write code that is optimised for performance today, and that still runs fast on computers later.
Imagine you are asked to optimise some critical infrastructure that is 10 years old and conclude that it needs to run on a modern GPU.
Do you think that will be easier to achieve if it is optimised for a human, or if it is optimised for a Pentium III?

### Entities Have Many Attributes But Only One Name

Naming is hard, really hard. Naming is also critical.
In order to Be Kind to Your Future Self, not only do you need to give an entity a good name, but if the entity changes, you must revise the name.
Two major pitfalls beset the budding namer:

Just because something is true of an entity doesn't mean it belongs in the name. Is your middle name Reader? Why not?! You're reading, aren't you?

A good rule of thumb: is the thing still well named if an incidental detail about it changes. E.g.

* `new_resource_manager` - is this still a good name after five years? If not, `new` is *circumstantial*.
* `resource_manager_2018` - is this still in use in 2019? Is not, `2018` has become *dated*.
* `book_resource_manager<T>` - sure, you're using this entity to manage books currently. But if `T` was `pebble`, `jingle` or `solar_system`, would it still do a good job managing those resources? If so, `book` is *overspecific*?

#### It's Possible to Name an Entity Without Naming It.

Now I've convinced you not to use `new`, `2018` or `book`, let's keep going.

Some words have little or no meaning in the context of software. Remember, software is just data and operations on data. Many badly-chosen names fit into one or both of those categories.

Useless proxies for 'data' include:

* `object`,
* `item`,
* `element`, and...
* `resource`!

Useless proxies for `operations on data` include:

* `process`,
* `run`,
* `complete`,
* `execute`, and...
* `manage`!

In highly generic code, these are good names for entities... occasionally. Most everywhere else, they are the absence of a name. You didn't spend the time to understand what the entity *was* so you gave up.

The result is that you're computer is happy. But you have made your future self sad.

### Avoid Circular Dependency

Dependency may arise in many settings. Header files that include one another are a sure sign then something is wrong.
Generally, where there are dependencies, the system can be visualised as a directed graph - a graph with arrows for edges.
If you can follow the arrows around in a circle, the graph is circular, or cyclic.
A cyclic graph is far harder to deal with than an acyclic graph for reasons that are beyond this document.

One example of where a cyclic graph is bad news, is when it describes the build and package systems.

Conan's [_cmake_ generator](https://docs.conan.io/en/latest/integrations/build_system/cmake/cmake_generator.html) is one way to provide a CMake project with its essential packages.
Thus, the Conan system refers to the CMake system within a codebase project.

But the _cmake_ generator requires that the CMake system includes Conan commands.
Thus, the CMake system refers to the Conan system within a codebase project.

This is not end of the world; most Conan+CMake projects use this generator quite happily.
But consider a project which wishes to remain package system-agnostic?
What if I wish to support vcpkg _as well as_ Conan?
The Conan-specific code in the CMake scripts are now unwanted at best, and breaking at worst.
Further, they're entirely unnecessary.

By using a decoupled generator, such as the [_cmake_find_package_ generator](https://docs.conan.io/en/latest/integrations/build_system/cmake/cmake_find_package_generator.html),
both package managers are supported in the same project and the CMake is cleaner to boot.

The moral of the story: don't go 'round in circles!
