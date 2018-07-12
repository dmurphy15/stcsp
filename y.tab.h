/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    STATEMENT = 258,
    RANGE = 259,
    LIST = 260,
    VAR = 261,
    OBJ = 262,
    ARR = 263,
    LE_CON = 264,
    GE_CON = 265,
    EQ_CON = 266,
    NE_CON = 267,
    IMPLY_CON = 268,
    UNTIL_CON = 269,
    LT_OP = 270,
    GT_OP = 271,
    LE_OP = 272,
    GE_OP = 273,
    EQ_OP = 274,
    NE_OP = 275,
    AND_OP = 276,
    OR_OP = 277,
    NOT_OP = 278,
    AT = 279,
    FIRST = 280,
    NEXT = 281,
    FBY = 282,
    IF = 283,
    THEN = 284,
    ELSE = 285,
    ABS = 286,
    IDENTIFIER = 287,
    CONSTANT = 288
  };
#endif
/* Tokens.  */
#define STATEMENT 258
#define RANGE 259
#define LIST 260
#define VAR 261
#define OBJ 262
#define ARR 263
#define LE_CON 264
#define GE_CON 265
#define EQ_CON 266
#define NE_CON 267
#define IMPLY_CON 268
#define UNTIL_CON 269
#define LT_OP 270
#define GT_OP 271
#define LE_OP 272
#define GE_OP 273
#define EQ_OP 274
#define NE_OP 275
#define AND_OP 276
#define OR_OP 277
#define NOT_OP 278
#define AT 279
#define FIRST 280
#define NEXT 281
#define FBY 282
#define IF 283
#define THEN 284
#define ELSE 285
#define ABS 286
#define IDENTIFIER 287
#define CONSTANT 288

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 60 "stcsp.y" /* yacc.c:1909  */

    char *str;
    int num;
    void *ptr;

#line 126 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
