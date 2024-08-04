package pl.edu.mimuw;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;
import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.investor.RandomInvestor;
import pl.edu.mimuw.investor.SMAInvestor;

import java.util.List;

public class FileReaderTest {
    @Test
    public void shouldReadFileAndIgnoreComments() {
        StockMarket stockMarket = FileReader.readFile("./tests/example.in");
        Assertions.assertNotNull(stockMarket);
        List<Company> companies = stockMarket.getCompanies();
        Assertions.assertEquals(3, companies.size());
        Assertions.assertEquals("APL", companies.get(0).getName());
        Assertions.assertEquals("MSFT", companies.get(1).getName());
        Assertions.assertEquals("GOOGL", companies.get(2).getName());
        Assertions.assertEquals(145, companies.get(0).getPrice());
        Assertions.assertEquals(300, companies.get(1).getPrice());
        Assertions.assertEquals(2700, companies.get(2).getPrice());
        List<Investor> investors = stockMarket.getInvestors();
        Assertions.assertEquals(6, investors.size());
        for (int i = 0; i < 4; i++) {
            Assertions.assertEquals(RandomInvestor.class, investors.get(i).getClass());
        }
        for (int i = 4; i < 6; i++) {
            Assertions.assertEquals(SMAInvestor.class, investors.get(i).getClass());
        }
        for (Investor investor : investors) {
            Assertions.assertEquals(100000, investor.getMoney());
            Assertions.assertEquals(5, investor.getNumberOfOwnedShares(companies.get(0)));
            Assertions.assertEquals(15, investor.getNumberOfOwnedShares(companies.get(1)));
            Assertions.assertEquals(3, investor.getNumberOfOwnedShares(companies.get(2)));
        }
    }

    @Test
    public void shouldNotReadNotExistingFile() {
        StockMarket stockMarket = FileReader.readFile("./tests/not-existing-file.txt");
        Assertions.assertNull(stockMarket);
    }

    @Test
    public void shouldNotReadFileInWrongFormat() {
        StockMarket stockMarket = FileReader.readFile("./tests/wronginvestortype.in");
        Assertions.assertNull(stockMarket);
        stockMarket = FileReader.readFile("./tests/emptyfile.in/");
        Assertions.assertNull(stockMarket);
    }
}
