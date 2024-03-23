package pl.edu.mimuw;

import java.util.Arrays;

public class IntArray {
    final private int[] array;
    final private int arraySize;

    public IntArray(int newSize) {
        this.array = new int[newSize];
        this.arraySize = newSize;
    }

    public IntArray() {
        this(0);
    }

    public IntArray addElement(int newElement) {
        IntArray newArray = new IntArray(arraySize + 1);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        newArray.array[arraySize] = newElement;
        return newArray;
    }

    public IntArray removeElement() {
        if (arraySize == 0) {
            return this;
        }
        IntArray newArray = new IntArray(arraySize - 1);
        System.arraycopy(array, 0, newArray.array, 0, arraySize - 1);
        return newArray;
    }

    public IntArray changeElement(int index, int newValue) {
        if(index >= arraySize){
            return this;
        }
        IntArray newArray = new IntArray(arraySize);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        newArray.array[index] = newValue;
        return newArray;
    }

    public void printArray() {
        System.out.println(Arrays.toString(array));
    }

    public int readElement(int index) {
        if(index >= arraySize){
            return 0;
        }
        return array[index];
    }

    public int getArraySize() {
        return arraySize;
    }
}
