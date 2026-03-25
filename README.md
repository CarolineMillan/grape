# Grape

A regex engine in C++ built on Thomson's NFA algorithm.

<!-- TODO: add in a GIF of it being used-->

## Quick Start

## Motivation

This project started in a [codecrafter's tutorial](https://app.codecrafters.io/courses/grep/overview). It recommended backtracking but when I was reading about this online I discovered that most regex engines use finite automata instead. I remembered studying them in my undergrad, so I decided to diverge from the tutorial and give it a go.

## How it works -- the theory and why NFA is faster than backtracking

## How to use grape

You will be prompted for your password to install grape to ```/usr/local/bin```.

### Windows

Requires `make` to be available — install via [Git for Windows](https://gitforwindows.org/), 
[MSYS2](https://www.msys2.org/), or [Chocolatey](https://chocolatey.org/) (`choco install make`).

### Linux

### MacOS

(these are for me to use while working on the project)

to build:
```make config=debug```

to use:

```grape -E <regex> [file]```

if the regex is found in the input string then it will print the matched line

## What's supported

- '\d' matches digits
- '\w' matches alphanumeric chars
- positive character class '[abc]' matches one of abc
- negative character class '[^abc]' matches any character that isn't one of abc
- '*' Star matches when there are 0 or more characters matching the character preceding the '*'. i.e. A* matches 0 or more As.
                                // A* accepts when there are 0 or more A's
- '+' Plus matches 
                                // A+ accepts when there are 1 or more A's
- '?' Question
                                // A? is equiv to (nothing)|A
- '|' Alt
                                // A|B accepts when there is either A or B
- '()' used for order of operations
- '.' Dot
- Concatenation
- Literal characters
- ^ start anchor
- $ end anchor
- '\s' matches whitespace

currently searches for a substring in a given input file. If no input file is given it will wait for an input string.


## Future Plans:

### Immediate
- [ ] setup github actions
- [ ] writeup the readme properly
- [ ] write proper tests using a testing framework like Catch2

### Future:
- [ ] recursively search a directory
- [ ] {n,m} support, counted repetition
- [ ] backreferences ("\(cat) and \1" matches "cat and cat" but not "cat and dog")
- [ ] highlighting
- [ ] line numbers

### Maybe one day
- [ ] use subset construction algorithm to convert the NFA to DFA

## Acknowledgements and Resources Used

## Contributing

If you'd like to contribute, please fork the repository and open a pull request on the ```main``` branch.

## Licence
