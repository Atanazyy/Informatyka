package pl.edu.mimuw.order.comparators;

import pl.edu.mimuw.order.Order;

import java.util.Comparator;

public class BuyOrderComparator implements Comparator<Order> {
    @Override
    public int compare(Order order1, Order order2) {
        if (order1 == order2) {
            return 0;
        }
        if (order1.getPrice() < order2.getPrice()) {
            return 1;
        }
        if (order1.getPrice() > order2.getPrice()) {
            return -1;
        }
        return order1.getTransactionNumber() > order2.getTransactionNumber() ? -1 : 1;
    }
}
