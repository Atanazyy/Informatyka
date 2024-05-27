package pl.edu.mimuw;

import java.util.Map;

public class Main {

    public static void main(String[] args) {
        Map<Integer, String> map = new CollectionMap<>();
        map.put(1, "Kowalski");
        map.put(2, "Malinowski");
        map.put(3, "Zagłoba");
        map.put(4, "Trąbalski");
        map.put(5, "Karlsruhe");
        for(int i = 1; i <= 5; i++) {
            System.out.println(map.get(i));
        }
        System.out.println(map.containsKey(3));
        System.out.println(map.containsKey(10));
        System.out.println(map.containsValue("Karłowicz"));
        System.out.println(map.containsValue("Trąbalski"));
        System.out.println(map.entrySet());
        System.out.println(map.keySet());
        System.out.println(map.values());
        System.out.println(map.containsValue("Zagłoba"));
        System.out.println(map.size());
        map.remove(3);
        System.out.println(map.containsValue("Zagłoba"));
        System.out.println(map.size());
        map.putAll(map);
        Map<Integer, Object> map2 = new CollectionMap<>();
        map2.putAll(map);
        System.out.println(map2.entrySet());
        System.out.println(map.entrySet());
        System.out.println(map.equals(map2));
        System.out.println(map.remove(1));
        Map<Integer, Object> map3 = new CollectionMap<>();
        System.out.println(map3.equals(map));
        System.out.println(map);
        System.out.println(map.equals(map2));
    }
}
