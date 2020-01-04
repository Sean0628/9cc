#define _GNU_SOURCE
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Type Type;
typedef struct Member Member;

//
// tokenize.c
//

// Token
typedef enum {
  TK_RESERVED,
  TK_IDENT,
  TK_STR,
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

  char *contents;
  char cont_len;
};

void error(char *fmt, ...);
void error_at(char *loc, char *fmt, ...);
void error_tok(Token *tok, char *fmt, ...);
Token *peek(char *s);
Token *consume(char *op);
Token *consume_ident(void);
void expect(char *s);
int expect_number();
char *expect_ident(void);
bool at_eof();
Token *tokenize();

extern char *filename;
extern char *user_input;
Token *token;

//
// parse.c
//

// Variable
typedef struct Var Var;
struct Var {
  char *name;
  Type *ty;
  bool is_local;

  // Local variable
  int offset;

  // Global variable
  char *contents;
  int cont_len;
};

typedef struct VarList VarList;
struct VarList {
  VarList *next;
  Var *var;
};

typedef enum {
  ND_ADD,       // +
  ND_PTR_ADD,
  ND_SUB,       // -
  ND_PTR_SUB,
  ND_PTR_DIFF,
  ND_MUL,       // *
  ND_DIV,       // /
  ND_EQ,        // ==
  ND_NE,        // !=
  ND_LT,        // <
  ND_LE,        // <=
  ND_GT,        // >
  ND_GE,        // >=
  ND_ASSIGN,    // =
  ND_MEMBER,    // . (struct member access)
  ND_ADDR,      // unary &
  ND_DEREF,     // unary *
  ND_VAR,       // Variable
  ND_RETURN,    // "return"
  ND_IF,        // "if"
  ND_WHILE,     // "while"
  ND_FOR,       // "for"
  ND_BLOCK,     // { ... }
  ND_FCALL,     // Function call
  ND_EXPR_STMT, // Expression statement
  ND_STMT_EXPR, // Statement expression
  ND_NUM,       // Integer
  ND_NULL,      // null
} NodeKind;

// AST node type
typedef struct Node Node;
struct Node {
  NodeKind kind;
  Node *next;
  Type *ty;
  Token *tok;
  Node *lhs;
  Node *rhs;

  // "if"/"while"/"for" statement
  Node *cond;
  Node *then;
  Node *els;
  Node *init;
  Node *inc;

  // Block / statement expression
  Node *body;

  // Struct member access
  Member *member;

  // Function call
  char *funcname;
  Node *args;

  Var *var;
  int val;
};

typedef struct Function Function;
struct Function {
  Function *next;
  char *name;
  VarList *params;
  Node *node;
  VarList *locals;
  int stack_size;
};

typedef struct {
  VarList *globals;
  Function *fns;
} Program;

Program *program(void);

//
// type.c
//

typedef enum {
  TY_CHAR,
  TY_INT,
  TY_PTR,
  TY_ARRAY,
  TY_STRUCT,
} TypeKind;

struct Type {
  TypeKind kind;
  int size;        // sizeof() value
  Type *base;      // pointer / array
  int array_len;   // array
  Member *members; // struct
};

struct Member {
  Member *next;
  Type *ty;
  char *name;
  int offset;
};

extern Type *char_type;
extern Type *int_type;

bool is_integer(Type *ty);
Type *pointer_to(Type *base);
Type *array_of(Type *base, int size);
void add_type(Node *node);

//
// codegen.c
//

void codegen(Program *prog);
