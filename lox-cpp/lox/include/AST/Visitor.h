#ifndef VISITOR_H
#define VISITOR_H

namespace lox
{
// expressions
class BinaryExpression;
class GroupingExpression;
class LiteralExpression;
class UnaryExpression;

// statements
class ExpressionStatement;
class PrintStatement;

// abstract
class ExprVisitor
{
  public:
    virtual ~ExprVisitor() = default;

    virtual void visitBinaryExpr(const BinaryExpression &) = 0;
    virtual void visitGroupingExpr(const GroupingExpression &) = 0;
    virtual void visitLiteralExpr(const LiteralExpression &) = 0;
    virtual void visitUnaryExpr(const UnaryExpression &) = 0;
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
