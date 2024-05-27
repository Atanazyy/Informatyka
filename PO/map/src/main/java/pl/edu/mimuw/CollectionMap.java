package pl.edu.mimuw;

import java.util.*;

public class CollectionMap<K, V> implements Map<K, V> {
    private final List<Map.Entry<K, V>> list;

    public CollectionMap() {
        list = new ArrayList<>();
    }

    @Override
    public void clear() {
        list.clear();
    }

    @Override
    public boolean containsKey(Object key) {
        for (Map.Entry<K, V> entry : list) {
            if (entry.getKey().equals(key)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public boolean containsValue(Object value) {
        for (Map.Entry<K, V> entry : list) {
            if (entry.getValue().equals(value)) {
                return true;
            }
        }
        return false;
    }

    @Override
    public Set<Map.Entry<K, V>> entrySet() {
        return new HashSet<>(list);
    }

    @Override
    public V get(Object key) {
        for (Map.Entry<K, V> entry : list) {
            if (entry.getKey().equals(key)) {
                return entry.getValue();
            }
        }
        return null;
    }

    @Override
    public boolean isEmpty() {
        return list.isEmpty();
    }

    @Override
    public V put(K key, V value) {
        for (Map.Entry<K, V> entry : list) {
            if (entry.getKey().equals(key)) {
                return entry.setValue(value);
            }
        }
        list.add(new AbstractMap.SimpleEntry<>(key, value));
        return value;
    }

    @Override
    public void putAll(Map<? extends K, ? extends V> m) {
        for (Map.Entry<? extends K, ? extends V> entry : m.entrySet()) {
            list.add(new AbstractMap.SimpleEntry<>(entry.getKey(), entry.getValue()));
        }
    }

    @Override
    public V remove(Object key) {
        for (Map.Entry<K, V> entry : list) {
            if (entry.getKey().equals(key)) {
                list.remove(new AbstractMap.SimpleEntry<>(entry.getKey(), entry.getValue()));
                return entry.getValue();
            }
        }
        return null;
    }

    @Override
    public int size() {
        return list.size();
    }

    @Override
    public Collection<V> values() {
        List<V> values = new ArrayList<>();
        for (Map.Entry<K, V> entry : list) {
            values.add(entry.getValue());
        }
        return values;
    }

    @Override
    public Set<K> keySet() {
        Set<K> keys = new HashSet<>();
        for (Map.Entry<K, V> entry : list) {
            keys.add(entry.getKey());
        }
        return keys;
    }

    @Override
    public boolean equals(Object obj) {
        if (obj instanceof Map) {
            if(obj.hashCode() != hashCode()) {
                return false;
            }
            return ((Map<?, ?>) obj).entrySet().equals(entrySet());
        }
        return false;
    }

    @Override
    public int hashCode() {
        return this.entrySet().hashCode();
    }

    @Override
    public String toString() {
        return entrySet().toString();
    }
}
