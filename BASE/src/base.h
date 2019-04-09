#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "fraction.h"
#include "util.h"

#ifndef BASE_H
#define BASE_H

int digit (char c) {
    if (c < '0' || c > 'Z' || (c < 'A' && c > '9')) return -1;
    if (c < 'A') return c - '0';
    return c - 'A' + 10;
}

char numeral (int n) {
    if (n > 9) return 'A' + n - 10;
    return n + '0';
}

Fraction fracParse (const char* str, int len, int base) {
    Fraction res = { 0, 1 }, rep = { 0, 0 };
    int dot = -1, brace = -1, norep = 0, i, d;

    if (len <= 0) return res;

    for (i = 0; i < len; i++) {
        if (dot == -1 && str[i] == '.') dot = i;
        if (brace == -1 && str[i] == '{') brace = i;
        if (dot != -1 && brace == -1 && digit(str[i]) != -1) norep++;
    }

    for (i = 0; i < len; i++) {
        d = digit(str[i]);
        if (d != -1) {
            res.num *= base;
            res.num += d;
        }

        if (str[i] == '{') {
            i++;
            break;
        }
    }

    for (; i < len; i++) {
        d = digit(str[i]);
        if (d != -1) {
            rep.num *= base;
            rep.num += d;
            rep.denom *= base;
            rep.denom += base-1;
        }
    }

    for (i = 0; i < norep; i++) {
        res.denom *= base;
        rep.denom *= base;
    }

    if (rep.denom != 0) res = fracAdd(res, rep);
    else fracSimplify(&res);

    return res;
}

int outputBase = 10;
int outputMode = 0;

typedef struct {
	char symbol;
	Fraction value;
} Token;

Token tokens [80];
int toki = 0;

int isOperator (char c) {
	return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}
int isSymbol (char c) {
	return isOperator(c) || c == '(' || c == ')';
}
int isNumeric (char c) {
	return digit(c) > -1 || c == '.';
}

int prevToken (int index) {
	do {
		index--;
	} while (index > -1 && index < toki && tokens[index].symbol == ' ');
	if (index >= toki || index < 0) return -1;

	return index;
}

int nextToken (int index) {
	do {
		index++;
	} while (index > -1 && index < toki && tokens[index].symbol == ' ');
	if (index >= toki || index < 0) return -1;

	return index;
}

int operands (int i, int* lhs, int* rhs) {
	while (1) {
		*lhs = prevToken(i);
		if (*lhs == -1) break;
		if (tokens[*lhs].symbol == 'n') break;
		tokens[*lhs].symbol = ' ';
	}
	while (1) {
		*rhs = nextToken(i);
		if (*rhs == -1) break;
		if (tokens[*rhs].symbol == 'n') break;
		tokens[*rhs].symbol = ' ';
	}
	if (*lhs == -1 || *rhs == -1) return -1;
	return 0;
}


void exprEval (int start, int len) {
	int i, op = 1, multiplier = 1;
	int paren = -1;
	int stack = 0;
    int lhs, rhs;

	if (len < 0) len = toki;

	// Parenthesis

	for (i = start; i < start+toki; i++) {
		if (tokens[i].symbol == '(') {
			if (paren == -1) paren = i;
			stack++;
		}
		if (tokens[i].symbol == ')') {
			if (paren != -1 && stack == 1) {
				tokens[i].symbol = ' ';
				tokens[paren].symbol = ' ';

				exprEval(paren, i - paren);
			}
			stack--;
		}
	}

	// Unary minus

	for (i = start; i < start + len; i++) {
		if (isOperator(tokens[i].symbol) && op > -1) {
			if (tokens[i].symbol == '-') {
				multiplier *= -1;
			}
			tokens[i].symbol = ' ';
		}
		if (tokens[i].symbol == '^') {
			op = i;
		}
		if (tokens[i].symbol == 'n') {
			op = -1;
			tokens[i].value.num *= multiplier;
			multiplier = 1;
		}
	}

	// Exponentiation
	for (i = start; i < start + len; i++) {
		if (tokens[i].symbol == '^') {
			tokens[i].symbol = ' ';

			if (operands(i, &lhs, &rhs) != -1) {
				tokens[lhs].value = fracPow(tokens[lhs].value, tokens[rhs].value);
				tokens[rhs].symbol = ' ';
			}
		}
	}

	// Unary minus
	for (i = start; i < start + len; i++) {
		if (isOperator(tokens[i].symbol) && op > -1) {
			if (tokens[i].symbol == '-') {
				multiplier *= -1;
			}
			tokens[i].symbol = ' ';
		}
		if (isOperator(tokens[i].symbol) && op == -1) {
			op = i;
		}
		if (tokens[i].symbol == 'n') {
			op = -1;
			tokens[i].value.num *= multiplier;
			multiplier = 1;
		}
	}

	// Multiplication and division
	for (i = start; i < start + len; i++) {
		if (tokens[i].symbol == '*') {
			tokens[i].symbol = ' ';

			if (operands(i, &lhs, &rhs) != -1) {
				tokens[lhs].value = fracMult(tokens[lhs].value, tokens[rhs].value);
				tokens[rhs].symbol = ' ';
			}
		}
		if (tokens[i].symbol == '/') {
			tokens[i].symbol = ' ';

			if (operands(i, &lhs, &rhs) != -1) {
				tokens[lhs].value = fracDiv(tokens[lhs].value, tokens[rhs].value);
				tokens[rhs].symbol = ' ';
			}
		}
	}


	// Addition and subtraction
	for (i = start; i < start + len; i++) {
		if (tokens[i].symbol == '+') {
			tokens[i].symbol = ' ';

			if (operands(i, &lhs, &rhs) != -1) {
				tokens[lhs].value = fracAdd(tokens[lhs].value, tokens[rhs].value);
				tokens[rhs].symbol = ' ';
			}
		}
		if (tokens[i].symbol == '-') {
			tokens[i].symbol = ' ';

			if (operands(i, &lhs, &rhs) != -1) {
				tokens[lhs].value = fracSubt(tokens[lhs].value, tokens[rhs].value);
				tokens[rhs].symbol = ' ';
			}
		}
	}

}

