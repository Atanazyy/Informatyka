package pl.edu.mimuw.order;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.type.OrderType;

public class TimeLimitedOrder extends Order {
    private final int expirationTurn;

    public TimeLimitedOrder(OrderType orderType, Company company, int numberOfShares, int price, Investor investor, int expirationTurn) {
        super(orderType, company, numberOfShares, price, investor);
        this.expirationTurn = expirationTurn;
    }

    @Override
    public boolean hasExpired() {
        return stockMarket.getTurnNumber() >= expirationTurn;
    }

    @Override
    public boolean canBeProcessed() {
        return true;
    }
}
