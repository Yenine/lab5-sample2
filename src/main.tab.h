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

#ifndef YY_YY_SRC_MAIN_TAB_H_INCLUDED
# define YY_YY_SRC_MAIN_TAB_H_INCLUDED
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
    T_CHAR = 258,
    T_INT = 259,
    T_STRING = 260,
    T_BOOL = 261,
    LOP_ASSIGN = 262,
    LOP_ADD_ASSIGN = 263,
    LOP_MINUS_ASSIGN = 264,
    LOP_MUL_ASSIGN = 265,
    LOP_DIV_ASSIGN = 266,
    O_ADD = 267,
    O_MINUS = 268,
    O_MUL = 269,
    O_DIV = 270,
    O_EQ = 271,
    O_UEQ = 272,
    O_GRE = 273,
    O_LES = 274,
    O_GREEQ = 275,
    O_LESEQ = 276,
    O_AND = 277,
    O_OR = 278,
    O_NOT = 279,
    O_MOD = 280,
    O_DOUBLE_ADD = 281,
    O_DOUBLE_MINUS = 282,
    O_ADDRESS = 283,
    SEMICOLON = 284,
    COMMA = 285,
    WHILE = 286,
    IF = 287,
    ELSE = 288,
    FOR = 289,
    RET = 290,
    BRK = 291,
    CONST = 292,
    LP = 293,
    RP = 294,
    LC = 295,
    RC = 296,
    IDENTIFIER = 297,
    INTEGER = 298,
    CHAR = 299,
    BOOL = 300,
    STRING = 301,
    LOP_EQ = 302,
    PRE_ADD = 303,
    PRE_MINUS = 304,
    UMINUS = 305,
    AFT_ADD = 306,
    AFT_MINUS = 307
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SRC_MAIN_TAB_H_INCLUDED  */
