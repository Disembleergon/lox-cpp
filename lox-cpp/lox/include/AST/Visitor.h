#ifndef VISITOR_H
#define VISITOR_H

namespace lox
{
// expressions
class AssignExpression;
class BinaryExpression;
class GroupingExpression;
class LiteralExpression;
class UnaryExpression;
class VarExpression;

// statements
class ExpressionStatement;
class VarStatement;
class PrintStatement;

// abstract
class ExprVisitor
{
  public:
    virtual ~ExprVisitor() = default;

    virtual void visitAssignExpr(const AssignExpression &) = 0;
    virtual void visitBinaryExpr(const BinaryExpression &) = 0;
    virtual void visitGroupingExpr(const GroupingExpression &) = 0;
    virtual void visitLiteralExpr(const LiteralExpression &) = 0;
    virtual void visitUnaryExpr(const UnaryExpression &) = 0;
    virtual void visitVarExpr(const VarExpression &) = 0;
};

// abstract
class StmtVisitor
{
  public:
    virtual ~StmtVisitor() = default;

    virtual void visitExpressionStmt(const ExpressionStatement &) = 0;
    virtual void visitVarStmt(const VarStatement &) = 0;
    virtual void visitPrintStmt(const PrintStatement &) = 0;
};

} // namespace lox

#endif
