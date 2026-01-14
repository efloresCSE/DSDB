#ifndef MPAIR_H
#define MPAIR_H
#include "mylib.h"

using namespace std;

// Forward declarations to fix -Wnon-template-friend:
template <typename K, typename V>
struct MPair;

template <typename K, typename V>
MPair<K, V> operator+(const MPair<K, V> &lhs, const MPair<K, V> &rhs);

template <typename K, typename V>
struct MPair
{
    K key;
    vector<V> value_list;

    MPair(const K &k = K())
    {
        key = k;
        value_list = vector<V>();
    }

    MPair(const K &k, const V &v)
    {
        (void)v; // fixes -Wunused-parameter
        key = k;
        value_list = vector<V>();
        // value_list.push_back(v);
    }

    MPair(const K &k, const vector<V> &vlist)
    {
        key = k;
        value_list = vector<V>();
        value_list = vlist;
    }

    friend ostream &operator<<(ostream &outs,
                               const MPair<K, V> &print_me)
    {
        for (size_t i = 0; i < print_me.value_list.size(); ++i) // fixes -Wsign-compare
        {
            outs << print_me.value_list[i] << " ";
        }
        return outs;
    }

    friend bool operator==(const MPair<K, V> &lhs,
                           const MPair<K, V> &rhs)
    {
        if (lhs.key == rhs.key)
            return true;
        return false;
    }
    friend bool operator!=(const MPair<K, V> &lhs,
                           const MPair<K, V> &rhs)
    {
        if (lhs.key != rhs.key)
            return true;
        return false;
    }

    friend bool operator<(const MPair<K, V> &lhs,
                          const MPair<K, V> &rhs)
    {
        if (lhs.key < rhs.key)
            return true;
        return false;
    }

    friend bool operator<=(const MPair<K, V> &lhs,
                           const MPair<K, V> &rhs)
    {
        if (lhs.key <= rhs.key)
            return true;
        return false;
    }

    friend bool operator>(const MPair<K, V> &lhs,
                          const MPair<K, V> &rhs)
    {
        if (lhs.key > rhs.key)
            return true;
        return false;
    }

    friend bool operator>=(const MPair<K, V> &lhs,
                           const MPair<K, V> &rhs)
    {
        if (lhs.key >= rhs.key)
            return true;
        return false;
    }

    // fixes -Wnon-template-friend (operator+ is a template now)
    friend MPair<K, V> operator+ <>(const MPair<K, V> &lhs,
                                    const MPair<K, V> &rhs);

    friend MPair<K, V> operator+=(const MPair<K, V> &lhs,
                                  const MPair<K, V> &rhs)
    {
        MPair<K, V> temp = lhs;
        temp.value_list += rhs.value_list[0];
        return temp;
    }
};

#endif // MPAIR_H
