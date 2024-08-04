package pl.edu.mimuw.investor;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;
import pl.edu.mimuw.order.*;
import pl.edu.mimuw.order.type.BuyOrder;
import pl.edu.mimuw.order.type.OrderType;
import pl.edu.mimuw.order.type.SellOrder;

import java.util.Map;
import java.util.concurrent.ThreadLocalRandom;

public class RandomInvestor extends Investor {
    private final double randomLowerBound;
    private final double randomUpperBound;
    private double chanceToPerformAnyAction;

    public RandomInvestor(int index, int money, Map<Company, Integer> shares, StockMarket stockMarket, double chanceToPerformAnyAction, double randomLowerBound, double randomUpperBound) {
        super(index, money, shares, stockMarket);
        this.chanceToPerformAnyAction = chanceToPerformAnyAction;
        this.randomLowerBound = randomLowerBound;
        this.randomUpperBound = randomUpperBound;
    }

    @Override
    public void takeTurn() {
        if (ThreadLocalRandom.current().nextDouble() > chanceToPerformAnyAction) {
            return;
        }
        Company company = stockMarket.getCompanies().get(ThreadLocalRandom.current()
                .nextInt(stockMarket.getNumberOfCompanies()));
        int price = company.getPrice();
        price = (int) ThreadLocalRandom.current()
                .nextDouble(Math.floor(price * randomLowerBound) - 1, Math.ceil(price * randomUpperBound) + 1);
        int numberOfShares = 1 + company.getTotalNumberOfShares() / (stockMarket.getNumberOfInvestors() * stockMarket.getNumberOfInvestors());
        numberOfShares = (int) ThreadLocalRandom.current()
                .nextDouble(Math.floor(numberOfShares * randomLowerBound), Math.ceil(numberOfShares * randomUpperBound));
        OrderType orderType = new SellOrder();
        if (ThreadLocalRandom.current().nextDouble() < 0.5) {
            orderType = new BuyOrder();
        }
        int expirationType = ThreadLocalRandom.current().nextInt(0, 4);
        Order order = switch (expirationType) {
            case 0 -> new IndefiniteOrder(orderType, company, numberOfShares, price, this);
            case 1 -> new InstantOrder(orderType, company, numberOfShares, price, this);
            case 2 ->
                    new TimeLimitedOrder(orderType, company, numberOfShares, price, this, (int) ThreadLocalRandom.current()
                            .nextDouble(stockMarket.getTurnNumber(), Math.ceil(randomUpperBound * stockMarket.getTurnNumber())));
            case 3 -> new CompleteOrCancelOrder(orderType, company, numberOfShares, price, this);
            default -> null;
        };
        assert order != null;
        stockMarket.placeOrder(order);
    }

    public void setChanceToPerformAnyAction(double chanceToPerformAnyAction) {
        this.chanceToPerformAnyAction = chanceToPerformAnyAction;
    }
}
