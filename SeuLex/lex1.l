%{#include <iostream>
#include <string>
#include <stdio.h>
#define  ID		    1
#define  NUM		2
#define  SUB		3
#define  ADD_MUL	4
#define  INT		5
%}
digit	[0-9]
letter	[a-zA-Z]
id	{letter}[{letter}|{digit}]*
num	{digit}{digit}*
%%
num         {return NUM;}
id          {return ID;}
int         {return INT;}
-           {return SUB;}
+=          {return ADD_MUL;}
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