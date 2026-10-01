# evalexpr

simple command-line math expression evaluator.

- supports `+` `-` `*` `/` `%`
- unary minus (`-5`, `-(3+2)`)
- operator precedence (`*` `/` `%` before `+` `-`)
- error handling for division/modulo by zero and syntax errors

build:

```bash
  # build
	make
```

run:

```bash
	# run
	./evalexpr
```

example expressions:

```bash
	>> 2 + 3 * 4
	14
	>> (10 - 3) * 2
	14
	>> -5 + 8
	3
	>> 10 / 0
	ZeroDivisionError: division by zero
```
press ```(CTRL+D)``` to exit

**note**:
    - only integers are supported
    - no parentheses support yet
    - spaces are ignored
