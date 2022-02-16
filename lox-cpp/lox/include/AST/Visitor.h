#ifndef VISITOR_H
#define VISITOR_H

namespace lox
{
// expressions
class Binary;
class Grouping;
class Literal;
class Unary;

// statements
class ExpressionStatement;
class PrintStatement;

// abstract
class ExprVisitor
{
  public:
    virtual ~ExprVisitor() = default;

    virtual void visitBinaryExpr(const Binary &) = 0;
    virtual void visitGroupingExpr(const Grouping &) = 0;
    virtual void visitLiteralExpr(const Literal &) = 0;
    virtual void visitUnaryExpr(const Unary &) = 0;
};

// abstract
class StmtVisitor
{
  public:
    virtual ~StmtVisitor() = default;

    virtual void visitExpressionStmt(const ExpressionStatement &) = 0;
    virtual void visitPrintStmt(const PrintStatement &) = 0;
};

} // namespace lox

#endif
