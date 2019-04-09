#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>
#include <fileioc.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "base.h"

void printText(const char *text, uint8_t x, uint8_t y);
void printTextSmall(const char *text, uint8_t xpos, uint8_t ypos);
void replaceChars (char *entry);

void main(void) {
    char input[160];
    int i;
    Fraction frac;
    real_t a, b;

    //char str[10];

    /*os_ClrHome();

	printText("\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f", 0, 0);
	printText("\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f", 0, 1);
	printText("\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f", 0, 2);
	printText("\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f", 0, 3);
	printText("\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f", 0, 4);
	printText("\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f", 0, 5);
	printText("\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f", 0, 6);
	printText("\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f", 0, 7);

    while (!os_GetCSC());

	os_ClrHome();

	printText("\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f", 0, 0);
	printText("\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f", 0, 1);
	printText("\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAa\xAb\xAc\xAd\xAe\xAf", 0, 2);
	printText("\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBa\xBb\xBc\xBd\xBe\xBf", 0, 3);
	printText("\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCa\xCb\xCc\xCd\xCe\xCf", 0, 4);
	printText("\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDa\xDb\xDc\xDd\xDe\xDf", 0, 5);
	printText("\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEa\xEb\xEc\xEd\xEe\xEf", 0, 6);
	printText("\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFa\xFb\xFc\xFd\xFe\xFf", 0, 7);

    while (!os_GetCSC());*/


    os_ClrHome();
    os_GetStringInput("Expr: ", input, sizeof input);

    replaceChars(input);

    frac = fracEval(input, -1);

    if (outputMode == 0) { // decimal
        baseConvReal(frac.num * 1.0 / frac.denom, outputBase);
    } else { // fraction
        baseConvInt(frac.num, outputBase);
        res[resi++] = '/';
        baseConvInt(frac.denom, outputBase);
    }
    printText(res, 0, 9);

    //printText(input, 0, 1);
    //sprintf(str, "%d", (unsigned)input[0]);
    //printText(str, 0, 2);

    a = os_Int24ToReal(frac.num);
    b = os_Int24ToReal(frac.denom);

    ti_SetVar(TI_REAL_TYPE, ti_A, &a);
    ti_SetVar(TI_REAL_TYPE, ti_B, &b);

    while (!os_GetCSC());
}

/* Draw text on the homescreen at the given X/Y location */
void printText(const char *text, uint8_t xpos, uint8_t ypos) {
    os_SetCursorPos(ypos, xpos);
    os_PutStrFull(text);
}

/* Draw small text at the given X/Y location */
void printTextSmall(const char *text, uint8_t xpos, uint8_t ypos) {
    os_FontDrawText(text, xpos, ypos);
}
void replaceChars (char *entry) {
    const char orig [] = "pq\x82\x83\xf0\x10\x11\x08\x09:\xb0>+";
    const char repl [] = "+-*" "/" "^" "(" ")" "{" "}" ".-" ":,";
                    //    +-*/^(){}.-:,

    int i, j, len = strlen(entry);
    for (i = 0; i < len; i++) {
        for (j = 0; j < sizeof orig; j++) {
            if (entry[i] == orig[j]) {
                entry[i] = repl[j];
                break;
            }
        }
    }
}
