package pl.edu.mimuw.order;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.investor.RandomInvestor;
import pl.edu.mimuw.order.type.BuyOrder;
import pl.edu.mimuw.order.type.SellOrder;

import java.util.ArrayList;
import java.util.HashMap;

public class OrderTest {
    @Test
    public void shouldCreateOrder() {
        Order order = new InstantOrder(new SellOrder(), new Company("A", 15, 100), 10, 20,
                new RandomInvestor(0, 1000, new HashMap<>(), new StockMarket(new ArrayList<>(), new ArrayList<>()), 0.9, 0.9, 1.1));
        Assertions.assertEquals(20, order.getPrice());
    }

    @Test
    public void shouldMatchOrders() {
        Investor investor = new RandomInvestor(0, 1000, new HashMap<>(), new StockMarket(new ArrayList<>(), new ArrayList<>()), 0.9, 0.9, 1.1);
        Company company = new Company("A", 15, 100);
        Order SellOrder = new InstantOrder(new SellOrder(), company, 7, 15, investor);
        Order BuyOrder = new InstantOrder(new SellOrder(), company, 10, 20, investor);
        Order.matchOrders(SellOrder, BuyOrder);
        Assertions.assertEquals(0, SellOrder.getNumberOfSharesLeftToExchange());
        Assertions.assertEquals(3, BuyOrder.getNumberOfSharesLeftToExchange());
    }

    @Test
    public void shouldCheckIfOrderIsValid() {
        Investor investor = new RandomInvestor(0, 0, new HashMap<>(), new StockMarket(new ArrayList<>(), new ArrayList<>()), 0.9, 0.9, 1.1);
        Company company = new Company("A", 15, 100);
        Order SellOrder = new InstantOrder(new SellOrder(), company, 7, 15, investor);
        Assertions.assertFalse(SellOrder.isValid());
        investor.gainShares(company, 7);
        Assertions.assertTrue(SellOrder.isValid());
        Order buyOrder = new InstantOrder(new BuyOrder(), company, 7, 15, investor);
        Assertions.assertFalse(buyOrder.isValid());
        investor.gainMoney(7 * 15);
        Assertions.assertTrue(buyOrder.isValid());
        Order freeOrder = new InstantOrder(new BuyOrder(), company, 7, 0, investor);
        Assertions.assertFalse(freeOrder.isValid());
        Order zeroOrder = new InstantOrder(new BuyOrder(), company, 0, 15, investor);
        Assertions.assertFalse(zeroOrder.isValid());
    }
}
