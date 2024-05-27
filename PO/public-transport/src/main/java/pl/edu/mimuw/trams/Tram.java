package pl.edu.mimuw.trams;

import pl.edu.mimuw.Passenger;
import pl.edu.mimuw.Stop;
import pl.edu.mimuw.Vehicle;
import pl.edu.mimuw.datastructures.DynamicArrayOfPassengers;
import pl.edu.mimuw.datastructures.EventQueue;
import pl.edu.mimuw.events.TramArrival;
import pl.edu.mimuw.utilities.Time;

public class Tram extends Vehicle {
    private final int capacity;
    private final DynamicArrayOfPassengers[] passengers;
    private final TramLine line;
    private final boolean isReversed;
    private int numberOfPassengers;
    private boolean isCurrentlyReversed;
    private int currentStopNumber;

    public Tram(int sideNumber, TramLine line, int tramCapacity, boolean isReversed) {
        super(sideNumber);
        this.capacity = tramCapacity;
        this.isReversed = isReversed;
        this.line = line;
        passengers = new DynamicArrayOfPassengers[line.getNumberOfStops()];
        for (int i = 0; i < line.getNumberOfStops(); i++) {
            passengers[i] = new DynamicArrayOfPassengers();
        }
        numberOfPassengers = 0;
    }

    public int getCapacity() {
        return capacity;
    }

    public boolean isFull() {
        return numberOfPassengers >= capacity;
    }

    public boolean existsPassengerExitingOnStop(int stopNumber) {
        return !passengers[stopNumber].isEmpty();
    }

    public TramLine getLine() {
        return line;
    }

    public Passenger singlePassengerExits(int stopNumber) {
        numberOfPassengers--;
        return passengers[stopNumber].pop();
    }

    public void singlePassengerEnters(Passenger passenger, int destinationStopNumber) {
        numberOfPassengers++;
        passengers[destinationStopNumber].push(passenger);
    }

    public int getNumberOfLastStopInCurrentRide() {
        if (currentStopNumber == 0) {
            return line.getNumberOfStops() - 1;
        }
        if (currentStopNumber == line.getNumberOfStops() - 1) {
            return 0;
        }
        if (isCurrentlyReversed) {
            return 0;
        } else {
            return line.getNumberOfStops() - 1;
        }
    }

    public int getNextStopNumber() {
        if (currentStopNumber == 0) {
            if (isWaitingOnLoop()) {
                return currentStopNumber;
            } else {
                return 1;
            }
        }
        if (currentStopNumber == line.getNumberOfStops() - 1) {
            if (isWaitingOnLoop()) {
                return currentStopNumber;
            } else {
                return currentStopNumber - 1;
            }
        }
        if (isCurrentlyReversed) {
            return currentStopNumber - 1;
        } else {
            return currentStopNumber + 1;
        }
    }

    public int getCurrentStopNumber() {
        return currentStopNumber;
    }

    public int getStartingStopNumber() {
        if (isReversed) {
            return line.getNumberOfStops() - 1;
        } else {
            return 0;
        }
    }

    public Stop getStartingStop() {
        return line.getStop(getStartingStopNumber());
    }

    public void beginDay() {
        isCurrentlyReversed = isReversed;
        currentStopNumber = getStartingStopNumber();
        numberOfPassengers = 0;
        for (int i = 0; i < line.getNumberOfStops(); i++) {
            passengers[i] = new DynamicArrayOfPassengers();
        }
    }

    public boolean isWaitingOnLoop() {
        if (currentStopNumber == 0 && isCurrentlyReversed) {
            return true;
        }
        return currentStopNumber == line.getNumberOfStops() - 1 && !isCurrentlyReversed;
    }

    public TramArrival goToNextStop(int time, EventQueue eventQueue) {

        int arrivalTime = time;
        if (isWaitingOnLoop()) {
            arrivalTime += line.getWaitingTime();
        } else {
            arrivalTime += line.getTime(Math.min(currentStopNumber, getNextStopNumber()));
        }
        if (isWaitingOnLoop() && arrivalTime > Time.covertTimeToMinutes(23, 0)) {
            return null;
        }
        int nextStopNumber = getNextStopNumber();
        if (isWaitingOnLoop()) {
            isCurrentlyReversed = !isCurrentlyReversed;
        }
        currentStopNumber = nextStopNumber;
        return new TramArrival(arrivalTime, this, eventQueue);
    }
}
