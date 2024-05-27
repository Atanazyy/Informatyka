package pl.edu.mimuw.events;

import pl.edu.mimuw.Passenger;
import pl.edu.mimuw.Stop;
import pl.edu.mimuw.datastructures.EventQueue;
import pl.edu.mimuw.trams.Tram;
import pl.edu.mimuw.utilities.Losowanie;
import pl.edu.mimuw.utilities.Time;

public class TramArrival extends Event {
    private final Tram tram;
    private final EventQueue eventQueue;

    public TramArrival(int time, Tram tram, EventQueue eventQueue) {
        super(time);
        this.tram = tram;
        this.eventQueue = eventQueue;
    }

    @Override
    public void execute() {
        int stopNumber = tram.getCurrentStopNumber();
        Stop stop = tram.getLine().getStop(stopNumber);
        System.out.println(STR."Wydarzenie nr. \{numberOfEvent}, Godzina \{Time.displayTime(time)}: Przyjechał tramwaj linii \{tram.getLine().getLineNumber()} (numer boczny \{tram.getSideNumber()}) na przystanek '\{stop.getName()}'");
        while (tram.existsPassengerExitingOnStop(stopNumber) && !stop.isFull()) {
            Passenger passenger = tram.singlePassengerExits(stopNumber);
            stop.singlePassengerEnters(passenger);
            passenger.beginWaiting(time);
            System.out.println(STR."    Wysiada pasażer \{passenger.getId()}");
        }
        while (!tram.isWaitingOnLoop() && !stop.isEmpty() && !tram.isFull()) {
            Passenger passenger = stop.singlePassengerExits();
            int numberOfNextStop = tram.getNextStopNumber();
            int numberOfLastStop = tram.getNumberOfLastStopInCurrentRide();
            int destinationStopNumber = Losowanie.losuj(Math.min(numberOfNextStop, numberOfLastStop), Math.max(numberOfNextStop, numberOfLastStop));
            tram.singlePassengerEnters(passenger, destinationStopNumber);
            passenger.beginTrip();
            passenger.endWaiting(time);
            System.out.println(STR."    Wsiada pasażer \{passenger.getId()} z zamiarem dojechania na przystanek '\{tram.getLine().getStop(destinationStopNumber).getName()}'");
        }
        Event nextArrival = tram.goToNextStop(time, eventQueue);
        eventQueue.push(nextArrival);
    }
}
