package pl.edu.mimuw.datastructures;

import pl.edu.mimuw.events.Event;

public class DynamicArrayOfEvents { // Dynamic array indexed from 1
    private Event[] array;
    private int arraySize;
    private int numberOfElements;

    public DynamicArrayOfEvents() {
        array = new Event[10];
        numberOfElements = 0;
        arraySize = 10;
    }

    public Event getElement(int index) {
        return array[index - 1];
    }

    public void push(Event newEvent) {
        if (numberOfElements >= arraySize) { // expanding array if needed
            Event[] newArray = new Event[arraySize * 2];
            System.arraycopy(array, 0, newArray, 0, numberOfElements);
            array = newArray;
            arraySize = 2 * arraySize;
        }
        array[numberOfElements] = newEvent;
        numberOfElements++;
    }

    public void swapElements(int index1, int index2) {
        Event temp = array[index1 - 1];
        array[index1 - 1] = array[index2 - 1];
        array[index2 - 1] = temp;
    }

    public void pop() {
        assert numberOfElements > 0;
        numberOfElements--;
    }
}
