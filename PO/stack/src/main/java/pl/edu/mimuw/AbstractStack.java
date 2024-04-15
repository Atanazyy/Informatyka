package pl.edu.mimuw;

public abstract class AbstractStack {
    public abstract int pop();

    public abstract int getSize();

    public abstract boolean isEmpty();

    public abstract int peek();

    public abstract void push(int value);
}
