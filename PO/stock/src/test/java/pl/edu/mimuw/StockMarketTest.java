package pl.edu.mimuw;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.investor.RandomInvestor;
import pl.edu.mimuw.order.InstantOrder;
import pl.edu.mimuw.order.type.BuyOrder;
import pl.edu.mimuw.order.type.SellOrder;

public class StockMarketTest {
    @Test
    void shouldProcessOneTurn() { // example from task description
        StockMarket stockMarket = FileReader.readFile("./tests/taskdescription.in");
        Assertions.assertNotNull(stockMarket);
        stockMarket.initialize();
        Company company = stockMarket.getCompanies().getFirst();
        Investor investor1 = stockMarket.getInvestors().get(0);
        Investor investor2 = stockMarket.getInvestors().get(1);
        RandomInvestor investor1r = (RandomInvestor) investor1;
        investor1r.setChanceToPerformAnyAction(0);
        RandomInvestor investor2r = (RandomInvestor) investor2;
        investor2r.setChanceToPerformAnyAction(0);
        stockMarket.placeOrder(new InstantOrder(new SellOrder(), company, 10, 123, investor1));
        stockMarket.placeOrder(new InstantOrder(new SellOrder(), company, 25, 124, investor1));
        stockMarket.placeOrder(new InstantOrder(new SellOrder(), company, 30, 125, investor1));
        stockMarket.placeOrder(new InstantOrder(new BuyOrder(), company, 100, 125, investor2));
        stockMarket.performTurn();
        Assertions.assertEquals(100000 + 10 * 123 + 25 * 124 + 30 * 125, investor1.getMoney());
    }
}
