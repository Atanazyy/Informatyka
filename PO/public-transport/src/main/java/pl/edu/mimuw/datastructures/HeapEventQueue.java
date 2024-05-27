package pl.edu.mimuw.datastructures;

import pl.edu.mimuw.events.Event;

public class HeapEventQueue implements EventQueue {
    private final DynamicArrayOfEvents array;
    private int size;
    private int numberOfEvents;

    public HeapEventQueue() {
        array = new DynamicArrayOfEvents();
        size = 0;
        numberOfEvents = 0;
    }

    private void fixDown(int index) { // shift event down to maintain heap property
        int leftChild = index * 2;
        int rightChild = leftChild + 1;
        int earliestEventIndex = index; // index of the earliest event from parent and children
        if (leftChild <= size && array.getElement(leftChild).isEarlier(array.getElement(earliestEventIndex))) {
            earliestEventIndex = leftChild;
        }
        if (rightChild <= size && array.getElement(rightChild).isEarlier(array.getElement(earliestEventIndex))) {
            earliestEventIndex = rightChild;
        }
        if (index != earliestEventIndex) {
            array.swapElements(index, earliestEventIndex);
            fixDown(earliestEventIndex);
        }
    }

    private void fixUp(int index) { // shift new event up to maintain heap property
        while (index > 1 && array.getElement(index).isEarlier(array.getElement(index / 2))) {
            array.swapElements(index, index / 2);
            index = index / 2;
        }
    }

    private Event top() {
        assert size > 0;
        return array.getElement(1);
    }

    private void pop() {
        assert size > 0;
        array.swapElements(1, size);
        size--;
        array.pop();
        fixDown(1);
    }

    @Override
    public void executeEarliestEvent() {
        Event earliestEvent = top();
        pop();
        earliestEvent.execute();
    }

    @Override
    public boolean isEmpty() {
        return size == 0;
    }

    @Override
    public void push(Event event) {
        if (event == null) {
            return;
        }
        event.setNumberOfEvent(numberOfEvents++);
        size++;
        array.push(event);
        fixUp(size);
    }

}
