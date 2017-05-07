%{
#include	<string.h>	
#include	<stdio.h>	
#include	<stdlib.h>	

#include	"symtab.h"	
#include	"types.h"	
#include	"minic.tab.h"	
#include	"names.h"	

extern int	lineno;		
extern SYM_TAB	*scope;		

void
lex_init()
{
scope	= symtab_open(0);	
}

#define	YY_USER_INIT	lex_init();

%}

alpha			[A-Za-z]
digit			[0-9]
alphanum		[A-Za-z0-9]

%%
[ \t\r]			break;		/* ignore white space */
"\n" 			++lineno; 


int			return INT;	/* Keywords come before NAMEs */
if			return IF;
else			return ELSE;
return			return RETURN;
float			return FLOAT;
struct			return STRUCT;

{alpha}{alphanum}*	{
			yylval.name	= names_find_or_add(yytext);
			return NAME;
			}

{digit}+		{
			yylval.value	= atoi(yytext);
			return NUMBER;
			}

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

.			{
			fprintf(stderr,
				"Illegal character with code %d on line #%d\n",
				*yytext, lineno);
			exit(1);
			}

%%

int
yywrap()
{
return 1; /* tell scanner no further files need to be processed */
}

