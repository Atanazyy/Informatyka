package pl.edu.mimuw;

import pl.edu.mimuw.datastructures.EventQueue;
import pl.edu.mimuw.datastructures.HeapEventQueue;
import pl.edu.mimuw.events.PassengerArrival;
import pl.edu.mimuw.events.TramArrival;
import pl.edu.mimuw.trams.Tram;
import pl.edu.mimuw.trams.TramLine;
import pl.edu.mimuw.utilities.Losowanie;
import pl.edu.mimuw.utilities.Statistics;
import pl.edu.mimuw.utilities.Time;

import java.util.Scanner;

public class Simulation {
    private int numberOfDays;
    private int numberOfTramLines;
    private TramLine[] tramLines;
    private Tram[][] trams;
    private int numberOfStops;
    private Stop[] stops;
    private int numberOfPassengers;
    private Passenger[] passengers;
    private Statistics[] statistics;
    private Statistics total;

    private Stop findStop(String name) {
        for (int i = 0; i < numberOfStops; i++) {
            if (name.equals(stops[i].getName())) {
                return stops[i];
            }
        }
        return null;
    }

    private void readData() {
        Scanner scanner = new Scanner(System.in);
        numberOfDays = scanner.nextInt();
        total = new Statistics();
        statistics = new Statistics[numberOfDays];
        int stopsCapacity = scanner.nextInt();
        numberOfStops = scanner.nextInt();
        stops = new Stop[numberOfStops];
        for (int i = 0; i < numberOfStops; i++) {
            stops[i] = new Stop(scanner.next(), stopsCapacity);
        }
        numberOfPassengers = scanner.nextInt();
        passengers = new Passenger[numberOfPassengers];
        for (int i = 0; i < numberOfPassengers; i++) {
            passengers[i] = new Passenger(stops[Losowanie.losuj(0, numberOfStops - 1)], i);
        }
        int tramsCapacity = scanner.nextInt();
        numberOfTramLines = scanner.nextInt();
        tramLines = new TramLine[numberOfTramLines];
        trams = new Tram[numberOfTramLines][];
        int nextSideNumber = 0;
        for (int i = 0; i < numberOfTramLines; i++) {
            int numberOfTramsOnLine = scanner.nextInt();
            int lineLength = scanner.nextInt();
            Stop[] stopsOnLine = new Stop[lineLength];
            int[] distances = new int[lineLength];
            for (int j = 0; j < lineLength; j++) {
                stopsOnLine[j] = findStop(scanner.next());
                distances[j] = scanner.nextInt();
            }
            tramLines[i] = new TramLine(lineLength, stopsOnLine, distances, i, numberOfTramsOnLine);
            trams[i] = new Tram[numberOfTramsOnLine];
            for (int j = 0; j < numberOfTramsOnLine; j++) {
                trams[i][j] = new Tram(nextSideNumber++, tramLines[i], tramsCapacity,
                        j < tramLines[i].getNumberOfTramsNotReversed());
            }
        }
    }

    private void printData() {
        System.out.println(STR."Symulacja trwa \{numberOfDays} dni");
        System.out.println("\nLista przystanków:");
        for (int i = 0; i < numberOfStops; i++) {
            System.out.println(STR."\{i}. '\{stops[i].getName()}', pojemność: \{stops[i].getCapacity()} pasażerów");
        }
        System.out.println("\nLista pasażerów biorących udział w symulacji:");
        for (int i = 0; i < numberOfPassengers; i++) {
            System.out.println(STR."Pasażer \{passengers[i].getId()}, najbliższy przystanek: \{passengers[i].getClosestStop().getName()}");
        }
        System.out.println("\nLista lini tramwajowych:");
        for (int i = 0; i < numberOfTramLines; i++) {
            System.out.println(STR."Linia numer \{tramLines[i].getLineNumber()}:");
            System.out.println(STR."Czas czekania na pętlach to \{tramLines[i].getWaitingTime()} minut, a rozkład jazdy to:");
            System.out.println(tramLines[i].getSchedule());
        }
        System.out.println("\nLista wszystkich tramwajów:");
        for (int i = 0; i < numberOfTramLines; i++) {
            for (int j = 0; j < trams[i].length; j++) {
                System.out.println(STR."Numer boczny: \{trams[i][j].getSideNumber()}, linia: \{trams[i][j].getLine().getLineNumber()}, Startuje z przystanku '\{trams[i][j].getStartingStop().getName()}', pojemność: \{trams[i][j].getCapacity()} pasażerów");
            }
        }
    }

    public void beginDay() {
        for (int i = 0; i < numberOfTramLines; i++) {
            for (int j = 0; j < trams[i].length; j++) {
                trams[i][j].beginDay();
            }
        }
        for (int i = 0; i < numberOfStops; i++) {
            stops[i].beginDay();
        }
        for (int i = 0; i < numberOfPassengers; i++) {
            passengers[i].beginDay();
        }
    }

    public void simulateSingleDay(int dayNumber) {
        statistics[dayNumber] = new Statistics();
        beginDay();
        EventQueue eventQueue = new HeapEventQueue();
        for (int i = 0; i < numberOfTramLines; i++) {
            for (int j = 0; j < tramLines[i].getNumberOfTramsNotReversed(); j++) {
                eventQueue.push(new TramArrival(Time.covertTimeToMinutes(6, 0) + j * tramLines[i].calculateIntervals(),
                        trams[i][j], eventQueue));
            }
            for (int j = tramLines[i].getNumberOfTramsNotReversed(); j < trams[i].length; j++) {
                eventQueue.push(new TramArrival(Time.covertTimeToMinutes(6, 0) + (j - tramLines[i].getNumberOfTramsNotReversed()) * tramLines[i].calculateIntervals(), trams[i][j], eventQueue));
            }
        }
        for (int i = 0; i < numberOfPassengers; i++) {
            eventQueue.push(new PassengerArrival(Losowanie.losuj(Time.covertTimeToMinutes(6, 0), Time.covertTimeToMinutes(12, 0)),
                    passengers[i], passengers[i].getClosestStop()));
        }
        while (!eventQueue.isEmpty()) {
            eventQueue.executeEarliestEvent();
        }
        for (int i = 0; i < numberOfPassengers; i++) {
            passengers[i].endWaiting(Time.covertTimeToMinutes(24, 0));
            statistics[dayNumber].increaseNumberOfTrips(passengers[i].getNumberOfTrips());
            statistics[dayNumber].increaseTotalTimeWaiting(passengers[i].getTimeWaiting());
            statistics[dayNumber].increaseNumberOfTimesWaiting(passengers[i].getNumberOfTimesWaiting());
        }
        total.increaseNumberOfTrips(statistics[dayNumber].getNumberOfTrips());
        total.increaseTotalTimeWaiting(statistics[dayNumber].getTotalTimeWaiting());
        total.increaseNumberOfTimesWaiting(statistics[dayNumber].getNumberOfTimesWaiting());
    }

    private void printStatistics() {
        System.out.println("\nStatystyki:");
        for (int i = 0; i < numberOfDays; i++) {
            System.out.println(STR."\nDzień \{i}:");
            statistics[i].print();
        }
        System.out.println("\nPodczas całej symulacji:");
        total.print();
    }

    public void run() {
        readData();
        printData();
        for (int i = 0; i < numberOfDays; i++) {
            System.out.println(STR."\nDzień \{i}:");
            simulateSingleDay(i);
        }
        printStatistics();
    }
}
