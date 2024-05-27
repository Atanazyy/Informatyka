package pl.edu.mimuw;

import pl.edu.mimuw.datastructures.DynamicArrayOfPassengers;

public class Stop {
    private final String name;
    private final int capacity;
    private DynamicArrayOfPassengers passengers;

    public Stop(String name, int stopCapacity) {
        this.name = name;
        this.capacity = stopCapacity;
        passengers = new DynamicArrayOfPassengers();
    }

    public String getName() {
        return name;
    }

    public boolean isFull() {
        return passengers.size() >= capacity;
    }

    public boolean isEmpty() {
        return passengers.isEmpty();
    }

    public void singlePassengerEnters(Passenger passenger) {
        passengers.push(passenger);
    }

    public Passenger singlePassengerExits() {
        return passengers.pop();
    }

    public int getCapacity() {
        return capacity;
    }

    public void beginDay() {
        passengers = new DynamicArrayOfPassengers();
    }
}
