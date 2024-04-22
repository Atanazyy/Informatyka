package pl.edu.mimuw.expression.binary;

import pl.edu.mimuw.expression.Expression;

public class Addition extends BinaryOperator {
    public Addition(Expression leftOperand, Expression rightOperand) {
        super(leftOperand, rightOperand, '+');
    }

    public double evaluate(double x) {
        return leftOperand.evaluate(x) + rightOperand.evaluate(x);
    }
}
