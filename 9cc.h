#define _GNU_SOURCE
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// tokenize.c
//

// Token
typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_NUM,
  TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
bool consume(char *op);
Token *consume_ident(void);
void expect(char *op);
int expect_number();
bool at_eof();
Token *tokenize();

char *user_input;
Token *token;

//
// parse.c
//

// Local variable
typedef struct Var Var;
struct Var {
  Var *next;
  char *name;
  int offset;
};

typedef enum {
  ND_ADD,       // +
  ND_SUB,       // -
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_GT,        // >
  ND_GE,        // >=
  ND_ASSIGN,    // =
  ND_VAR,       // Variable
  ND_RETURN,    // "return"
  ND_IF,        // "if"
  ND_WHILE,     // "while"
  ND_FOR,       // "for"
  ND_BLOCK,     // { ... }
  ND_FCALL,     // Function call
  ND_EXPR_STMT, // Expression statement
  ND_NUM,       // Integer
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *next;
  Node *lhs;
  Node *rhs;

  // "if"/"while"/"for" statement
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  // Block
  Node *body;

  // Function call
  char *funcname;

  Var *var;
  int val;
};

typedef struct Function Function;
struct Function {
  Node *node;
  Var *locals;
  int stack_size;
};

Function *program();

//
// codegen.c
//

void codegen(Function *prog);
