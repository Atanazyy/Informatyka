package pl.edu.mimuw;

import java.math.BigInteger;

public class Fraction {
    private final BigInteger numerator;
    private final BigInteger denominator;

    public Fraction(BigInteger numerator, BigInteger denominator) {
        var gcd = numerator.gcd(denominator);
        this.numerator = numerator.divide(gcd);
        this.denominator = denominator.divide(gcd);
    }

    public Fraction(int numerator, int denominator) {
        this(BigInteger.valueOf(numerator), BigInteger.valueOf(denominator));
    }

    public Fraction add(Fraction other) {
        BigInteger newNumerator = numerator.multiply(other.getDenominator()).add(other.getNumerator().multiply(denominator));
        BigInteger newDenominator = denominator.multiply(other.getDenominator());
        return new Fraction(newNumerator, newDenominator);
    }

    public Fraction subtract(Fraction other) {
        BigInteger newNumerator = numerator.multiply(other.getDenominator()).subtract(other.getNumerator().multiply(denominator));
        BigInteger newDenominator = denominator.multiply(other.getDenominator());
        return new Fraction(newNumerator, newDenominator);
    }

    public Fraction multiply(Fraction other) {
        BigInteger newNumerator = numerator.multiply(other.getNumerator());
        BigInteger newDenominator = denominator.multiply(other.getDenominator());
        return new Fraction(newNumerator, newDenominator);
    }

    public Fraction divide(Fraction other) {
        if(other.getNumerator().equals(BigInteger.ZERO)) {
            return new Fraction(numerator, denominator);
        }
        BigInteger newNumerator = numerator.multiply(other.getDenominator());
        BigInteger newDenominator = denominator.multiply(other.getNumerator());
        return new Fraction(newNumerator, newDenominator);
    }

    @Override
    public String toString() {
        return numerator + "/" + denominator;
    }

    @Override
    public boolean equals(Object o) {
        if (!(o instanceof Fraction)) {
            return false;
        }
        Fraction other = (Fraction) o;
        if (!this.denominator.equals(other.getDenominator())) {
            return false;
        }
        if (!this.numerator.equals(other.getNumerator())) {
            return false;
        }
        return true;
    }

    public BigInteger getDenominator() {
        return denominator;
    }

    public BigInteger getNumerator() {
        return numerator;
    }
}
