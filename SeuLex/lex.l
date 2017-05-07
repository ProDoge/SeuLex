%{
/*	lex.l(1.6)	17:46:22	97/12/10
*
*	Lexical analyzer for the toy language ``minic''
*/
#include	<string.h>	/* for strcmp, strdup & friends */
#include	<stdio.h>	/* for strcmp, strdup & friends */
#include	<stdlib.h>	/* for atoi() */

#include	"symtab.h"	/* symbol table management */
#include	"types.h"	/* symbol table management */
#include	"minic.tab.h"	/* token type definitions from .y file */
#include	"names.h"	/* string pool management */

extern int	lineno;		/* defined in minic.y */
extern SYM_TAB	*scope;		/* defined in minic.y */

void
lex_init()
/*	Initialize data structures etc. for scanner */
{
scope	= symtab_open(0);	/* open topmost scope */
}

/*
*	The macro below will be called automatically when the generated scanner
*	initializes itself.
*/
#define	YY_USER_INIT	lex_init();

%}

alpha			[A-Za-z]
digit			[0-9]
alphanum		[A-Za-z0-9]
integer             {digit}+
name                {alpha}{alphanum}*

%%


int			return INT;
if			return IF;
else			return ELSE;
return			return RETURN;
float			return FLOAT;
struct			return STRUCT;

name			return NAME;

		

integer		return NUMBER;

"("			return LPAR;
")"			return RPAR;
"{"			return LBRACE;
"}"			return RBRACE;
"["			return LBRACK;
"]"			return RBRACK;
"=="			return EQUAL;
"="			return ASSIGN;
";"			return SEMICOLON;
","			return COMMA;
"."			return DOT;
"+"			return PLUS;
"-"			return MINUS;
"*"			return TIMES;
"/"			return DIVIDE;


%%

int main()
{
	string s;
        while(true){
            cin>>s;
            cout<<analysis(s)<<endl;
        }
		system("pause");
	return 0;
}

