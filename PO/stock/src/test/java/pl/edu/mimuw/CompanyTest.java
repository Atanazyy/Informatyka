package pl.edu.mimuw;


import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;


public class CompanyTest {

    private Company createExampleCompany() {
        return new Company("TEST", 42, 100);
    }

    @Test
    void shouldCreateCompany() {
        Company company = createExampleCompany();
        Assertions.assertEquals("TEST", company.getName());
        Assertions.assertEquals(42, company.getPrice());
        Assertions.assertEquals(100, company.getTotalNumberOfShares());
    }

    @Test
    void shouldConvertCompanyToString() {
        Company company = createExampleCompany();
        Assertions.assertEquals("TEST: price = 42$, total shares = 100", company.toString());
    }


}
