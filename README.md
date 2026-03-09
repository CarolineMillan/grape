# grape

Grape is a regex engine built on Thomson's NFA algorithm. Currently a work in progress.

## Motivation

This project started in a [codecrafter's tutorial](https://app.codecrafters.io/courses/grep/overview). It recommended backtracking but when I was reading about this online I discovered that most regex engines use finite automata instead. I remembered studying them in my undergrad, so I decided to diverge from the tutorial and give it a go.

## How it works -- the theory and why NFA is faster than backtracking

## How to use grape

to build:
```make config=debug```

## What's supported


## Plans:

### Immediate
- [ ] adding anchors ^ and $
- [ ] reading from files and stdin
- [ ] print matching line rather than true/false
- [X] adding'\s' for matching whitespace
- [X] substring matching

### Future:
- [ ] multiline support
- [ ] {n,m} support, counted repetition
- [ ] highlighting
