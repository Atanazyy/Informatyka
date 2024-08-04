package pl.edu.mimuw.order;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.type.OrderType;

public abstract class Order {
    protected final StockMarket stockMarket;
    protected final OrderType orderType;
    protected final Company company;
    protected final int price;
    private final int numberOfSharesToExchange;
    private final Investor investor;
    private final int transactionNumber;
    protected int numberOfSharesExchanged;

    public Order(OrderType orderType, Company company, int numberOfSharesToExchange, int price, Investor investor) {
        this.orderType = orderType;
        this.company = company;
        this.numberOfSharesToExchange = numberOfSharesToExchange;
        this.numberOfSharesExchanged = 0;
        this.price = price;
        this.investor = investor;
        this.stockMarket = investor.getStockMarket();
        this.transactionNumber = stockMarket.getNextOrderNumber();
    }

    public static void matchOrders(Order sellOrder, Order buyOrder) {
        int price = (sellOrder.transactionNumber < buyOrder.transactionNumber) ? sellOrder.price : buyOrder.price;
        int numberOfShares = Math.min(sellOrder.numberOfSharesToExchange - sellOrder.numberOfSharesExchanged,
                buyOrder.numberOfSharesToExchange - buyOrder.numberOfSharesExchanged);
        sellOrder.fillOrder(price, numberOfShares);
        buyOrder.fillOrder(price, numberOfShares);
        sellOrder.getCompany().setPrice(price);
    }

    public boolean isValid() {
        if (price <= 0) {
            return false;
        }
        int numberOfSharesLeftToExchange = numberOfSharesToExchange - numberOfSharesExchanged;
        if (numberOfSharesLeftToExchange <= 0) {
            return false;
        }
        return price * numberOfSharesLeftToExchange * -orderType.getOrderSign() + investor.getMoney() >= 0
                && investor.getNumberOfOwnedShares(company) + numberOfSharesLeftToExchange * orderType.getOrderSign() >= 0;
    }

    public Company getCompany() {
        return company;
    }

    public int getPrice() {
        return price;
    }

    public int getTransactionNumber() {
        return transactionNumber;
    }

    private void fillOrder(int price, int numberOfSharesToExchange) {
        investor.gainMoney(price * numberOfSharesToExchange * -orderType.getOrderSign());
        investor.gainShares(company, numberOfSharesToExchange * orderType.getOrderSign());
        numberOfSharesExchanged += numberOfSharesToExchange;
    }

    public boolean isCompleted() {
        return numberOfSharesToExchange == numberOfSharesExchanged;
    }

    public OrderType getOrderType() {
        return orderType;
    }

    public int getNumberOfSharesLeftToExchange() {
        return numberOfSharesToExchange - numberOfSharesExchanged;
    }

    @Override
    public String toString() {
        return STR."\{investor.getIndex()} trades \{numberOfSharesToExchange - numberOfSharesExchanged} \{company.getName()} for \{price}";
    }

    public abstract boolean hasExpired();

    public abstract boolean canBeProcessed();
}
