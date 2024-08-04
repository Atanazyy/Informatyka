package pl.edu.mimuw;

public class Simulation {
    public static void run(String fileName, int numberOfDays) {
        StockMarket stockMarket = FileReader.readFile(fileName);
        if (stockMarket == null) {
            System.out.println("Error reading stock market");
            return;
        }
        stockMarket.initialize();
        System.out.println("List of companies before simulation:");
        stockMarket.printAllCompanies();
        System.out.println("List of Investors before simulation:");
        stockMarket.printAllInvestors();
        for (int i = 1; i <= numberOfDays; i++) {
            stockMarket.performTurn();
        }
        System.out.println("List of companies after simulation:");
        stockMarket.printAllCompanies();
        System.out.println("List of Investors after simulation:");
        stockMarket.printAllInvestors();
    }
}
