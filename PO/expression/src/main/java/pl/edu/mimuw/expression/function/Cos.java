package pl.edu.mimuw.expression.function;

import pl.edu.mimuw.expression.Expression;

public class Cos extends OneArgumentFunction {
    public Cos(Expression argument) {
        super("cos", argument);
    }

    @Override
    public double evaluate(double x) {
        return Math.cos(argument.evaluate(x));
    }
}
