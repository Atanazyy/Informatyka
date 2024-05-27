package pl.edu.mimuw.events;

public abstract class Event {
    protected final int time;
    protected int numberOfEvent;

    public Event(int time) {
        this.time = time;
    }

    public boolean isEarlier(Event other) {
        if (time != other.getTime()) {
            return time < other.getTime();
        }
        return numberOfEvent < other.getNumberOfEvent();
    }

    public int getTime() {
        return time;
    }

    public int getNumberOfEvent() {
        return numberOfEvent;
    }

    public void setNumberOfEvent(int numberOfEvent) {
        this.numberOfEvent = numberOfEvent;
    }

    public abstract void execute();
}
