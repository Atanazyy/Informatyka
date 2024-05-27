package pl.edu.mimuw;

public class Passenger {
    private final int id;
    private final Stop closestStop;
    private int numberOfTrips;
    private int numberOfTimesWaiting;
    private int timeOfStartOfWaiting;
    private int timeWaiting;

    public Passenger(Stop closestStop, int id) {
        this.closestStop = closestStop;
        this.id = id;
        numberOfTrips = 0;
        timeWaiting = 0;
        numberOfTimesWaiting = 0;
    }

    public void beginDay() {
        numberOfTrips = 0;
        timeWaiting = 0;
        numberOfTimesWaiting = 0;
    }

    public int getId() {
        return id;
    }

    public Stop getClosestStop() {
        return closestStop;
    }

    public void beginWaiting(int time) {
        numberOfTimesWaiting++;
        timeOfStartOfWaiting = time;
    }

    public void endWaiting(int time) {
        timeWaiting += time - timeOfStartOfWaiting;
    }

    public void beginTrip() {
        numberOfTrips++;
    }

    public int getNumberOfTrips() {
        return numberOfTrips;
    }

    public int getTimeWaiting() {
        return timeWaiting;
    }

    public int getNumberOfTimesWaiting() {
        return numberOfTimesWaiting;
    }
}