Fraction fracEval (char* str, int len) {
	int colon;
	int i;
	int numStart = -1, comma = -1;
	Fraction f = { 0, 1 };

	if (len < 0) len = strlen(str);
	colon = len;

	for (i = len-1; i > -1; i--) {
		if (str[i] == ':') {
			colon = i;
			break;
		}
	}

	for (i = colon+1; i < len; i++) {
		if (str[i] == '.') {
			str[i] = ' ';
			outputMode = 0;
		}
		if (str[i] == '/') {
			str[i] = ' ';
			outputMode = 1;
		}
	}

	if (colon != len) outputBase = fracParse(str + colon + 1, len-colon-1, 10).num;
	else outputBase = 10;
	if (outputBase == 0) outputBase = 10;

	for (i = 0; i < colon; i++) {
		if (isSymbol(str[i])) {
			if (numStart > -1) {
				tokens[toki].symbol = 'n';
				if (comma == -1) {
					tokens[toki].value = fracParse(str + numStart, i - numStart, 10);
				} else {
					tokens[toki].value = fracParse(str + numStart, comma - numStart, fracParse(str + comma, i - comma, 10).num);
				}
				toki++;
				comma = -1;
				numStart = -1;
			}
			tokens[toki++].symbol = str[i];
		}
		if (numStart == -1 && isNumeric(str[i])) {
			numStart = i;
		}
		if (numStart > -1 && str[i] == ',') {
			comma = i;
		}
	}

	if (numStart > -1) {
		tokens[toki].symbol = 'n';
		if (comma == -1) {
			tokens[toki].value = fracParse(str + numStart, i - numStart, 10);
		} else {
			tokens[toki].value = fracParse(str + numStart, comma - numStart, fracParse(str + comma, i - comma, 10).num);
		}
		toki++;
		comma = -1;
		numStart = -1;
	}

	exprEval(0, -1);

	for (i = 0; i < toki; i++) {
		if (tokens[i].symbol == 'n')
			return tokens[i].value;
	}
	return f;
}
char res[20];
int resi = 0;

void baseConvInt (int x, int base) {
    int start;
    int end;

    if (x == 0) {
            res[resi++] = '0';
            return;
    }

    if (x < 0) {
        res[resi++] = '-';
        x *= -1;
    }
    start = resi;

    while (x > 0) {
        res[resi++] = numeral(x % base);
        x /= base;
    }
    end = resi-1;

    while (start < end) {
        char c = res[start];
        res[start] = res[end];
        res[end] = c;

        start++;
        end--;
    }

    res[resi] = '\0';
}
void baseConvReal (double x, int base) {
    int dot = 0;
    int i, d;

    if (x < 0) {
        res[resi++] = '-';
        x *= -1;
    }
    res[resi++] = '0';

    while (x >= base) {
        dot++;
        x /= base;
    }

    while (resi < 14) {
        res[resi++] = numeral(floor(x));
        x -= floor(x);
        x *= base;
        if (dot == 0) res[resi++] = '.';
        dot--;
    }
    i = resi;
    if (x*2 >= base) {
        int roundup = 1;
        while (roundup) {
            do {
                i--;
            } while (res[i] == '.');

            d = digit(res[i]);
            if (d + 1 == base) {
                res[i] = '0';
            } else {
                res[i] = numeral(d+1);
                roundup = 0;
            }
        }
    }
    if (i != 0) {
        for (i = 0; i < resi; i++) {
            if (res[i] != '-') {
                res[i] = res[i+1];
            }
        }
        resi--;
        res[resi] = '\0';
    }
    for (i = resi - 1; i > -1; i--) {
        if (res[i] != '0') break;
        res[i] = '\0';
    }
    if (res[i] == '.') {
        res[i--] = '\0';
    }
    resi = i+1;
}

#endif
