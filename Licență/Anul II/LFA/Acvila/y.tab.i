#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "limbaj.ypp"
#include <stdio.h>
#include <string>
#include <vector>
#include <stack>

#define RESET		0
#define BRIGHT 		1
#define DIM		2
#define UNDERLINE 	3
#define BLINK		4
#define REVERSE		7
#define HIDDEN		8

#define BLACK 		0
#define RED		1
#define GREEN		2
#define YELLOW		3
#define BLUE		4
#define MAGENTA		5
#define CYAN		6
#define	WHITE		7

using namespace std;
extern FILE* yyin;
extern FILE* yylogfile;
extern FILE* yyerrorfile;
extern FILE *yyout;
extern int yyparse(void);
extern char* yytext;
extern int _allOkFlag;
extern string _programDescription; 
extern string _tempBuff;
extern int yylineno;
extern int yyval;

/*
extern struct MyObject;
extern struct dataType;

extern vector<string> dataTypes;
extern stack<string> encounteredStrings;
*/

int yylex(void);
void yyerror(const char *);
void Log(char *);
void Log(char * prepend, int d);
void Log(char * prepend, char * s);
void textcolor(int attr, int fg, int bg);

#ifndef YYSTYPE
typedef int YYSTYPE;
#endif

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#define YYERROR_DECL() yyerror(const char *s)
#define YYERROR_CALL(msg) yyerror(msg)

extern int YYPARSE_DECL();

extern	int      yydebug;
extern	int      yynerrs;

extern	int      yyerrflag;
extern	int      yychar;
extern	YYSTYPE  yyval;
extern	YYSTYPE  yylval;
