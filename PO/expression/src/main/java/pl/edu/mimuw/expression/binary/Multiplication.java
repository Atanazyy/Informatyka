package pl.edu.mimuw.expression.binary;

import pl.edu.mimuw.expression.Expression;

public class Multiplication extends BinaryOperator {
    public Multiplication(Expression leftOperand, Expression rightOperand) {
        super(leftOperand, rightOperand, '*');
    }

    @Override
    public double evaluate(double x) {
        return leftOperand.evaluate(x) * rightOperand.evaluate(x);
    }
}
