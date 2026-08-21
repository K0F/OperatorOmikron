# Welcome to Kof's abstract math crash course!

## Greekset

It uses 24 letters of Greek alphabet representing numbers from 1..24

where `A = 1, B = 2, Γ = 3 and Δ = 4`



## Special Operator = Omicron (better symbol yet to come)

Operator Omicron stands for variable in mathematical operator symbol.

Omicron can be either "+", "−", "×", "÷" ...etc. the variable is for the operation itself:

o = any operation:

```
aοbοcοd = ...
```

..please notice that, while having fun all numbers are still pretty, and so on



## What the program does

`omicron` takes the first four Greek numerals — `Α = 1`, `Β = 2`, `Γ = 3`, `Δ = 4` —
and substitutes every possible combination of Omicrons between them:

```
a ο b ο c ο d
```

Since each of the three gaps holds one of four operations (`+ − × ÷`),
there are exactly `4³ = 64` expressions, evaluated strictly left-to-right.
For example:

```
a + b + c + d = 10
a + b × c ÷ d = 2.25
a − b × c + d = 1
a ÷ b ÷ c ÷ d = 0.04166667
```

Run without arguments, the program prints all 64 of them.

### Reverse mode

Given a target number, omicron searches backwards through the whole space
of expressions and reports every combination of operators that produces it:

```
$ ./build/omicron --reverse 24
a + b + c × d = 24
a × b × c × d = 24
```

If no expression yields the target, it says so.



## Build & run

```
make            # builds build/omicron
make test       # runs self-checks
./build/omicron                 # enumerate all 64 expressions
./build/omicron --reverse 24    # find expressions equal to 24
```
