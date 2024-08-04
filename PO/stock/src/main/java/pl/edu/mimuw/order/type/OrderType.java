package pl.edu.mimuw.order.type;

public abstract class OrderType {
    // returns -1 if it is sell order, returns 1 if it is buy order
    public abstract int getOrderSign();
}
