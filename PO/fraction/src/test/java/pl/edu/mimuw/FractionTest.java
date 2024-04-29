package pl.edu.mimuw;

import org.junit.jupiter.api.Assertions;
import org.junit.jupiter.api.Test;

import java.math.BigInteger;

public class FractionTest {

    @Test
    void shouldCreateFractionUsingBigInteger() {
        BigInteger numerator = BigInteger.valueOf(60);
        BigInteger denominator = BigInteger.valueOf(40);

        Fraction fraction = new Fraction(numerator, denominator);

        Assertions.assertEquals(BigInteger.valueOf(3), fraction.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(2), fraction.getDenominator());
    }

    @Test
    void shouldCreateFractionUsingInteger() {
        var numerator = 60;
        var denominator = 40;

        Fraction fraction = new Fraction(numerator, denominator);

        Assertions.assertEquals(BigInteger.valueOf(3), fraction.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(2), fraction.getDenominator());
    }

    @Test
    void shouldAddFractions() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(4), BigInteger.valueOf(3));
        Fraction fraction3 = new Fraction(BigInteger.valueOf(5), BigInteger.valueOf(2));

        Fraction result1 = fraction1.add(fraction2);
        Fraction result2 = fraction1.add(fraction3);

        Assertions.assertEquals(BigInteger.valueOf(17), result1.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(6), result1.getDenominator());
        Assertions.assertEquals(BigInteger.valueOf(4), result2.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(1), result2.getDenominator());
    }
    @Test
    void shouldMultiplyFractions() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(4), BigInteger.valueOf(3));
        Fraction fraction3 = new Fraction(BigInteger.valueOf(5), BigInteger.valueOf(2));

        Fraction result1 = fraction1.multiply(fraction2);
        Fraction result2 = fraction1.multiply(fraction3);

        Assertions.assertEquals(BigInteger.valueOf(2), result1.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(1), result1.getDenominator());
        Assertions.assertEquals(BigInteger.valueOf(15), result2.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(4), result2.getDenominator());
    }
    @Test
    void shouldDivideFractions() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(4));
        Fraction fraction3 = new Fraction(BigInteger.valueOf(2), BigInteger.valueOf(5));

        Fraction result1 = fraction1.divide(fraction2);
        Fraction result2 = fraction1.divide(fraction3);

        Assertions.assertEquals(BigInteger.valueOf(2), result1.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(1), result1.getDenominator());
        Assertions.assertEquals(BigInteger.valueOf(15), result2.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(4), result2.getDenominator());
    }

    @Test
    void shouldNotDivideByZero() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(0), BigInteger.valueOf(3));

        Fraction result1 = fraction1.divide(fraction2);

        Assertions.assertEquals(BigInteger.valueOf(3), result1.getNumerator());
        Assertions.assertEquals(BigInteger.valueOf(2), result1.getDenominator());
    }

    @Test
    void shouldCompareFractions() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(6), BigInteger.valueOf(4));
        Fraction fraction3 = new Fraction(BigInteger.valueOf(4), BigInteger.valueOf(2));

        Assertions.assertEquals(fraction1, fraction1);
        Assertions.assertEquals(fraction2, fraction2);
        Assertions.assertEquals(fraction1, fraction2);
        Assertions.assertNotEquals(fraction1, fraction3);
        Assertions.assertNotEquals(fraction2, fraction3);
    }

    @Test
    void shouldConvertToStringFractions() {
        Fraction fraction1 = new Fraction(BigInteger.valueOf(3), BigInteger.valueOf(2));
        Fraction fraction2 = new Fraction(BigInteger.valueOf(6), BigInteger.valueOf(4));
        Fraction fraction3 = new Fraction(BigInteger.valueOf(4), BigInteger.valueOf(2));

        Assertions.assertEquals(fraction1.toString(), "3/2");
        Assertions.assertEquals(fraction2.toString(), "3/2");
        Assertions.assertEquals(fraction3.toString(), "2/1");
    }
}


