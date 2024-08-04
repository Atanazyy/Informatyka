package pl.edu.mimuw;

import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.order.Order;
import pl.edu.mimuw.order.comparators.BuyOrderComparator;
import pl.edu.mimuw.order.comparators.SellOrderComparator;

import java.util.*;

public class StockMarket {
    private final List<Investor> investors;
    private final List<Company> companies;
    private final Map<Company, PriorityQueue<Order>> sellOrders;
    private final Map<Company, PriorityQueue<Order>> buyOrders;
    private int turnNumber;
    private int orderNumber;

    public StockMarket(List<Investor> investors, List<Company> companies) {
        this.investors = investors;
        this.companies = companies;
        sellOrders = new HashMap<>();
        buyOrders = new HashMap<>();
    }

    public void initialize() {
        turnNumber = 0;
        orderNumber = 0;
        for (Company company : companies) {
            this.sellOrders.put(company, new PriorityQueue<>(new SellOrderComparator()));
            this.buyOrders.put(company, new PriorityQueue<>(new BuyOrderComparator()));
        }
    }

    public void performTurn() {
        turnNumber++;
        List<Investor> shuffledInvestors = new ArrayList<>(investors);
        Collections.shuffle(shuffledInvestors);
        for (Investor investor : shuffledInvestors) {
            investor.takeTurn();
        }
        for (Company company : companies) {
            while (!sellOrders.get(company).isEmpty() && !buyOrders.get(company).isEmpty()
                    && Objects.requireNonNull(sellOrders.get(company).peek()).getPrice() <= Objects.requireNonNull(buyOrders.get(company).peek()).getPrice()) {
                if (!Objects.requireNonNull(sellOrders.get(company).peek()).isValid() || !Objects.requireNonNull(sellOrders.get(company).peek()).canBeProcessed()) {
                    sellOrders.get(company).remove();
                } else if (!Objects.requireNonNull(buyOrders.get(company).peek()).isValid() || !Objects.requireNonNull(buyOrders.get(company).peek()).canBeProcessed()) {
                    buyOrders.get(company).remove();
                } else {
                    Order.matchOrders(Objects.requireNonNull(sellOrders.get(company).peek()), Objects.requireNonNull(buyOrders.get(company).peek()));
                    if (Objects.requireNonNull(sellOrders.get(company).peek()).isCompleted()) {
                        sellOrders.get(company).remove();
                    }
                    if (Objects.requireNonNull(buyOrders.get(company).peek()).isCompleted()) {
                        buyOrders.get(company).remove();
                    }
                }
            }
        }
        for (Company company : companies) {
            PriorityQueue<Order> newSellOrders = new PriorityQueue<>(new SellOrderComparator());
            PriorityQueue<Order> newBuyOrders = new PriorityQueue<>(new BuyOrderComparator());
            for (Order order : sellOrders.get(company)) {
                if (!order.isCompleted() && order.isValid() && !order.hasExpired()) {
                    newSellOrders.add(order);
                }
            }
            for (Order order : buyOrders.get(company)) {
                if (!order.isCompleted() && order.isValid() && !order.hasExpired()) {
                    newBuyOrders.add(order);
                }
            }
            buyOrders.put(company, newBuyOrders);
            sellOrders.put(company, newSellOrders);
        }
    }

    public void placeOrder(Order order) {
        if (!order.isValid()) {
            return;
        }
        if (order.getOrderType().getOrderSign() < 0) {
            sellOrders.get(order.getCompany()).add(order);
        } else {
            buyOrders.get(order.getCompany()).add(order);
        }
    }

    public int numberOfSharesThatCanBeSold(Company company, int price) {
        int result = 0;
        for (Order order : buyOrders.get(company)) {
            if (price >= order.getPrice()) {
                result += order.getNumberOfSharesLeftToExchange();
            }
        }
        return result;
    }

    public int numberOfSharesThatCanBeBought(Company company, int price) {
        int result = 0;
        for (Order order : sellOrders.get(company)) {
            if (price >= order.getPrice()) {
                result += order.getNumberOfSharesLeftToExchange();
            }
        }
        return result;
    }


    private void printAllOrders() {
        for (Company company : companies) {
            System.out.println(STR."Company: \{company.getName()}");
            System.out.println("Sell Orders: ");
            for (Order order : sellOrders.get(company)) {
                System.out.println(order);
            }
            System.out.println("Buy Orders: ");
            for (Order order : buyOrders.get(company)) {
                System.out.println(order);
            }
        }
    }

    public void printAllCompanies() {
        for (Company company : companies) {
            System.out.println(company);
        }
    }

    public void printAllInvestors() {
        for (Investor investor : investors) {
            System.out.println(investor);
        }
    }

    public int getTurnNumber() {
        return turnNumber;
    }

    public List<Company> getCompanies() {
        return companies;
    }

    public int getNumberOfCompanies() {
        return companies.size();
    }

    public int getNumberOfInvestors() {
        return investors.size();
    }

    public int getNextOrderNumber() {
        orderNumber++;
        return orderNumber;
    }

    public List<Investor> getInvestors() {
        return investors;
    }
}
