# Grape

An NFA-based regex engine in C++.

## How it works

- Tokenizes the input regex
- [Shunting-Yard Algorithm](https://en.wikipedia.org/wiki/Shunting_yard_algorithm) to convert the tokens to postfix notation
- [Thompson's Construction algorithm](https://en.wikipedia.org/wiki/Thompson%27s_construction) to construct a Nondeterministic Finite Automata (NFA) from the regex
- Simulate the NFA with the input string to find a match

<!-- TODO: add in a GIF of it being used-->

## Motivation

This project started in a [CodeCrafter's tutorial](https://app.codecrafters.io/courses/grep/overview). It recommended backtracking but when I was reading about this online I discovered that most regex engines use finite automata instead. I remembered studying them in my undergrad, so I decided to diverge from the tutorial and give it a go.

## Quick Start

### Mac

```
git clone https://github.com/CarolineMillan/grape.git
cd grape
chmod +x Scripts/Setup-Mac.sh
./Scripts/Setup-Mac.sh
```
You'll be prompted for your password to install grape to `/usr/local/bin`. After that you can use `grape` from anywhere.

### Linux

```
git clone https://github.com/CarolineMillan/grape.git
cd grape
chmod +x Scripts/Setup-Linux.sh
./Scripts/Setup-Linux.sh
```

You'll be prompted for your password to install grape to `/usr/local/bin`. After that you can use `grape` from anywhere.

### Windows

```
git clone https://github.com/CarolineMillan/grape.git
cd grape
Scripts\Setup-Windows.bat
```

Requires `make` to be available first — install via [Git for Windows](https://gitforwindows.org/), [MSYS2](https://www.msys2.org/), or Chocolatey (`choco install make`). After running the script, `grape.exe` will be copied to `C:\Windows\System32`.

NB: I don't have a Windows machine to test this on, so please let me know if it doesn't work.

## How to use grape

```grape -E <regex> [file]```

If the regex is found in the input file then it will print the matched line. If no input file is given it will wait for an input string to use instead.

## What's supported

- '\d' matches digits
- '\w' matches alphanumeric chars
- positive character class '[abc]' matches one of abc
- negative character class '[^abc]' matches any character that isn't one of abc
- '*' Star (A* accepts when there are 0 or more A's)
- '+' Plus (A+ accepts when there are 1 or more A's)
- '?' Question (A? is equiv to (nothing)|A)
- '|' Alt (A|B accepts when there is either A or B)
- '()' used for order of operations
- '.' Dot matches any character except \n
- Concatenation
- Literal characters
- '\s' matches whitespace

## Future Plans:

- [ ] recursively search a directory
- [ ] {n,m} support, counted repetition
- [ ] backreferences ("\(cat) and \1" matches "cat and cat" but not "cat and dog")
- [ ] highlighting
- [ ] line numbers
- [ ] write proper tests using a testing framework like Catch2
- [ ] ^ start anchor
- [ ] $ end anchor

### Maybe one day
- [ ] use subset construction algorithm to convert the NFA to DFA

## Acknowledgements and Resources Used

- [CodeCrafters grep tutorial](https://app.codecrafters.io/courses/grep/overview) (the starting point for this project)
- [Rob Pike and Brian Kernighan's article on regex and backtracking](https://www.cs.princeton.edu/courses/archive/spr09/cos333/beautiful.html) (which led me to explore NFA-based approaches instead)
- [Russ Cox's article on regex and finite automata](https://swtch.com/~rsc/regexp/regexp1.html)
- Wikipedia articles on Thompson's construction and shunting yard algorithm
- I used AI as a sounding board to test understanding and giving feedback on implementation approaches
## Contributing

If you'd like to contribute, please fork the repository and open a pull request on the ```main``` branch.

## License

This software is available as open source under the terms of [the MIT license](LICENSE).
