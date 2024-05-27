package pl.edu.mimuw.datastructures;

import pl.edu.mimuw.Passenger;

public class DynamicArrayOfPassengers {
    private Passenger[] array;
    private int arraySize;
    private int numberOfElements;

    public DynamicArrayOfPassengers() {
        array = new Passenger[10];
        numberOfElements = 0;
        arraySize = 10;
    }

    public void push(Passenger newEvent) {
        if (numberOfElements >= arraySize) { // expanding array if needed
            Passenger[] newArray = new Passenger[arraySize * 2];
            System.arraycopy(array, 0, newArray, 0, numberOfElements);
            array = newArray;
            arraySize = 2 * arraySize;
        }
        array[numberOfElements] = newEvent;
        numberOfElements++;
    }

    public Passenger pop() {
        assert numberOfElements > 0;
        numberOfElements--;
        return array[numberOfElements];
    }

    public int size() {
        return numberOfElements;
    }

    public boolean isEmpty() {
        return numberOfElements == 0;
    }
}
