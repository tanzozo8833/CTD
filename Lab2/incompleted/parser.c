/*
 * @copyright (c) 2008, Hedspi, Hanoi University of Technology
 * @author Huu-Duc Nguyen
 * @version 1.0
 */

#include <stdlib.h>

#include "reader.h"
#include "scanner.h"
#include "parser.h"
#include "error.h"

Token *currentToken;
Token *lookAhead;

void scan(void)
{
  Token *tmp = currentToken;
  currentToken = lookAhead;
  lookAhead = getValidToken();
  free(tmp);
}

void eat(TokenType tokenType)
{
  if (lookAhead->tokenType == tokenType)
  {
    printToken(lookAhead);
    scan();
  }
  else
    missingToken(tokenType, lookAhead->lineNo, lookAhead->colNo);
}

void compileProgram(void)
{
  assert("Parsing a Program ....");
  eat(KW_PROGRAM);
  eat(TK_IDENT);
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_PERIOD);
  assert("Program parsed!");
}

void compileBlock(void)
{
  assert("Parsing a Block ....");
  if (lookAhead->tokenType == KW_CONST)
  {
    eat(KW_CONST);
    compileConstDecl();
    compileConstDecls();
    compileBlock2();
  }
  else
    compileBlock2();
  assert("Block parsed!");
}

void compileBlock2(void)
{
  if (lookAhead->tokenType == KW_TYPE)
  {
    eat(KW_TYPE);
    compileTypeDecl();
    compileTypeDecls();
    compileBlock3();
  }
  else
    compileBlock3();
}

void compileBlock3(void)
{
  if (lookAhead->tokenType == KW_VAR)
  {
    eat(KW_VAR);
    compileVarDecl();
    compileVarDecls();
    compileBlock4();
  }
  else
    compileBlock4();
}

void compileBlock4(void)
{
  compileSubDecls();
  compileBlock5();
}

void compileBlock5(void)
{
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
}

void compileConstDecls(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileConstDecl();
    compileConstDecls();
  }
}

