package pl.edu.mimuw.events;

import pl.edu.mimuw.Passenger;
import pl.edu.mimuw.Stop;
import pl.edu.mimuw.utilities.Time;

public class PassengerArrival extends Event {
    private final Passenger passenger;
    private final Stop stop;

    public PassengerArrival(int time, Passenger passenger, Stop stop) {
        super(time);
        this.passenger = passenger;
        this.stop = stop;
    }

    @Override
    public void execute() {
        if (stop.isFull()) {
            System.out.println(STR."Wydarzenie nr. \{numberOfEvent}, Godzina \{Time.displayTime(time)}: Pasażer \{passenger.getId()} przychodzi rano na przystanek '\{stop.getName()}', ale nie ma dla niego miejsca");
            return;
        }
        passenger.beginWaiting(time);
        stop.singlePassengerEnters(passenger);
        System.out.println(STR."Wydarzenie nr. \{numberOfEvent}, Godzina \{Time.displayTime(time)}: Na przystanek '\{stop.getName()}' przychodzi pasażer \{passenger.getId()}");
    }
}
