package pl.edu.mimuw;

import java.util.Scanner;

public class Main {

    public static void main(String[] args) {
        int n = (int)1e8;
        long sum = 0;
        boolean[] tab = new boolean[n + 1];
        for (int i = 2; i <= n; i++) {
            tab[i] = true;
        }
        for (int i = 2; i <= n; i++) {
            if (tab[i]) {
                for (int j = i + i; j <= n; j += i) {
                    tab[j] = false;
                }
                sum += i;
            }
        }
        System.out.println(sum);
    }
}
