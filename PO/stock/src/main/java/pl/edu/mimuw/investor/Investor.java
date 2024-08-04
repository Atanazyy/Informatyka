package pl.edu.mimuw.investor;

import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;

import java.util.Map;

public abstract class Investor {
    protected final StockMarket stockMarket;
    protected final Map<Company, Integer> shares;
    private final int index;
    private int money;

    public Investor(int index, int money, Map<Company, Integer> shares, StockMarket stockMarket) {
        this.index = index;
        this.stockMarket = stockMarket;
        this.shares = shares;
        this.money = money;
    }

    public abstract void takeTurn();

    public int getNumberOfOwnedShares(Company company) {
        return (shares.get(company) == null) ? 0 : shares.get(company);
    }

    public StockMarket getStockMarket() {
        return stockMarket;
    }

    public int getMoney() {
        return money;
    }

    public void setMoney(int money) {
        this.money = money;
    }

    public void gainMoney(int money) {
        this.money += money;
    }

    public void gainShares(Company company, int shares) {
        this.shares.put(company, getNumberOfOwnedShares(company) + shares);
    }

    public int getIndex() {
        return index;
    }

    public int calculateTotalWealth() {
        int result = money;
        for (Map.Entry<Company, Integer> entry : shares.entrySet()) {
            result += entry.getValue() * entry.getKey().getPrice();
        }
        return result;
    }

    @Override
    public String toString() {
        String result = STR."Investor \{index} has \{money}$ (\{calculateTotalWealth()}$ total) {";
        for (Map.Entry<Company, Integer> entry : shares.entrySet()) {
            result += STR."\{entry.getKey().getName()} = \{entry.getValue()}, ";
        }
        return result + "}";
    }
}
