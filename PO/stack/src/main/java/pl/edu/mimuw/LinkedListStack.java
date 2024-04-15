package pl.edu.mimuw;

public class LinkedListStack extends AbstractStack {
    private LinkedList list;

    public LinkedListStack() {
        list = new LinkedList();
    }

    public int getSize() {
        return list.getSize();
    }

    public boolean isEmpty() {
        return list.getSize() == 0;
    }

    public void push(int value) {
        list = list.added(value);
    }

    public int pop() {
        if (!isEmpty()) {
            int top = list.read();
            list = list.removed();
            return top;
        }
        return -1;
    }

    public int peek() {
        if (!isEmpty()) {
            return list.read();
        }
        return -1;
    }
}
