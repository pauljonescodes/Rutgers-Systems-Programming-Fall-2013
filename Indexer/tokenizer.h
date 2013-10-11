#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

typedef struct TokenizerT_ {
	char* copied_string;
	char* delimiters;
	char* current_position;
} TokenizerT;

TokenizerT *TKCreate(char *ts);
void TKDestroy(TokenizerT *tk);
char *TKGetNextToken(TokenizerT *tk);

#endif
