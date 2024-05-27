package pl.edu.mimuw;

public abstract class Vehicle {
    private final int sideNumber;

    public Vehicle(int sideNumber) {
        this.sideNumber = sideNumber;
    }

    public int getSideNumber() {
        return sideNumber;
    }
}
