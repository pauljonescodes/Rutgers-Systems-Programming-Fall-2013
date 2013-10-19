#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokenizer struct */
typedef struct TokenizerT_ {
	int token_index; /* This keeps track of the token that this object is on for the nextToken feature */
	char** tokens;   /* filled in by TKCreate; a list of the tokens to be iterated through */
	int num_tok;     /* number of tokens */
}TokenizerT;

int isAlphanumeric(char c) {
	if('A'<=c && 'Z'>=c) {
		return 1;
	}
	if('a'<=c && 'z'>=c) {
		return 1;
	}
	return 0;
}

/* Clone of the string.h function strpbrk */
char* brk(char* s1) {
	int i;
	
	for(i=0;i<strlen(s1);i++) {
		if(!isAlphanumeric(s1[i])) {
			return s1 + i;
		}
	}
	return NULL;
}
/*
 * TKCreate creates a new TokenizerT object for a given set of separator
 * characters (given as a string) and a token stream (given as a string).
 *
 * TKCreate should copy the two arguments so that it is not dependent on
 * them staying immutable after returning.  (In the future, this may change
 * to increase efficiency.)
 *
 * If the function succeeds, it returns a non-NULL TokenizerT.
 * Else it returns NULL.
 */

TokenizerT *TKCreate(char *ts) {
	int tok_index = 0;
	
	/* allocate space and initilize it to 0 before starting */
	TokenizerT *tk = malloc(sizeof(TokenizerT));
	memset(tk,0,sizeof(TokenizerT));
	
	/* allocate memory for the list of tokens */
	tk->tokens = malloc(strlen(ts)*sizeof(char*));
	memset(tk->tokens,0,strlen(ts)*sizeof(char*));

	tk->token_index = 0;
	for(;;) {
		char* c = brk(ts);
		if(c==NULL) {
			if(strlen(ts)==0) {
				break;
			}
			tk->tokens[tok_index] = malloc(strlen(ts));
			memset(tk->tokens[tok_index],0,strlen(ts));
			strcpy(tk->tokens[tok_index],ts);
			tok_index++;
			break;
		}
		if(c==ts) {
			ts++;
			continue;
		}
		tk->tokens[tok_index] = malloc((c-ts) + 1);
		memset(tk->tokens[tok_index],0,(c-ts) + 1);
		strncpy(tk->tokens[tok_index],ts,c-ts);
		tok_index++;
		ts = c;
	 	
	}
	tk->num_tok = tok_index;
  	return tk;
}

/*
 * TKDestroy destroys a TokenizerT object.  It should free all dynamically
 * allocated memory that is part of the object being destroyed.
 *
 * You need to fill in this function as part of your implementation.
 */

void TKDestroy(TokenizerT *tk) {
	int i;
	
	for(i=0;i<tk->num_tok;i++) {
		free(tk->tokens[i]);
	}
	free(tk->tokens);
	free(tk);
}

/*
 * TKGetNextToken returns the next token from the token stream as a
 * character string.  Space for the returned token should be dynamically
 * allocated.  The caller is responsible for freeing the space once it is
 * no longer needed.
 *
 * If the function succeeds, it returns a C string (delimited by '\0')
 * containing the token.  Else it returns 0.
 *
 * You need to fill in this function as part of your implementation.
 */

char *TKGetNextToken(TokenizerT *tk) {
	char* ret;
	
	if(tk->token_index == tk->num_tok) {
		return NULL;
	}
	
	ret = tk->tokens[tk->token_index];

	tk->token_index++;
  	return ret;
}

