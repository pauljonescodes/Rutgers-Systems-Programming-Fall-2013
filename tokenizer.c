#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Tokenizer struct asjkhdbaskjdbn */
typedef struct TokenizerT_ {
	int token_index; // This keeps track of the token that this object is on for the nextToken feature */
	char** tokens;   // filled in by TKCreate; a list of the tokens to be iterated through
	int num_tok;     // number of tokens
}TokenizerT;

/* Clone of the string.h function strpbrk */
char* brk(char* s1, char* s2) {
	int i, j;

	for(i=0;i<strlen(s1);i++) {
		for(j=0;j<strlen(s2);j++) {
			if(s1[i] == s2[j]) {
				return s1 + i;
			}
		}
	}
	return NULL;
}

/* replace in all escape characters */
char* escapeChar(char* s){ 
	int i, j = 0, val;
	char* ret = malloc(strlen(s) + 1);
	memset(ret, 0, strlen(s) + 1);

	for(i=0;i<strlen(s);i++) {
		if(s[i] != '\\') {
			ret[j] = s[i];
			j++;
			continue;
		}
		switch(s[i+1]) {
			case 'a':
				ret[j] = '\a';
				break;
			case 'b':
				ret[j] = '\b';
				break;
			case 'f':
				ret[j] = '\f';
				break;
			case 'n':
				ret[j] = '\n';
				break;
			case 'r':
				ret[j] = '\r';
				break;
			case 't':
				ret[j] = '\t';
				break;
			case 'v':
				ret[j] = '\v';
				break;
			case '\'':
				ret[j] = '\'';
				break;
			case '\"':
				ret[j] = '\"';
				break;
			case '\\':
				ret[j] = '\\';
				break;
			case '\?':
				ret[j] = '\?';
				break;
			case 'x': // Hexadecimal ascii character - assuming perfect formatting
				val = 0;
				if( s[i+2] >= 48 && s[i+2] <= 57) { // MSB character is less than A
					val += 16 * (s[i+2]-48);	
				}else{
					if(s[i+2] < 96) { // uppercase
						val += 16 * (s[i+2]-55);	
					}else{
						val += 16 * (s[i+2]-87);
					}
				}
				if( s[i+3] >= 48 && s[i+3] <= 57) { // MSB character is less than A
					val += (s[i+3]-48);	
				}else{
					if(s[i+3] < 96) { // uppercase
						val += (s[i+3]-55);	
					}else{
						val += (s[i+3]-87);
					}
				}
				ret[j] = val;
				i+=2;
				break;
			default:
				if(s[i+1] >= 48 && s[i] <= 57) { // octal ascii escape character... assuming perfect format here.
					ret[j] = (s[i+1]-48)*4 + (s[i+2]-48)*2 + (s[i+3]-48); i+=3;	
				}else{
					continue;
				}
		}
		j++; i++;
	}
	return ret;
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

TokenizerT *TKCreate(char *separators, char *ts) {
	int tok_index = 0;

	/* allocate space and initilize it to 0 before starting */
	TokenizerT *tk = malloc(sizeof(TokenizerT));
	memset(tk,0,sizeof(TokenizerT));
	
	/* allocate memory for the list of tokens */
	tk->tokens = malloc(strlen(ts)*sizeof(char*));
	memset(tk->tokens,0,strlen(ts)*sizeof(char*));

	ts = escapeChar(ts);
	separators = escapeChar(separators);	
	tk->token_index = 0;
	if(strlen(separators)==0) {
		separators = " ";
	}
	for(;;) {
		char* c = brk(ts,separators);
		if(c==NULL) {
			if(strlen(ts)==0) {
				break;
			}
			tk->tokens[tok_index] = malloc(strlen(ts));
			strcpy(tk->tokens[tok_index],ts);
			tok_index++;
			break;
		}
		if(c==ts) {
			ts++;
			continue;
		}
		tk->tokens[tok_index] = malloc((c-ts) + 1);
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
	if(tk->token_index==tk->num_tok) {
		return NULL;
	}
	char* ret = tk->tokens[tk->token_index];
	tk->token_index++;
  	return ret;
}

/*
 * main will have two string arguments (in argv[1] and argv[2]).
 * The first string contains the separator characters.
 * The second string contains the tokens.
 * Print out the tokens in the second string in left-to-right order.
 * Each token should be printed on a separate line.
 */

int main(int argc, char **argv) {
	/* do some error handling on the arguments */
	if(argc!=3) {
		fprintf(stderr,"ERROR: Invalid number of arguments.\nExpects format \n\n\t$ tokenizer <delims> <token string>\n");
		return 1;
	}

	/* loop through the tokens */
	TokenizerT* tk = TKCreate(argv[1],argv[2]);
	for(;;) {
		char* token = TKGetNextToken(tk);
		if(token == 0) {
			/* end of the stream */
			break;
		}
		/* got a token, print it out on it's own line */
		printf("%s\n",token); 
	}
	TKDestroy(tk);
	return 0;
}
