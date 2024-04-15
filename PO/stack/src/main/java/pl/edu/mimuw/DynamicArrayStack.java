package pl.edu.mimuw;

public class DynamicArrayStack extends AbstractStack {
    private final DynamicArray array;

    public DynamicArrayStack() {
        array = new DynamicArray();
    }

    public int getSize() {
        return array.getSize();
    }

    public boolean isEmpty() {
        return array.getSize() == 0;
    }

    public void push(int value) {
        array.add(value);
    }

    public int pop() {
        if (!isEmpty()) {
            return array.pop();
        }
        return -1;
    }

    public int peek() {
        if (!isEmpty()) {
            return array.read(array.getSize() - 1);
        }
        return -1;
    }
}
