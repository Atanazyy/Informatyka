package pl.edu.mimuw.order;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.type.OrderType;

public class IndefiniteOrder extends Order {
    public IndefiniteOrder(OrderType orderType, Company company, int numberOfShares, int price, Investor investor) {
        super(orderType, company, numberOfShares, price, investor);
    }

    @Override
    public boolean hasExpired() {
        return false;
    }

    @Override
    public boolean canBeProcessed() {
        return true;
    }
}
