package pl.edu.mimuw;

public class Main {
    public static void main(String[] args) {
        /*AbstractStack stackL = new LinkedListStack();
        System.out.println(stackL.isEmpty());
        for (int i = 0; i < 100; i++) {
            stackL.push(i);
        }
        for (int i = 0; i < 3; i++) {
            System.out.println(stackL.peek());
            System.out.println(stackL.pop());
            System.out.println(stackL.getSize());
            System.out.println(stackL.isEmpty());
        }

        AbstractStack stackA = new LinkedListStack();
        System.out.println(stackA.isEmpty());
        for (int i = 0; i < 100; i++) {
            stackA.push(i);
        }
        for (int i = 0; i < 3; i++) {
            System.out.println(stackA.peek());
            System.out.println(stackA.pop());
            System.out.println(stackA.getSize());
            System.out.println(stackA.isEmpty());
        }*/

        ////////
        int n = 10 * 1000 * 1000;

        long start = System.currentTimeMillis();
        AbstractStack stackLT = new LinkedListStack();
        for (int i = 0; i < n; i++) {
            stackLT.push(i);
        }
        long finish = System.currentTimeMillis();
        System.out.println(finish - start);

        start = System.currentTimeMillis();
        AbstractStack stackAT = new DynamicArrayStack();
        for (int i = 0; i < n; i++) {
            stackAT.push(i);
        }
        finish = System.currentTimeMillis();
        System.out.println(finish - start);

        start = System.currentTimeMillis();
        for (int i = 0; i < n; i++) {
            stackLT.peek();
            stackLT.pop();
            stackLT.getSize();
            stackLT.isEmpty();
        }
        finish = System.currentTimeMillis();
        System.out.println(finish - start);

        start = System.currentTimeMillis();
        for (int i = 0; i < n; i++) {
            stackAT.peek();
            stackAT.pop();
            stackAT.getSize();
            stackAT.isEmpty();
        }
        finish = System.currentTimeMillis();
        System.out.println(finish - start);
    }
}