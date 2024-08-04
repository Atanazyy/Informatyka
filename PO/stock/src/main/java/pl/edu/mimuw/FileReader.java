package pl.edu.mimuw;

import pl.edu.mimuw.investor.Investor;
import pl.edu.mimuw.investor.RandomInvestor;
import pl.edu.mimuw.investor.SMAInvestor;

import java.io.File;
import java.io.FileNotFoundException;
import java.util.*;

public class FileReader {
    public static StockMarket readFile(String fileName) {
        try {
            File file = new File(fileName);
            Scanner scanner = new Scanner(file);
            List<Company> companies = new ArrayList<>();
            List<Investor> investors = new ArrayList<>();
            StockMarket stockMarket = new StockMarket(investors, companies);
            String[] parts;
            parts = getNewSplitLine(scanner);
            for (int i = 0; i < parts.length; i++) {
                Investor investor;
                if (parts[i].equals("R")) {
                    investor = new RandomInvestor(i, 0, new HashMap<>(), stockMarket, 0.9, 0.9, 1.15);
                } else if (parts[i].equals("S")) {
                    investor = new SMAInvestor(i, 0, new HashMap<>(), stockMarket, 0.9, 1.15);
                } else {
                    throw new IllegalArgumentException();
                }
                investors.add(investor);
            }
            Map<String, Integer> companyIndex = new HashMap<>();
            parts = getNewSplitLine(scanner);
            for (int i = 0; i < parts.length; i++) {
                String name = parts[i].split(":")[0];
                int price = Integer.parseInt(parts[i].split(":")[1]);
                if (name.length() > 5) {
                    throw new IllegalArgumentException();
                }
                companies.add(new Company(name, price, 0));
                companyIndex.put(name, i);
            }
            parts = getNewSplitLine(scanner);
            for (Investor investor : investors) {
                investor.setMoney(Integer.parseInt(parts[0]));
                for (int i = 1; i < parts.length; i++) {
                    String name = parts[i].split(":")[0];
                    int numberOfShares = Integer.parseInt(parts[i].split(":")[1]);
                    Company company = companies.get(companyIndex.get(name));
                    if (company == null) {
                        throw new IllegalArgumentException();
                    }
                    investor.gainShares(company, numberOfShares);
                    company.increaseTotalNumberOfShares(numberOfShares);
                }
            }
            scanner.close();
            return stockMarket;
        } catch (FileNotFoundException e) {
            System.out.println("File not found");
            return null;
        } catch (IllegalArgumentException e) {
            System.out.println("Invalid input");
            return null;
        }
    }

    private static String[] getNewSplitLine(Scanner scanner) {
        while (scanner.hasNextLine()) {
            String line = scanner.nextLine();
            String[] parts = line.split(" ");
            if (parts[0].charAt(0) == '#') {
                continue;
            }
            return parts;
        }
        throw new IllegalArgumentException();
    }
}
