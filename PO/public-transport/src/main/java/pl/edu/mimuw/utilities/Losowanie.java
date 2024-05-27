package pl.edu.mimuw.utilities;

import java.util.Random;

public class Losowanie {
    public static int losuj(int dolna, int gorna) {
        Random random = new Random();
        return random.nextInt(gorna + 1 - dolna) + dolna;
    }
}
