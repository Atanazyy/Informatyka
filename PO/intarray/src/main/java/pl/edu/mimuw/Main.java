package pl.edu.mimuw;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        IntArray testArray = new IntArray();
        Scanner scanner = new Scanner(System.in);
        while (true) {
            int nextOperation = scanner.nextInt();
            switch (nextOperation) {
                case 1: {
                    int newValue = scanner.nextInt();
                    testArray = testArray.addElement(newValue);
                    break;
                }
                case 2: {
                    testArray = testArray.removeElement();
                    break;
                }
                case 3: {
                    int index = scanner.nextInt();
                    int newValue = scanner.nextInt();
                    testArray = testArray.changeElement(index, newValue);
                    break;
                }
                case 4: {
                    int index = scanner.nextInt();
                    System.out.println(testArray.readElement(index));
                    break;
                }
                case 5: {
                    System.out.println(testArray.getArraySize());
                    break;
                }
                case 6: {
                    testArray.printArray();
                    break;
                }
            }
        }
    }
}

/*
    1 - add element
    2 - remove element
    3 - change value at index
    4 - read from index
    5 - get length
    6 - print array
 */