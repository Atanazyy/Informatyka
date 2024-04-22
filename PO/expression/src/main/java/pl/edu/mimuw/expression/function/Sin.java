package pl.edu.mimuw.expression.function;

import pl.edu.mimuw.expression.Expression;

public class Sin extends OneArgumentFunction {
    public Sin(Expression argument) {
        super("sin", argument);
    }

    @Override
    public double evaluate(double x) {
        return Math.sin(argument.evaluate(x));
    }
}
