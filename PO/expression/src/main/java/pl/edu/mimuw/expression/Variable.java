package pl.edu.mimuw.expression;

public class Variable extends Expression {
    @Override
    public String toString() {
        return "x";
    }

    @Override
    public double evaluate(double x) {
        return x;
    }
}
