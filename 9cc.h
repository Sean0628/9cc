#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
typedef struct Token Token;
typedef enum {
  ND_ADD,
  ND_SUB,
  ND_MUL,
  ND_DIV,
  ND_EQ,
  ND_NE,
  ND_LT,
  ND_LE,
  ND_GT,
  ND_GE,
  ND_NUM,
} NodeKind;
typedef enum {
  TK_RESERVED,
  TK_NUM,
  TK_EOF,
} TokenKind;

char *user_input;

void gen(Node *node);
void error(char *fmt, ...);
void expect(char *op);
bool consume(char *op);
int expect_number();

Token *tokenize();
Token *token;

Node *new_node(NodeKind kind);
Node *expr();
Node *equality();
Node *relational();
Node *add();
Node *mul();
Node *unary();
Node *primary();

struct Node {
  NodeKind kind;
  Node *lhs;
  Node *rhs;
  int val;
};

struct Token {
  TokenKind kind;
  Token *next;
  int val;
  char *str;
  int len;
};
