package pl.edu.mimuw;

public class Main {

    public static void main(String[] args) {
        if (args.length != 2) {
            System.out.println("Invalid parameters");
            return;
        }
        try {
            Simulation.run(args[0], Integer.parseInt(args[1]));
        } catch (NumberFormatException e) {
            System.out.println("Enter correct number of days");
        }
    }
}
