package pl.edu.mimuw;

public class Main {

    public static void main(String[] args) {
        IntArray array1 = new IntArray();
        array1 = array1.pushed(9);
        array1 = array1.unshifted(6);
        System.out.println(array1);

        int[] array = new int[]{0, 2, 4};
        IntArray array2 = new IntArray(3, array);
        array2 = array2.reversed();
        System.out.println(array2);

        IntArray array3 = array1.concat(array2);
        System.out.println(array3);
        System.out.println(array3.at(0));
        array3 = array3.with(0, 666);
        System.out.println(array3.at(0));
        System.out.println(array3.getLength());
        System.out.println(array3.indexOf());
        System.out.println(array3.lastIndexOf());
        System.out.println(array3.includes(666));
        System.out.println(array3.includes(-1));

        array3 = array3.popped().shifted();
        System.out.println(array3);

        array3 = array3.filled(0, 2, 999);
        System.out.println(array3);

        IntArray array4 = new IntArray(0, 0, 7);
        System.out.println(array4);
    }

}
