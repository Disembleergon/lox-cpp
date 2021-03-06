#ifndef VISITOR_H
#define VISITOR_H

namespace lox
{
// expressions
class AssignExpression;
class BinaryExpression;
class CallExpression;
class GroupingExpression;
class LiteralExpression;
class LogicalExpression;
class UnaryExpression;
class VarExpression;

// statements
class IfStatement;
class BlockStatement;
class ExpressionStatement;
class FunctionStatement;
class VarStatement;
class PrintStatement;
class ReturnStatement;
class WhileStatement;
class BreakStatement;

// abstract
class ExprVisitor
{
  public:
    virtual ~ExprVisitor() = default;

    virtual void visitAssignExpr(const AssignExpression &) = 0;
    virtual void visitBinaryExpr(const BinaryExpression &) = 0;
    virtual void visitCallExpr(const CallExpression &) = 0;
    virtual void visitGroupingExpr(const GroupingExpression &) = 0;
    virtual void visitLiteralExpr(const LiteralExpression &) = 0;
    virtual void visitLogicalExpr(const LogicalExpression &) = 0;
    virtual void visitUnaryExpr(const UnaryExpression &) = 0;
    virtual void visitVarExpr(const VarExpression &) = 0;
};

// abstract
class StmtVisitor
{
  public:
    virtual ~StmtVisitor() = default;

    virtual void visitIfStmt(const IfStatement &) = 0;
    virtual void visitBlockStmt(const BlockStatement &) = 0;
    virtual void visitExpressionStmt(const ExpressionStatement &) = 0;
    virtual void visitFunctionStatement(const FunctionStatement &) = 0;
    virtual void visitVarStmt(const VarStatement &) = 0;
    virtual void visitPrintStmt(const PrintStatement &) = 0;
    virtual void visitReturnStmt(const ReturnStatement &) = 0;
    virtual void visitWhileStmt(const WhileStatement &) = 0;
    virtual void visitBreakStmt(const BreakStatement &) = 0;
};

} // namespace lox

#endif
