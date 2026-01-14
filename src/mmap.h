#ifndef MMAP_H
#define MMAP_H

#include "mylib.h"
#include "pair.h"
#include "mpair.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class MMap
{
public:
    typedef BPlusTree<MPair<K, V>> map_base;
    /*
     * *************************************************************
     *          N E S T E D     I T E R A T O R     C L A S S
     * *************************************************************
     */
    class Iterator
    {
    public:
        friend class MMap;

        // constructor for iterator
        Iterator(typename map_base::Iterator it)
            : _it(it) {}

        // overloaded dereference operator
        MPair<K, V> operator*()
        {
            return *_it;
        }

        // overloaded incrementation operator
        Iterator operator++(int un_used)
        {
            (void)un_used; // silence -Wunused-parameter
            return ++_it;
        }

        // overloaded incrementation operator
        Iterator operator++()
        {
            return _it++;
        }

        // overloaded equality operator
        friend bool operator==(const Iterator &lhs,
                               const Iterator &rhs)
        {
            return (lhs._it == rhs._it);
        }

        // overlaoded inequality operator
        friend bool operator!=(const Iterator &lhs,
                               const Iterator &rhs)
        {
            return (lhs._it != rhs._it);
        }

    private:
        typename map_base::Iterator _it;
    };
    /*
     * *************************************************************
     *                  C O N S T R U C T O R
     * *************************************************************
     */
    MMap()
    {
        mmap = BPlusTree<MPair<K, V>>();
    }

    ~MMap()
    {
        mmap.clear_tree();
    }

    // Copy constructor
    MMap(const MMap &other)
    {
        // clear tree first
        mmap.clear_tree();
        mmap.copy_tree(other.mmap);
    }

    MMap &operator=(const MMap &RHS)
    {
        if (this != &RHS)
        {
            // Clean up the current object
            mmap.clear_tree();
            // Copy the content from RHS
            mmap.copy_tree(RHS.mmap);
        }
        return *this; // Return *this for chaining
    }

    /*
     * *************************************************************
     *                      C A P A C I T Y
     * *************************************************************
     */

    void clearMap()
    {
        mmap.clear_tree();
    }

    // Postcondition: Returns size of mmap
    int size() const { return mmap.size(); }
    // Postcondition: reuturns true if mmap is empty
    //(if key_count is equal to 0)
    bool empty() const { return mmap.size() == 0; }

    /*
     * *************************************************************
     *              S E A R C H I N G  &  A C C E S S
     * *************************************************************
     */

    // const overloaded access function to return value
    //(if value is not found in map, should crash?)
    // cout << test[0];
    const vector<V> &operator[](const K &key) const
    {
        return mmap.get(MPair<K, V>(key, V())).value_list;
    }

    // Postcondition: Returns value from key
    // we can change values using this one
    // test[0] += 12;
    vector<V> &operator[](const K &key)
    {
        return mmap.get(MPair<K, V>(key, V())).value_list;
    }

    bool contains(const K &key) const
    {
        return mmap.contains(key);
    }
    vector<V> &get(const K &key)
    {
        return mmap.get(key);
    }

    /*
     * *************************************************************
     *              M O D I F I E R     F U N C T I O N S
     * *************************************************************
     */
    // insert item to tree
    void insert(const K &k, const V &v)
    {
        mmap.insert(MPair<K, V>(k, v));
    }

    // erases an item from BPlusTree
    void erase(const K &key)
    {
        mmap.remove(key);
    }
    // clears all items from tree
    void clear()
    {
        mmap.clear_tree();
    }

    /*
     * *************************************************************
     *                       O U T P U T
     * *************************************************************
     */
    friend ostream &operator<<(ostream &outs,
                               const MMap<K, V> &print_me)
    {
        outs << print_me.mmap << endl;
        return outs;
    }

    /*
     * *************************************************************
     *                      I T E R A T O R S
     * *************************************************************
     */
    // begins at the left most node
    Iterator begin()
    {
        return Iterator(mmap.begin());
    }
    Iterator end()
    {
        return Iterator(NULL);
    }

    // returns iterator for given key
    Iterator lower_bound(string startingKey)
    {
        return mmap.find(startingKey);
    }

    // returns iterator for given key
    Iterator upper_bound(string endingKey)
    {
        return ++mmap.find(endingKey);
    }

    /*
     * *************************************************************
     *                  E X T R A  F U N C T I O N S
     * *************************************************************
     */
    // Postcondition: manually creates a mmap object
    void make()
    {
        vector<int> hey = {1, 5, 8};
        vector<int> hi = {11, 15, 18};

        MPair<int, int> test(0, hey);
        mmap.insert(test);

        MPair<int, int> test2(1, hi);
        mmap.insert(test2);
    }

    bool is_valid() { return mmap.is_valid(); };

private:
    // our mmap variable made of a BPlusTree
    BPlusTree<MPair<K, V>> mmap;
};
#endif // MMAP_H
