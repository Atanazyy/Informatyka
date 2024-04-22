package pl.edu.mimuw.expression.unary;

import pl.edu.mimuw.expression.Expression;

public abstract class UnaryOperator extends Expression {
    private final char operatorChar;
    protected Expression operand;

    public UnaryOperator(Expression operand, char operatorChar) {
        this.operand = operand;
        this.operatorChar = operatorChar;
    }

    @Override
    public String toString() {
        return STR."(\{operatorChar}\{operand.toString()})";
    }
}
