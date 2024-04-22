package pl.edu.mimuw;

import pl.edu.mimuw.expression.Constant;
import pl.edu.mimuw.expression.Expression;
import pl.edu.mimuw.expression.Variable;
import pl.edu.mimuw.expression.binary.Addition;
import pl.edu.mimuw.expression.binary.Multiplication;
import pl.edu.mimuw.expression.function.Cos;
import pl.edu.mimuw.expression.function.Sin;
import pl.edu.mimuw.expression.unary.Negation;

public class Main {
    public static void main(String[] args) {
        Expression x = new Variable();
        Expression sum = new Addition(x, new Constant(420.69));
        Expression sin = new Sin(sum);
        Expression negation = new Negation(sin);
        Expression exp = new Multiplication(new Constant(21.37), negation);
        System.out.println(x);
        System.out.println(sum);
        System.out.println(sin);
        System.out.println(negation);
        System.out.println(exp);
        System.out.println(exp.evaluate(0.07));
        System.out.println(exp.evaluate(42));
        Expression cos = new Cos(exp);
        System.out.println(cos);
        System.out.println(cos.evaluate(13));
        cos = new Cos(exp);
        sin = new Sin(exp);
        Expression one = new Addition(new Multiplication(cos, cos), new Multiplication(sin, sin));
        System.out.println(one);
        System.out.println(one.evaluate(69));
    }
}