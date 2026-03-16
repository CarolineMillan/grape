# grape

Grape is a regex engine built on Thomson's NFA algorithm. Currently a work in progress.

## Motivation

This project started in a [codecrafter's tutorial](https://app.codecrafters.io/courses/grep/overview). It recommended backtracking but when I was reading about this online I discovered that most regex engines use finite automata instead. I remembered studying them in my undergrad, so I decided to diverge from the tutorial and give it a go.

## How it works -- the theory and why NFA is faster than backtracking

## How to use grape

to build:
```make config=debug```

to use:
```grape -E <regex> <file.txt>```
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

currently searches for a substring in a given input file


## Plans:

### Immediate
- [ ] setup github actions
- [ ] writeup the readme properly
- [X] adding anchors ^ and $
- [X] reading from files and stdin
- [X] print matching line rather than true/false
- [X] adding'\s' for matching whitespace
- [X] substring matching
- [X] search multiple files
- [ ] recursively search a directory

### Future:
- [ ] {n,m} support, counted repetition
- [ ] backreferences
- [ ] highlighting

### Maybe one day
- [ ] use subset construction algorithm to convert the NFA to DFA
