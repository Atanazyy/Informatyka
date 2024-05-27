package pl.edu.mimuw.utilities;

public class Time {
    public static String displayTime(int time) {
        int hour = time / 60;
        String displayHour = Integer.toString(hour);
        if (hour < 10) {
            displayHour = STR."0\{displayHour}";
        }
        int minutes = time % 60;
        String displayMinutes = Integer.toString(minutes);
        if (minutes < 10) {
            displayMinutes = STR."0\{displayMinutes}";
        }
        return STR."\{displayHour}:\{displayMinutes}";
    }

    public static int covertTimeToMinutes(int hours, int minutes) {
        return hours * 60 + minutes;
    }
}