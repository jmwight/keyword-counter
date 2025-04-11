#include "getch.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define MAXWORD	100

/* struct with some of the keyword types, not all and an int to store count */
struct key
{
	char *word;
	int count;
} keytab[] = {
	"alignas", 0,
	"alignof", 0,
	"auto", 0,
	"bool", 0,
	"break", 0,
	"case", 0,
	"char", 0,
	"const", 0,
	"constexpr", 0,
	"continue", 0,
	"default", 0,
	"do", 0,
	"double", 0,
	"else", 0,
	"enum", 0,
	"extern", 0,
	"false", 0,
	"float", 0,
	"for", 0,
	"goto", 0,
	"if", 0,
	"inline", 0,
	"int", 0,
	"long", 0,
	"nullptr", 0,
	"register", 0,
	"restrict", 0,
	"return", 0,
	"short", 0,
	"signed", 0,
	"sizeof", 0,
	"static", 0,
	"static_assert", 0,
	"struct", 0,
	"switch", 0,
	"thread_local", 0,
	"true", 0,
	"typedef", 0,
	"typeof", 0,
	"typeof_unqual", 0,
	"union", 0,
	"unsigned", 0,
	"void", 0,
	"volatile", 0,
	"while", 0,
	"_Alignas", 0,
	"_Alignof", 0,
	"_Atomic", 0,
	"_BitInt", 0,
	"_Bool", 0,
	"_Complex", 0,
	"_Decimal128", 0,
	"_Decimal32", 0,
	"_Decimal64", 0,
	"_Generic", 0,
	"Imaginary", 0,
	"_Noreturn", 0,
	"_Static_assert", 0,
	"_Thread_local", 0
};

#define NKEYS (sizeof keytab / sizeof(struct key))

int getword(char *, int);
int binsearch(char *, struct key *, int);

int main(void)
{
	int n;
	char word[MAXWORD];

	while(getword(word, MAXWORD) != EOF)
		if(isalpha(word[0]))
			if((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	for(n = 0; n < NKEYS; n++)
		printf("%4d %s\n", keytab[n].count, keytab[n].word);

	return 0;
}

/* binsearch: find word in tab[0]...tab[n-1] */
int binsearch(char *word, struct key tab[], int n)
{
	int cond;
	int low, high, mid;

	low = 0;
	high = n - 1;
	while(low <= high)
	{
		mid = (low + high) / 2;
		if((cond = strcmp(word, tab[mid].word)) < 0)
			high = mid - 1;
		else if(cond > 0)
			low = mid + 1;
		else
			return mid;
	}

	return -1;
}

/* getword: get next word or character from input */
int getword(char *word, int lim)
{
	int c; /* getch(void); 
	void ungetch(int); */   //try this later & remove getch.h header for learning
	char *w = word;

	while(isspace(c = getch()))
		;
	if(c != EOF)
		*w++ = c;
	if(!isalpha(c))
	{
		*w = '\0';
		return c;
	}
	for( ; --lim > 0; w++)
		if(!isalnum(*w = getch()))
		{
			ungetch(*w);
			break;
		}
	*w = '\0';
	return word[0];
}
