# Welcome to Kof's abstract math crash course!

![Operator Omikron](OperatorOmikron_24.png)

Two small C tools live here: **omicron** plays every possible combination
of operators between letter-numerals, **kofsum** is its quick checksum
companion.

## Greekset

Letters of an alphabet double as numerals from 1..N

where `A = 1, B = 2, Γ = 3 and Δ = 4`

The original Greekset had 24 letters, `Α..Ω` = 1..24;
this implementation runs on the Latin alphabet, `a..z` = 1..26.



## Special Operator = Omicron (better symbol yet to come)

Operator Omicron stands for variable in mathematical operator symbol.

Omicron can be either "+", "−", "×", "÷" ...etc. the variable is for the operation itself:

o = any operation:

```
aοbοcοd = ...
```

..please notice that, while having fun all numbers are still pretty, and so on



## What the program does

`omicron` takes the first N numerals — `a = 1`, `b = 2`, ..., up to `z = 26` —
and substitutes every possible combination of Omicrons between them:

```
a ο b ο ... ο z
```

Each gap holds one of four operations (`+ − × ÷`), so N letters give
`4^(N-1)` expressions, evaluated strictly left-to-right.
For example, with `-n 4` there are exactly `4³ = 64` of them:

```
$ ./build/omicron -n 4
a + b + c + d = 10
a + b + c − d = 2
a + b + c × d = 24
...
```

Run without arguments you get all 26 letters: `4²⁵ ≈ 1.1 quadrillion`
expressions — too many to print, so the program says so instead and hints
at `-n`, `--force` or reverse mode.

### Reverse mode

Given a target number, omicron searches through the whole space of
expressions and reports every combination of operators that produces it.
Interval pruning discards prefixes that can no longer reach the target,
so even the full a..z space is searched in an instant:

```
$ ./build/omicron --reverse 24 -n 4
a + b + c × d = 24
a × b × c × d = 24
```

Use `--limit K` (default 1000) to cap the number of matches reported.

### Options

```
-n N           use letters a..(a+N-1), 1..26; default: a..z
--reverse R    list expressions that evaluate to R (left-to-right)
--limit K      max matches reported in reverse mode; default 1000
--force        allow printing more than one million expressions
```



## kofsum

A fast, non-cryptographic checksum in the spirit of `shasum`. Reads files or
stdin and prints a fixed-width 64-bit FNV-1a digest per input — good for
spotting changed content, not for standing up to adversaries:

```
$ ./build/kofsum README.md
$ printf 'hello' | ./build/kofsum
a430d84680aabd0b  -
```



## Build & run

```
make            # builds build/omicron and build/kofsum
make test       # runs self-checks
./build/omicron -n 4            # enumerate all a..d expressions
./build/omicron --reverse 24    # find a..z expressions equal to 24
./build/kofsum README.md        # checksum a file
```
