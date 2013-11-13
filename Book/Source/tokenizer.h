#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdlib.h>

/*
 * Tokenizer type.  You need to fill in the type as part of your implementation.
 */

/*typedef struct TokenizerT_ {
	char* copied_string;
	char* delimiters;
	char* current_position;
} TokenizerT;*/

/* Tokenizer struct */
typedef struct TokenizerT_ {
	int token_index; /* This keeps track of the token that this object is on for the nextToken feature */
	char** tokens;   /* filled in by TKCreate; a list of the tokens to be iterated through */
	int num_tok;     /* number of tokens */
}TokenizerT;

TokenizerT *TKCreate(char *ts, int, char*);
void TKDestroy(TokenizerT *tk);
char *TKGetNextToken(TokenizerT *tk);

#endif
