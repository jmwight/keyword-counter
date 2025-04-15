//#include "getch.h"
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
	"#define", 0,
	"#endif", 0,
	"#error", 0,
	"#if", 0,
	"#ifdef", 0,
	"#ifndef", 0,
	"#include", 0,
	"#undef", 0,
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

struct attr
{
	int comm, str, c;
};

struct attr getword(char *, int);
int binsearch(char *, struct key *, int);

int main(void)
{
	int n;
	char word[MAXWORD];
	struct attr wattr;

	while((wattr = getword(word, MAXWORD)).c != EOF)
		if((isalpha(word[0]) || word[0] == '_' || word[0] == '#') && !wattr.str && !wattr.comm)
			if((n = binsearch(word, keytab, NKEYS)) >= 0)
				keytab[n].count++;
	printf("\nKeywords Found:\n");
	for(n = 0; n < NKEYS; n++)
		if(keytab[n].count > 0)
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
struct attr getword(char *word, int lim)
{
	int c, nxt_c, firstch; 
	static int onelncom, multilncom;
	static struct attr wattr;

	int getch(void);
	void ungetch(int);   //try this later & remove getch.h header for learning	

	char *w = word;
	onelncom = multilncom = 0;

	while(isspace(c = getch()))
		if(c == '\n')
			onelncom = 0;

	for(firstch = 1; --lim > 0; c = getch())
	{
		switch(c) 
		{
			case '"':
				/* flip quote on or off */
				if(!wattr.str)
					wattr.str = 1;
				else
					wattr.str = 0;
				break;
			case '/':
				if((nxt_c = getch()) == '*')
					multilncom = 1;
				else if(nxt_c == '/')
					onelncom = 1;
				else
					ungetch(nxt_c);
				if(multilncom || onelncom)
					wattr.comm = 1;
				break;
			case '*':
				if((nxt_c = getch()) == '/')
				{
					multilncom = 0;
					wattr.comm = 0;
				}
				else
					ungetch(nxt_c);
				break;
			case '#':
				/* if at beginning of word */
				if(w = word)
					*w++ = c;
				else
					wattr.c = *word;
				break;
			case EOF:
				*w = '\0';
				wattr.c = c;
				return wattr;
			case '_':
				*w++ = c;
				if(firstch)
					firstch = 0;
				break;
			default:
				*w = c;
				if(firstch && !isalpha(*w) || !firstch && !isalnum(*w))
				{
					*w = '\0';
					wattr.c = *word;
					return wattr;
				}
				if(firstch)
					firstch = 0;
				w++;
				break;
		}

	}
}
