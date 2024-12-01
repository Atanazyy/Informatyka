package cp2024.solution;

import cp2024.circuit.Circuit;
import cp2024.circuit.CircuitSolver;
import cp2024.circuit.CircuitValue;

import java.util.ArrayList;
import java.util.List;

public class ParallelCircuitSolver implements CircuitSolver {
    private final List<ParallelCircuitValue> parallelCircuitValues;
    private boolean isStopped = false;

    public ParallelCircuitSolver() {
        super();
        parallelCircuitValues = new ArrayList<>();
    }

    @Override
    public synchronized CircuitValue solve(Circuit c) {
        if (isStopped) {
            return new BrokenCircuitValue();
        } else {
            return new ParallelCircuitValue(c, parallelCircuitValues);
        }
    }

    @Override
    public synchronized void stop() {
        isStopped = true;
        for (ParallelCircuitValue parallelCircuitValue : parallelCircuitValues) {
            parallelCircuitValue.stop();
        }
    }
}
