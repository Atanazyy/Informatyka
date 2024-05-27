package pl.edu.mimuw.utilities;

public class Statistics {
    private int totalTimeWaiting;
    private int numberOfTrips;
    private int numberOfTimesWaiting;

    public Statistics() {
        totalTimeWaiting = 0;
        numberOfTrips = 0;
        numberOfTimesWaiting = 0;
    }

    public void increaseNumberOfTimesWaiting(int numberOfTimesWaiting) {
        this.numberOfTimesWaiting += numberOfTimesWaiting;
    }

    public void increaseTotalTimeWaiting(int timeWaiting) {
        this.totalTimeWaiting += timeWaiting;
    }

    public void increaseNumberOfTrips(int numberOfTrips) {
        this.numberOfTrips += numberOfTrips;
    }

    public int getNumberOfTrips() {
        return numberOfTrips;
    }

    public int getTotalTimeWaiting() {
        return totalTimeWaiting;
    }

    public int getNumberOfTimesWaiting() {
        return numberOfTimesWaiting;
    }

    private double getAverageTimeWaiting() {
        return (double) totalTimeWaiting / numberOfTimesWaiting;
    }

    public void print() {
        System.out.println(STR."Łączna liczba przejazdów: \{numberOfTrips}");
        System.out.print(STR."Średni czas czekania na przystanku: \{getAverageTimeWaiting()} minut\n");
        System.out.println(STR."Łączny czas czekania na przystankach: \{totalTimeWaiting} minut");
    }
}
