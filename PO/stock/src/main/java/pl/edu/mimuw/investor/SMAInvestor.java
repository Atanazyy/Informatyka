package pl.edu.mimuw.investor;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;
import pl.edu.mimuw.order.InstantOrder;
import pl.edu.mimuw.order.type.BuyOrder;
import pl.edu.mimuw.order.type.SellOrder;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ThreadLocalRandom;

public class SMAInvestor extends Investor {
    private final Map<Company, List<Integer>> prices;
    private final double randomLowerBound;
    private final double randomUpperBound;
    private final Map<Company, Integer> previousSMA5;
    private final Map<Company, Integer> previousSMA10;

    public SMAInvestor(int index, int money, Map<Company, Integer> shares, StockMarket stockMarket, double randomLowerBound, double randomUpperBound) {
        super(index, money, shares, stockMarket);
        prices = new HashMap<>();
        previousSMA5 = new HashMap<>();
        previousSMA10 = new HashMap<>();
        this.randomLowerBound = randomLowerBound;
        this.randomUpperBound = randomUpperBound;
    }

    @Override
    public void takeTurn() {
        for (Company company : stockMarket.getCompanies()) {
            prices.computeIfAbsent(company, _ -> new ArrayList<>());
            prices.get(company).add(company.getPrice());
        }
        if (stockMarket.getTurnNumber() < 10) {
            return;
        }
        for (Company company : stockMarket.getCompanies()) {
            List<Integer> prices = this.prices.get(company);
            int SMA5 = 0;
            for (int i = prices.size() - 5; i < prices.size(); i++) {
                SMA5 += prices.get(i);
            }
            SMA5 /= 5;
            int SMA10 = 0;
            for (int i = prices.size() - 10; i < prices.size(); i++) {
                SMA10 += prices.get(i);
            }
            SMA10 /= 10;
            if (stockMarket.getTurnNumber() == 10) {
                previousSMA5.put(company, SMA5);
                previousSMA10.put(company, SMA10);
                continue;
            }
            int price = company.getPrice();
            price = (int) ThreadLocalRandom.current()
                    .nextDouble(Math.floor(price * randomLowerBound) - 1, Math.ceil(price * randomUpperBound) + 1);
            int numberOfShares = 1 + company.getTotalNumberOfShares() / stockMarket.getNumberOfInvestors();
            numberOfShares = (int) ThreadLocalRandom.current()
                    .nextDouble(Math.floor(numberOfShares * randomLowerBound), Math.ceil(numberOfShares * randomUpperBound));
            if (previousSMA5.get(company) > previousSMA10.get(company) && SMA5 < SMA10) {
                stockMarket.placeOrder(new InstantOrder(new SellOrder(), company, numberOfShares, price, this));
            }
            if (previousSMA5.get(company) < previousSMA10.get(company) && SMA5 > SMA10) {
                stockMarket.placeOrder(new InstantOrder(new BuyOrder(), company, numberOfShares, price, this));
            }
            previousSMA5.put(company, SMA5);
            previousSMA10.put(company, SMA10);
        }
    }
}
