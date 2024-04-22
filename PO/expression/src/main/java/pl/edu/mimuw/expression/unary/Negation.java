package pl.edu.mimuw.expression.unary;

import pl.edu.mimuw.expression.Expression;

public class Negation extends UnaryOperator {
    public Negation(Expression operand) {
        super(operand, '-');
    }

    @Override
    public double evaluate(double x) {
        return -operand.evaluate(x);
    }
}
