package pl.edu.mimuw.expression.function;

import pl.edu.mimuw.expression.Expression;

public abstract class OneArgumentFunction extends Expression {
    private final String functionName;
    protected Expression argument;

    public OneArgumentFunction(String functionName, Expression argument) {
        this.functionName = functionName;
        this.argument = argument;
    }

    @Override
    public String toString() {
        return STR."\{functionName}(\{argument})";
    }
}
