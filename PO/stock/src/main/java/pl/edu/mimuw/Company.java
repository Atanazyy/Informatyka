package pl.edu.mimuw;

public class Company {
    private final String name;
    private int totalNumberOfShares;
    private int price;

    public Company(String name, int price, int totalNumberOfShares) {
        this.name = name;
        this.price = price;
        this.totalNumberOfShares = totalNumberOfShares;
    }

    @Override
    public String toString() {
        return STR."\{name}: price = \{price}$, total shares = \{totalNumberOfShares}";
    }

    public int getPrice() {
        return price;
    }

    public void setPrice(int price) {
        this.price = price;
    }

    public int getTotalNumberOfShares() {
        return totalNumberOfShares;
    }

    public void increaseTotalNumberOfShares(int numberOfShares) {
        this.totalNumberOfShares += numberOfShares;
    }

    public String getName() {
        return name;
    }

}
