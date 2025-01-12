package cp2024.solution;

public interface BlockingQueueElement {
    int nodeNumber() throws InterruptedException;

    boolean value() throws InterruptedException;
}
