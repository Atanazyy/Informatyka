package pl.edu.mimuw.order;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.type.OrderType;

public class InstantOrder extends TimeLimitedOrder {
    public InstantOrder(OrderType orderType, Company company, int numberOfShares, int price, Investor investor) {
        super(orderType, company, numberOfShares, price, investor, investor.getStockMarket().getTurnNumber());
    }
}
