%{
#include <iostream>
#include <vector>
#include <cstring>
using namespace std;

const int START = 0;

%}

alpha		[A-Za-z]
digit		[0-9]
alphanum		[A-Za-z0-9]
name		{alpha}{alphanum}*
number		{digit}+

%%

int			return INT;	
if			return IF;
else		return ELSE;
return		return RETURN;
float		return FLOAT;
struct		return STRUCT;

{name}		return NAME;

"("			return LPAR;
")"			return RPAR;
"{"			return LBRACE;
"}"			return RBRACE;
"["			return LBRACK;
"]"			return RBRACK;
"=="		return EQUAL;
"="			return ASSIGN;
";"			return SEMICOLON;
","			return COMMA;
"."			return DOT;
"+"			return PLUS;
"-"			return MINUS;
"*"			return TIMES;
"/"			return DIVIDE;

%%


void main()
{
	char a[99999];
	cout << "please input some words separated by spaces"<< endl;
	cin.getline(a, 99999);

	const char * split = " ";
	char *strToken = nullptr;
	char *next_token = nullptr;
	strToken = strtok_s(a, split,&next_token);
	while (strToken != NULL) {
		cout << strToken << " : " << analysis(strlen(strToken), strToken) << endl;
		strToken = strtok_s(NULL, split, &next_token);
	}

	system("pause");
}