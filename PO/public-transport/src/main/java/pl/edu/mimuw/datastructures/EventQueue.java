package pl.edu.mimuw.datastructures;

import pl.edu.mimuw.events.Event;

public interface EventQueue {
    void push(Event newEvent);

    void executeEarliestEvent();

    boolean isEmpty();
}