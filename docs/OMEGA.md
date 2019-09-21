# OMEGA

## Base

Digits are `0123456789ABCDE...`.

Normal infix operators are `+-*/^`. Implicit multiplication is allowed.  Parentheses are usable.

Use `{}` to specify repeating decimal. For example `5.3{45}` signifies `5.34545454545...`.

Use `,` to specify base. For example `A3,16` signifies `A3` in base 16. 

Use `:` to end expression and begin output format.

### Output Format 

Specify output base.

`.` for decimal output.

`/` for fractional output.

Default is decimal base 10 output.

## RATIONAL

Varibles are `STUVWXYZ`.

Normal operators are `+-*/^`. Implicit multiplication is allowed. Parentheses are usable.

`Q` is polynomial quotient, `R` is polynomial remainder, both infix and same precedence as multiplication.

`p I q`, where `p = aX + b`, integrates the polynomial `q` with respect to `X` such that the result, `r`, satsfies `r(a) = b`. `D` takes the derivative with respect to `X`. Both have the same precedence, evaluated before exponentiation.

`p C q`, for polynomial `p` and `q`, returns a result the corresponding coefficiant multiplication. It is, by default, evaluated last.

Brace enclosed lists denote shorthand polynomial literals of one variable. For example, `{Y3,3,-2,4} = 3Y^3+3Y^2-2Y+4`. By default, the variable is `X`.

Use `:` to end expression and begin output format.

### Output Format 

`/` for quotient-remainder form.

`*` for factored from.

`.` for decimals.

`0` to output zeros (polynomials in `X` only).

`0` for list form (polynomials in `X` only).

Default is expression evaluable by TI-84 using fractions instead of decimals.
