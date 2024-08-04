package pl.edu.mimuw.order;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.type.BuyOrder;
import pl.edu.mimuw.order.type.OrderType;

public class CompleteOrCancelOrder extends InstantOrder {
    public CompleteOrCancelOrder(OrderType orderType, Company company, int numberOfShares, int price, Investor investor) {
        super(orderType, company, numberOfShares, price, investor);
    }

    @Override
    public boolean canBeProcessed() {
        if (orderType.getClass() == BuyOrder.class) {
            return stockMarket.numberOfSharesThatCanBeSold(company, price) >= getNumberOfSharesLeftToExchange();
        } else {
            return stockMarket.numberOfSharesThatCanBeBought(company, price) >= getNumberOfSharesLeftToExchange();
        }
    }
}
