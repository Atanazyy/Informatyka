package cp2024.solution;

public class BrokenBlockingQueueElement implements BlockingQueueElement {
    private final InterruptedException interruptedException;

    public BrokenBlockingQueueElement(InterruptedException interruptedException) {
        this.interruptedException = interruptedException;
    }

    @Override
    public int nodeNumber() throws InterruptedException {
        throw interruptedException;
    }

    @Override
    public boolean value() throws InterruptedException {
        throw interruptedException;
    }
}
