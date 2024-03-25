package pl.edu.mimuw;

import java.util.Arrays;
import java.util.Objects;

import static java.lang.Math.min;

public class IntArray {
    final private int[] array;
    final private int arraySize;

    public IntArray(int newSize, int[] newArray) {
        this.arraySize = newSize;
        this.array = new int[newSize];
        System.arraycopy(newArray, 0, array, 0, newSize);
    }

    public IntArray(int newSize) {
        this.array = new int[newSize];
        this.arraySize = newSize;
    }

    public IntArray() {
        this(0);
    }

    public IntArray(int... args) {
        this.arraySize = args.length;
        this.array = args;
    }

    public IntArray pushed(int newElement) {
        IntArray newArray = new IntArray(arraySize + 1);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        newArray.array[arraySize] = newElement;
        return newArray;
    }

    public IntArray unshifted(int newElement) {
        IntArray newArray = new IntArray(arraySize + 1);
        newArray.array[0] = newElement;
        System.arraycopy(array, 0, newArray.array, 1, arraySize);
        return newArray;
    }

    public IntArray popped() {
        if (arraySize == 0) {
            return this;
        }
        IntArray newArray = new IntArray(arraySize - 1);
        System.arraycopy(array, 0, newArray.array, 0, arraySize - 1);
        return newArray;
    }

    public IntArray shifted() {
        if (arraySize == 0) {
            return this;
        }
        IntArray newArray = new IntArray(arraySize - 1);
        System.arraycopy(array, 1, newArray.array, 0, arraySize - 1);
        return newArray;
    }

    public IntArray with(int index, int newValue) {
        if (index >= arraySize) {
            return this;
        }
        IntArray newArray = new IntArray(arraySize);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        newArray.array[index] = newValue;
        return newArray;
    }

    public IntArray filled(int start, int end, int value) {
        IntArray newArray = new IntArray(arraySize);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        for (int i = start; i < min(arraySize - 1, end); i++) {
            newArray.array[i] = value;
        }
        return newArray;
    }

    public IntArray reversed() {
        IntArray newArray = new IntArray(arraySize);
        int i = 0;
        int j = arraySize - 1;
        while (i <= j) {
            newArray.array[i] = array[j];
            newArray.array[j] = array[i];
            i++;
            j--;
        }
        return newArray;
    }

    public IntArray concat(IntArray secondArray) {
        IntArray newArray = new IntArray(arraySize + secondArray.arraySize);
        System.arraycopy(array, 0, newArray.array, 0, arraySize);
        System.arraycopy(secondArray.array, 0, newArray.array, arraySize, secondArray.arraySize);
        return newArray;
    }

    public boolean includes(int value) {
        for (int i = 0; i < arraySize; i++) {
            if (array[i] == value) {
                return true;
            }
        }
        return false;
    }

    public int indexOf() {
        return at(0);
    }

    public int lastIndexOf() {
        return at(arraySize - 1);
    }

    public int at(int index) {
        if (index >= arraySize || index < 0) {
            return 0;
        }
        return array[index];
    }

    public int getLength() {
        return arraySize;
    }

    @Override
    public String toString() {
        StringBuilder result = new StringBuilder();
        result.append("IntArray{");
        for (int i = 0; i < arraySize; i++) {
            result.append(array[i]);
            result.append(", ");
        }
        result.append("length = ");
        result.append(arraySize);
        result.append("}");
        return result.toString();
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        IntArray array1 = (IntArray) o;
        if (arraySize != array1.arraySize) {
            return false;
        }
        for (int i = 0; i < arraySize; i++) {
            if (array1.array[i] != array[i]) {
                return false;
            }
        }
        return true;
    }

    @Override
    public int hashCode() {
        int result = Objects.hash(arraySize);
        result = 31 * result + Arrays.hashCode(array);
        return result;
    }
}
