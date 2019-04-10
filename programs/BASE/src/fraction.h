#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#ifndef FRACTION_H
#define FRACTION_H

typedef struct {
    int num, denom;
} Fraction;

void fracSimplify (Fraction* frac) {
    int a = frac->num, b = frac->denom, tmp;

    while (b != 0) {
        tmp = b;
        b = a % b;
        a = tmp;
    }

    frac->num /= a;
    frac->denom /= a;

    if (frac->denom < 0) {
        frac->num *= -1;
        frac->denom *= -1;
    }
}

Fraction fracAdd (Fraction lhs, Fraction rhs) {
    Fraction res;
    res.num = lhs.num * rhs.denom + rhs.num * lhs.denom;
    res.denom = lhs.denom * rhs.denom;

    fracSimplify (&res);
    return res;
}
Fraction fracSubt (Fraction lhs, Fraction rhs) {
    rhs.num *= -1;

    return fracAdd(lhs, rhs);
}
Fraction fracMult (Fraction lhs, Fraction rhs) {
    Fraction res;
    res.num = lhs.num * rhs.num;
    res.denom = lhs.denom * rhs.denom;
    fracSimplify (&res);
    return res;
}
Fraction fracDiv (Fraction lhs, Fraction rhs) {
    Fraction res;
    res.num = lhs.num * rhs.denom;
    res.denom = lhs.denom * rhs.num;

    fracSimplify (&res);
    return res;

}
Fraction fracPow (Fraction lhs, Fraction rhs) {
    int t;
    Fraction res;

    if (rhs.num < 0) {
    	rhs.num *= -1;
    	t = lhs.num;
    	lhs.num = lhs.denom;
    	lhs.denom = t;
    }
    res.num = round(pow(lhs.num, 1.0*rhs.num/rhs.denom));
    res.denom = round(pow(lhs.denom, 1.0*rhs.num/rhs.denom));

    fracSimplify (&res);
    return res;
}



#endif FRACTION_H