#ifndef PAIR_H
#define PAIR_H
#include "mylib.h"

using namespace std;

template <typename K, typename V>
struct Pair
{
    K key;
    V value;

    // Constructor
    Pair(const K &k = K(), const V &v = V()) : key(k), value(v) {}

    // FIX: explicitly declare copy/move operations to avoid -Wdeprecated-copy
    Pair(const Pair<K, V> &other) = default;
    Pair(Pair<K, V> &&other) = default;
    Pair<K, V> &operator=(Pair<K, V> &&rhs) = default;

    // Overloaded insertion operator
    friend ostream &operator<<(ostream &outs,
                               const Pair<K, V> &print_me)
    {
        outs << "[" << print_me.key << ":" << print_me.value << "]";
        return outs;
    }

    // overloaded comparison operator
    friend bool operator==(const Pair<K, V> &lhs,
                           const Pair<K, V> &rhs)
    {
        if (lhs.key == rhs.key)
            return true;
        return false;
    }

    // overloaded comparison operator
    friend bool operator<(const Pair<K, V> &lhs,
                          const Pair<K, V> &rhs)
    {
        if (lhs.key < rhs.key)
            return true;
        return false;
    }

    // overloaded comparison operator
    friend bool operator>(const Pair<K, V> &lhs,
                          const Pair<K, V> &rhs)
    {
        if (lhs.key > rhs.key)
            return true;
        return false;
    }

    // overloaded comparison operator
    friend bool operator<=(const Pair<K, V> &lhs,
                           const Pair<K, V> &rhs)
    {
        if (lhs.key <= rhs.key)
            return true;
        return false;
    }

    // overloaded comparison operator
    friend bool operator>=(const Pair<K, V> &lhs,
                           const Pair<K, V> &rhs)
    {
        if (lhs.key >= rhs.key)
            return true;
        return false;
    }

    // overloaded addition operator
    friend Pair<K, V> operator+(const Pair<K, V> &lhs,
                                const Pair<K, V> &rhs)
    {
        Pair<K, V> temp = lhs;
        temp.value = rhs.value;

        return temp;
    }

    // overloaded addition operator
    friend Pair<K, V> operator+=(const Pair<K, V> &lhs,
                                 const Pair<K, V> &rhs)
    {
        Pair<K, V> temp = lhs;
        temp.value = rhs.value;

        return temp;
    }

    // overloaded assignment operator
    Pair<K, V> &operator=(const Pair<K, V> &rhs)
    {
        if (this != &rhs)
        {
            key = rhs.key;
            value = rhs.value;
        }
        return *this; // Return *this for chaining
    }
};

#endif // PAIR_H
