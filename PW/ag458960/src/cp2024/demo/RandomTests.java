package cp2024.demo;

import cp2024.circuit.*;
import cp2024.solution.ParallelCircuitSolver;

import java.time.Duration;
import java.util.Random;
import java.util.concurrent.*;

public class RandomTests {
    public static void main(String[] args) throws InterruptedException {
        CircuitSolver parallelSolver = new ParallelCircuitSolver();
        CircuitSolver sequentialSolver = new SequentialSolver();

        int testCount = 1000; // Liczba losowych testów
        int timeLimitMillis = 20000; // Limit czasu na test (3 sekundy)
        Random random = new Random();

        ExecutorService executor = Executors.newSingleThreadExecutor();

        int consistentTests = 0; // Licznik zgodnych wyników
        int inconsistentTests = 0; // Licznik niezgodnych wyników

        for (int i = 1; i <= testCount; i++) {
            // Generowanie losowego obwodu
            Circuit randomCircuit = generateRandomCircuit(random, 2); // max głębokość 4

            // Generowanie schematu obwodu
            System.out.println("Test #" + i);
            System.out.println("Schemat obwodu:");
            System.out.println(generateCircuitScheme(randomCircuit.getRoot(), 0));

            // Wyniki obu solverów
            CircuitValue sequentialResult = null;
            CircuitValue parallelResult = null;

            // Test dla SequentialSolver
            System.out.println("Testing SequentialSolver...");
            sequentialResult = executeWithTimeLimit(executor, sequentialSolver, randomCircuit, timeLimitMillis);

            // Test dla ParallelCircuitSolver
            System.out.println("Testing ParallelCircuitSolver...");
            parallelResult = executeWithTimeLimit(executor, parallelSolver, randomCircuit, timeLimitMillis);

            // Porównanie wyników
            if (sequentialResult != null && parallelResult != null && sequentialResult.getValue() == parallelResult.getValue()) {
                System.out.println("Wyniki zgodne.");
                consistentTests++;
            } else {
                System.out.println("Wyniki NIEZGODNE.");
                inconsistentTests++;
            }

            System.out.println("=====");
        }

        parallelSolver.stop();
        sequentialSolver.stop();
        executor.shutdown();

        // Podsumowanie wyników
        System.out.println("Podsumowanie testów:");
        System.out.println("Liczba zgodnych testów: " + consistentTests);
        System.out.println("Liczba niezgodnych testów: " + inconsistentTests);
        System.out.println("Test zakończony.");
    }

    // Funkcja generująca losowy obwód
    private static Circuit generateRandomCircuit(Random random, int maxDepth) {
        if (maxDepth == 0) {
            // Węzeł liścia
            boolean value = random.nextBoolean();
            Duration delay = random.nextBoolean() ? Duration.ofMillis(random.nextInt(100)) : Duration.ZERO;
            return new Circuit(CircuitNode.mk(value, delay));
        }

        NodeType[] nodeTypes = NodeType.values();
        NodeType type = nodeTypes[random.nextInt(nodeTypes.length)];

        // Obsługa specjalnych przypadków dla liści
        if (type == NodeType.LEAF) {
            boolean value = random.nextBoolean();
            Duration delay = random.nextBoolean() ? Duration.ofMillis(random.nextInt(100)) : Duration.ZERO;
            return new Circuit(CircuitNode.mk(value, delay));
        }

        int argCount;
        switch (type) {
            case GT, LT -> argCount = random.nextInt(4) + 2; // 2-5 argumentów
            case AND, OR -> argCount = random.nextInt(3) + 2; // 2-4 argumenty
            case NOT -> argCount = 1; // 1 argument
            case IF -> argCount = 3; // 3 argumenty
            default -> throw new IllegalArgumentException("Nieobsługiwany typ NodeType: " + type);
        }

        CircuitNode[] args = new CircuitNode[argCount];
        for (int i = 0; i < argCount; i++) {
            args[i] = generateRandomCircuit(random, maxDepth - 1).getRoot();
        }

        if (type == NodeType.GT || type == NodeType.LT) {
            int threshold = random.nextInt(argCount) + 1; // Threshold w zakresie 1-argCount
            return new Circuit(CircuitNode.mk(type, threshold, args));
        } else {
            return new Circuit(CircuitNode.mk(type, args));
        }
    }

    // Funkcja generująca schemat obwodu jako tekst
    private static String generateCircuitScheme(CircuitNode node, int depth) throws InterruptedException {
        StringBuilder sb = new StringBuilder();

        // Wcięcie zależne od głębokości
        sb.append("  ".repeat(depth));
        sb.append("- ");

        if (node instanceof SleepyLeafNode sleepyLeaf) {
            sb.append("LEAF (value: ").append(sleepyLeaf.getValue()).append(")");
        } else if (node instanceof ThresholdNode thresholdNode) {
            sb.append("THRESHOLD (type: ").append(thresholdNode.getType()).append(", threshold: ").append(thresholdNode.getThreshold()).append(")");
        } else if (node instanceof SimpleNode simpleNode) {
            sb.append("NODE (type: ").append(simpleNode.getType()).append(")");
        }

        sb.append("\n");

        try {
            for (CircuitNode child : node.getArgs()) {
                sb.append(generateCircuitScheme(child, depth + 1));
            }
        } catch (InterruptedException e) {
            sb.append("  ".repeat(depth + 1)).append("(Interrupted while retrieving children)\n");
        }

        return sb.toString();
    }

    // Wykonanie testu z limitem czasu
    private static CircuitValue executeWithTimeLimit(ExecutorService executor, CircuitSolver solver, Circuit circuit, int timeLimitMillis) {
        Future<CircuitValue> future = executor.submit(() -> solver.solve(circuit));
        try {
            long startTime = System.nanoTime();
            CircuitValue value = future.get(timeLimitMillis, TimeUnit.MILLISECONDS);
            long endTime = System.nanoTime();
            System.out.println("Wynik: " + value.getValue() + " (czas: " + (endTime - startTime) / 1_000_000 + " ms)");
            return value;
        } catch (TimeoutException e) {
            System.out.println("Test przerwany: przekroczono limit " + timeLimitMillis + " ms");
            future.cancel(true); // Przerwij wątek rozwiązujący test
        } catch (Exception e) {
            System.out.println("Błąd podczas rozwiązywania testu: " + e.getMessage());
        }
        return null;
    }
}