void compileConstDecl(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileConstant();
    eat(SB_SEMICOLON);
  }
  else
  {
    error(ERR_INVALIDCONSTDECL, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileTypeDecls(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileTypeDecl();
    compileTypeDecls();
  }
}

void compileTypeDecl(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    eat(SB_EQ);
    compileType();
    eat(SB_SEMICOLON);
  }
  else
  {
    error(ERR_INVALIDTYPEDECL, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileVarDecls(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    compileVarDecl();
    compileVarDecls();
  }
}

void compileVarDecl(void)
{
  // TODO
  if (lookAhead->tokenType == TK_IDENT)
  {
    eat(TK_IDENT);
    while (lookAhead->tokenType == SB_COMMA)
    {
      eat(SB_COMMA);
      eat(TK_IDENT);
    }
    eat(SB_COLON);
    compileType();
    eat(SB_SEMICOLON);
  }
  else
  {
    error(ERR_INVALIDVARDECL, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileSubDecls(void)
{
  assert("Parsing subtoutines ....");
  // TODO
  while ((lookAhead->tokenType == KW_FUNCTION) || (lookAhead->tokenType == KW_PROCEDURE))
  {
    if (lookAhead->tokenType == KW_FUNCTION)
      compileFuncDecl();
    else
      compileProcDecl();
  }
  assert("Subtoutines parsed ....");
}

void compileFuncDecl(void)
{
  assert("Parsing a function ....");
  // TODO
  eat(KW_FUNCTION);
  eat(TK_IDENT);
  compileParams();
  eat(SB_COLON);
  compileBasicType();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Function parsed ....");
  compileSubDecls();
}

void compileProcDecl(void)
{
  assert("Parsing a procedure ....");
  // TODO
  eat(KW_PROCEDURE);
  eat(TK_IDENT);
  compileParams();
  eat(SB_SEMICOLON);
  compileBlock();
  eat(SB_SEMICOLON);
  assert("Procedure parsed ....");
  compileSubDecls();
}

void compileUnsignedConstant(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  default:
    error(ERR_INVALIDCONSTANT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileConstant(void)
{
  // TODO
  if ((lookAhead->tokenType == SB_PLUS) || (lookAhead->tokenType == SB_MINUS))
    eat(lookAhead->tokenType);
  compileUnsignedConstant();
}

void compileConstant2(void)
{
  // TODO
  compileConstant();
}

void compileType(void)
{
  // TODO
  if (lookAhead->tokenType == KW_INTEGER || lookAhead->tokenType == KW_CHAR) {
    compileBasicType();
  }
  else if (lookAhead->tokenType == TK_IDENT) {
    // type = T;
    eat(TK_IDENT);
  }
  else if (lookAhead->tokenType == KW_ARRAY) {
    eat(KW_ARRAY);
    eat(SB_LSEL);
    eat(TK_NUMBER);

    // optional: parse ".."
    if (lookAhead->tokenType == SB_PERIOD) {
      eat(SB_PERIOD);
      eat(SB_PERIOD);
      eat(TK_NUMBER);
    }

    eat(SB_RSEL);
    eat(KW_OF);
    compileType();
  }
  else {
    error(ERR_INVALIDTYPE, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileBasicType(void)
{
  // TODO
  if (lookAhead->tokenType == KW_INTEGER)
    eat(KW_INTEGER);
  else if (lookAhead->tokenType == KW_CHAR)
    eat(KW_CHAR);
  else
    error(ERR_INVALIDBASICTYPE, lookAhead->lineNo, lookAhead->colNo);
}

void compileParams(void)
{
  // TODO
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    compileParam();
    while (lookAhead->tokenType == SB_SEMICOLON)
    {
      eat(SB_SEMICOLON);
      compileParam();
    }
    eat(SB_RPAR);
  }
}

void compileParams2(void)
{
  // TODO
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    eat(TK_IDENT);
  }
}

void compileParam(void)
{
  // TODO
  if (lookAhead->tokenType == KW_VAR)
    eat(KW_VAR);

  eat(TK_IDENT);
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    eat(TK_IDENT);
  }

  eat(SB_COLON);
  compileBasicType();
}

void compileStatements(void)
{
  // TODO
  if ((lookAhead->tokenType == TK_IDENT) || (lookAhead->tokenType == KW_CALL) ||
      (lookAhead->tokenType == KW_BEGIN) || (lookAhead->tokenType == KW_IF) ||
      (lookAhead->tokenType == KW_WHILE) || (lookAhead->tokenType == KW_FOR))
  {
    compileStatement();
    compileStatements2();
  }
}

void compileStatements2(void)
{
  // TODO
  while (1)
  {

    if ((lookAhead->tokenType == TK_IDENT) || (lookAhead->tokenType == KW_CALL) ||
        (lookAhead->tokenType == KW_BEGIN) || (lookAhead->tokenType == KW_IF) ||
        (lookAhead->tokenType == KW_WHILE) || (lookAhead->tokenType == KW_FOR) || lookAhead->tokenType == SB_SEMICOLON)
    {
      eat(SB_SEMICOLON);
    }
    else
      break; // gặp END hoặc ELSE => kết thúc statements

    // Kiểm tra END/ELSE để dừng
    if ((lookAhead->tokenType == KW_END) || (lookAhead->tokenType == KW_ELSE))
      break;

    // Parse statement tiếp theo
    compileStatement();
  }
}

void compileStatement(void)
{
  switch (lookAhead->tokenType)
  {
  case TK_IDENT:
    compileAssignSt();
    break;
  case KW_CALL:
    compileCallSt();
    break;
  case KW_BEGIN:
    compileGroupSt();
    break;
  case KW_IF:
    compileIfSt();
    break;
  case KW_WHILE:
    compileWhileSt();
    break;
  case KW_FOR:
    compileForSt();
    break;
    // EmptySt needs to check FOLLOW tokens
  case SB_SEMICOLON:
  case KW_END:
  case KW_ELSE:
    break;
    // Error occurs
  default:
    error(ERR_INVALIDSTATEMENT, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileAssignSt(void)
{
  assert("Parsing an assign statement ....");
  // TODO
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LSEL)
    compileIndexes();
  eat(SB_ASSIGN);
  compileExpression();
  assert("Assign statement parsed ....");
}

void compileCallSt(void)
{
  assert("Parsing a call statement ....");
  // TODO
  eat(KW_CALL);
  eat(TK_IDENT);
  if (lookAhead->tokenType == SB_LPAR)
  {
    eat(SB_LPAR);
    if (lookAhead->tokenType != SB_RPAR)
      compileArguments();
    eat(SB_RPAR);
  }

  assert("Call statement parsed ....");
}

void compileGroupSt(void)
{
  assert("Parsing a group statement ....");
  // TODO
  eat(KW_BEGIN);
  compileStatements();
  eat(KW_END);
  assert("Group statement parsed ....");
}

void compileIfSt(void)
{
  assert("Parsing an if statement ....");
  eat(KW_IF);
  compileCondition();
  eat(KW_THEN);
  compileStatement();
  if (lookAhead->tokenType == KW_ELSE)
    compileElseSt();
  assert("If statement parsed ....");
}

void compileElseSt(void)
{
  eat(KW_ELSE);
  compileStatement();
}

void compileWhileSt(void)
{
  assert("Parsing a while statement ....");
  // TODO
  eat(KW_WHILE);
  compileCondition();
  eat(KW_DO);
  compileStatement();
  assert("While statement parsed ....");
}

void compileForSt(void)
{
  assert("Parsing a for statement ....");
  // TODO
  eat(KW_FOR);
  eat(TK_IDENT);
  eat(SB_ASSIGN);
  compileExpression();
  eat(KW_TO);
  compileExpression();
  eat(KW_DO);
  compileStatement();
  assert("For statement parsed ....");
}

void compileArguments(void)
{
  // TODO
  compileExpression();
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
  }
}

void compileArguments2(void)
{
  // TODO
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
  }
}

void compileCondition(void)
{
  // TODO
  compileExpression();
  compileCondition2();
  compileExpression();
}

void compileCondition2(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case SB_EQ:
  case SB_NEQ:
  case SB_LT:
  case SB_LE:
  case SB_GT:
  case SB_GE:
    eat(lookAhead->tokenType);
    break;
  default:
    error(ERR_INVALIDCOMPARATOR, lookAhead->lineNo, lookAhead->colNo);
  }
}

void compileExpression(void)
{
  assert("Parsing an expression");
  // TODO
  if ((lookAhead->tokenType == SB_PLUS) || (lookAhead->tokenType == SB_MINUS))
    eat(lookAhead->tokenType);
  compileTerm();
  compileExpression2();
  assert("Expression parsed");
}

void compileExpression2(void)
{
  // TODO
  while ((lookAhead->tokenType == SB_PLUS) || (lookAhead->tokenType == SB_MINUS))
  {
    eat(lookAhead->tokenType);
    compileTerm();
  }
}

void compileExpression3(void)
{
  // TODO
  compileExpression();
}

void compileTerm(void)
{
  // TODO
  compileFactor();
  compileTerm2();
}

void compileTerm2(void)
{
  // TODO
  while ((lookAhead->tokenType == SB_TIMES) || (lookAhead->tokenType == SB_SLASH))
  {
    eat(lookAhead->tokenType);
    compileFactor();
  }
}

void compileFactor(void)
{
  // TODO
  switch (lookAhead->tokenType)
  {
  case TK_NUMBER:
    eat(TK_NUMBER);
    break;
  case TK_CHAR:
    eat(TK_CHAR);
    break;
  case TK_IDENT:
    eat(TK_IDENT);
    if (lookAhead->tokenType == SB_LPAR)
    {
      eat(SB_LPAR);
      if (lookAhead->tokenType != SB_RPAR)
        compileArguments();
      eat(SB_RPAR);
    }
    else if (lookAhead->tokenType == SB_LSEL)
    {
      compileIndexes();
    }
    break;
  case SB_LPAR:
    eat(SB_LPAR);
    compileExpression();
    eat(SB_RPAR);
    break;
  default:
    error(ERR_INVALIDFACTOR, lookAhead->lineNo, lookAhead->colNo);
    break;
  }
}

void compileIndexes(void)
{
  // TODO
  eat(SB_LSEL);
  compileExpression();
  while (lookAhead->tokenType == SB_COMMA)
  {
    eat(SB_COMMA);
    compileExpression();
  }
  eat(SB_RSEL);
}

int compile(char *fileName)
{
  if (openInputStream(fileName) == IO_ERROR)
    return IO_ERROR;

  currentToken = NULL;
  lookAhead = getValidToken();

  compileProgram();

  free(currentToken);
  free(lookAhead);
  closeInputStream();
  return IO_SUCCESS;
}
