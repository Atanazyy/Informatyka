package cp2024.solution;

import cp2024.circuit.*;

import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.LinkedBlockingQueue;

import static cp2024.circuit.NodeType.LEAF;

public class ParallelCircuitValue implements CircuitValue {
    private final Circuit circuit;
    Thread calculatingThread;
    private boolean isSolved = false;
    private boolean circuitResult;
    private boolean wasStopped = false;
    private BlockingQueue<BlockingQueueElement> finalQueue;

    public ParallelCircuitValue(Circuit circuit, List<ParallelCircuitValue> parallelCircuitValues) {
        this.circuit = new Circuit(circuit.getRoot());
        parallelCircuitValues.add(this);
        finalQueue = new LinkedBlockingQueue<>();
        calculatingThread = new Thread(() -> startCalculations());
        calculatingThread.start();
    }

    private void startCalculations() {
        calculateNodeValue(circuit.getRoot(), 0, finalQueue);
    }

    private boolean getResultOrThrowException(boolean result, boolean isResultCalculated) throws ResultNotCalculatedException {
        if (isResultCalculated) {
            return result;
        }
        throw new ResultNotCalculatedException();
    }

    private boolean calculateResult(CircuitNode node, int numberOfChildren, boolean[] results, boolean[] isResultCalculated, int numberOfTrues, int numberOfFalses) throws ResultNotCalculatedException {
        switch (node.getType()) {
            case IF:
                try {
                    if (getResultOrThrowException(results[0], isResultCalculated[0])) {
                        return getResultOrThrowException(results[1], isResultCalculated[1]);
                    } else {
                        return getResultOrThrowException(results[2], isResultCalculated[2]);
                    }
                } catch (ResultNotCalculatedException __) {
                    if (getResultOrThrowException(results[1], isResultCalculated[1]) && getResultOrThrowException(results[2], isResultCalculated[2])) {
                        return true;
                    }
                    if (!getResultOrThrowException(results[1], isResultCalculated[1]) && !getResultOrThrowException(results[2], isResultCalculated[2])) {
                        return false;
                    }
                }
            case NOT:
                return !getResultOrThrowException(results[0], isResultCalculated[0]);
            case AND:
                if (numberOfFalses >= 1) {
                    return false;
                }
                if (numberOfTrues == numberOfChildren) {
                    return true;
                }
                throw new ResultNotCalculatedException();
            case OR:
                if (numberOfTrues >= 1) {
                    return true;
                }
                if (numberOfFalses == numberOfChildren) {
                    return false;
                }
                throw new ResultNotCalculatedException();
            case GT:
                if (numberOfTrues >= ((ThresholdNode) node).getThreshold() + 1) {
                    return true;
                }
                if (numberOfChildren - numberOfFalses < ((ThresholdNode) node).getThreshold() + 1) {
                    return false;
                }
                throw new ResultNotCalculatedException();
            case LT:
                if (numberOfTrues > ((ThresholdNode) node).getThreshold() - 1) {
                    return false;
                }
                if (numberOfChildren - numberOfFalses <= ((ThresholdNode) node).getThreshold() - 1) {
                    return true;
                }
                throw new ResultNotCalculatedException();
            case LEAF:
                throw new RuntimeException();
        }
        throw new ResultNotCalculatedException();
    }

    private void interruptAllThreads(Thread[] threads) {
        for (Thread thread : threads) {
            thread.interrupt();
        }
    }

    private boolean calculateNonLeafNode(CircuitNode node) throws InterruptedException {
        BlockingQueue<BlockingQueueElement> queue = new LinkedBlockingQueue<>();
        CircuitNode[] args = node.getArgs();
        Thread[] threads = new Thread[args.length];
        for (int i = 0; i < args.length; i++) {
            final int childNumber = i;
            Thread thread = new Thread(() -> calculateNodeValue(args[childNumber], childNumber, queue));
            threads[i] = thread;
        }
        for (Thread thread : threads) {
            thread.start();
        }
        boolean[] results = new boolean[args.length];
        boolean[] isResultCalculated = new boolean[args.length];
        int numberOfTrues = 0;
        int numberOfFalses = 0;
        for (int i = 0; i < args.length; i++) {
            try {
                BlockingQueueElement element = queue.take();
                if (element.value()) {
                    numberOfTrues++;
                } else {
                    numberOfFalses++;
                }
                results[element.nodeNumber()] = element.value();
                isResultCalculated[element.nodeNumber()] = true;
            } catch (InterruptedException e) {
                interruptAllThreads(threads);
                throw e;
            }
            try {
                boolean res = calculateResult(node, args.length, results, isResultCalculated, numberOfTrues, numberOfFalses);
                interruptAllThreads(threads);
                return res;
            } catch (ResultNotCalculatedException e) {
            }
        }
        throw new InterruptedException("Too many threads interrupted");
    }

    private void calculateNodeValue(CircuitNode node, int nodeNumber, BlockingQueue<BlockingQueueElement> queue) {
        if (node.getType() == LEAF) {
            try {
                queue.add(new ChildResult(nodeNumber, ((LeafNode) node).getValue()));
            } catch (InterruptedException e) {
                queue.add(new BrokenBlockingQueueElement(e));
            }
        } else {
            try {
                queue.add(new ChildResult(nodeNumber, calculateNonLeafNode(node)));
            } catch (InterruptedException e) {
                queue.add(new BrokenBlockingQueueElement(e));
            }
        }
    }

    @Override
    public synchronized boolean getValue() throws InterruptedException {
        if (isSolved) {
            return circuitResult;
        }
        if (wasStopped) {
            throw new InterruptedException("Solver was stopped");
        }
        else {
            try {
                circuitResult = finalQueue.take().value();
                isSolved = true;
            } finally {
                calculatingThread = null;
            }
        }
        return circuitResult;
    }

    public void stop() {
        if (isSolved) {
            return;
        }
        wasStopped = true;
        if (calculatingThread != null) {
            calculatingThread.interrupt();
        }
    }
}
