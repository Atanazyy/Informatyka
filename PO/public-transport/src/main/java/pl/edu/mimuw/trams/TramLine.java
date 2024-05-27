package pl.edu.mimuw.trams;

import pl.edu.mimuw.Line;
import pl.edu.mimuw.Stop;

public class TramLine extends Line {
    private final int numberOfStops;
    private final Stop[] stops;
    private final int lineNumber;
    private final int[] times;
    private final int waitingTime;
    private final int numberOfTrams;
    private int cycleDuration;

    public TramLine(int numberOfStops, Stop[] stops, int[] times, int lineNumber, int numberOfTrams) {
        this.numberOfStops = numberOfStops;
        this.lineNumber = lineNumber;
        this.stops = new Stop[numberOfStops];
        System.arraycopy(stops, 0, this.stops, 0, numberOfStops);
        this.times = new int[numberOfStops - 1];
        System.arraycopy(times, 0, this.times, 0, numberOfStops - 1);
        this.waitingTime = times[numberOfStops - 1];
        cycleDuration = 2 * waitingTime;
        for (int i = 0; i < numberOfStops - 1; i++) {
            cycleDuration += 2 * times[i];
        }
        this.numberOfTrams = numberOfTrams;
    }

    public int getNumberOfStops() {
        return numberOfStops;
    }

    public Stop getStop(int stopNumber) {
        return stops[stopNumber];
    }

    public int getLineNumber() {
        return lineNumber;
    }

    public int getWaitingTime() {
        return waitingTime;
    }

    public String getSchedule() {
        String result = STR."'\{stops[0].getName()}'";
        for (int i = 1; i < numberOfStops; i++) {
            result = STR."\{result} --> \{times[i - 1]} min --> '\{stops[i].getName()}'";
        }
        return result;
    }

    public int getNumberOfTramsNotReversed() {
        return (numberOfTrams + 1) / 2;
    }

    public int calculateIntervals() {
        return cycleDuration / numberOfTrams;
    }

    public int getTime(int stopNumber) {
        return times[stopNumber];
    }
}
