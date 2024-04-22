package pl.edu.mimuw.expression.binary;

import pl.edu.mimuw.expression.Expression;

public abstract class BinaryOperator extends Expression {
    private final char operatorChar;
    protected Expression leftOperand;
    protected Expression rightOperand;

    public BinaryOperator(Expression leftOperand, Expression rightOperand, char operatorChar) {
        this.leftOperand = leftOperand;
        this.rightOperand = rightOperand;
        this.operatorChar = operatorChar;
    }

    @Override
    public String toString() {
        return STR."(\{leftOperand.toString()}\{operatorChar}\{rightOperand.toString()})";
    }
}
