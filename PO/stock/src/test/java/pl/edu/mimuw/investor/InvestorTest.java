package pl.edu.mimuw.investor;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import pl.edu.mimuw.Company;
import pl.edu.mimuw.StockMarket;

import java.util.ArrayList;
import java.util.HashMap;

public class InvestorTest {
    private Investor createExampleRandomInvestor() {
        return new RandomInvestor(15, 1000, new HashMap<>(), new StockMarket(new ArrayList<>(),
                new ArrayList<>()), 0.5, 0.5, 1.5);
    }

    @Test
    public void shouldCreateRandomInvestor() {
        Investor investor = createExampleRandomInvestor();
        Company company1 = new Company("A", 10, 3);
        Company company2 = new Company("B", 15, 2);
        Assertions.assertEquals(15, investor.getIndex());
        Assertions.assertEquals(1000, investor.getMoney());
        Assertions.assertEquals(0, investor.getNumberOfOwnedShares(company1));
        Assertions.assertEquals(0, investor.getNumberOfOwnedShares(company2));
    }

    @Test
    public void shouldGainShares() {
        Investor investor = createExampleRandomInvestor();
        Company company1 = new Company("A", 10, 3);
        Company company2 = new Company("B", 15, 2);
        investor.gainShares(company1, 5);
        investor.gainShares(company2, 2);
        Assertions.assertEquals(5, investor.getNumberOfOwnedShares(company1));
        Assertions.assertEquals(2, investor.getNumberOfOwnedShares(company2));
    }

    @Test
    public void shouldCalculateTotalWealth() {
        Investor investor = createExampleRandomInvestor();
        Company company1 = new Company("A", 10, 3);
        Company company2 = new Company("B", 15, 2);
        Assertions.assertEquals(1000, investor.calculateTotalWealth());
        investor.gainShares(company1, 5);
        Assertions.assertEquals(1050, investor.calculateTotalWealth());
        investor.gainShares(company2, 2);
        Assertions.assertEquals(1080, investor.calculateTotalWealth());
    }
}
