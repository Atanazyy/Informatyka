package pl.edu.mimuw.expression;

public class Constant extends Expression {
    double value;

    public Constant(double value) {
        this.value = value;
    }

    @Override
    public String toString() {
        return String.valueOf(value);
    }

    @Override
    public double evaluate(double x) {
        return value;
    }
}
