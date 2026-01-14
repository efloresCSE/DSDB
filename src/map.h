#ifndef MAP_H
#define MAP_H

#include "mylib.h"
#include "pair.h"
#include "bplustree.h"

using namespace std;

template <typename K, typename V>
class Map
{
public:

    typedef BPlusTree<Pair<K, V> > map_base;
/*
 * *************************************************************
 *          N E S T E D     I T E R A T O R     C L A S S
 * *************************************************************
*/
    class Iterator
    {
    public:
            friend class Map;

            //constructor for iterator
            Iterator(typename map_base::Iterator it)
                :_it(it){}

            //overloaded dereference operator
            Pair<K, V> operator *()
            {
                return *_it;
            }

            //overloaded incrementation operator
            Iterator operator++(int un_used)
            {
               return ++_it;
            }

            //overloaded incrementation operator
            Iterator operator++()
            {
                return _it++;
            }

            //overloaded equality operator
            friend bool operator ==(const Iterator& lhs,
                                    const Iterator& rhs)
            {
                return(lhs._it == rhs._it);
            }

            //overlaoded inequality operator
            friend bool operator !=(const Iterator& lhs,
                                    const Iterator& rhs)
            {
                return(lhs._it != rhs._it);
            }
        private:
            typename map_base::Iterator _it;
        };


/*
 * *************************************************************
 *                  C O N S T R U C T O R
 * *************************************************************
*/
    Map()
    {
        key_count = 0;
        map = BPlusTree<Pair<K,V> >();
    }

/*
 * *************************************************************
 *                      C A P A C I T Y
 * *************************************************************
*/

    //Postcondition: Returns size of map key_count
    int size() const {return key_count;}

    //Postcondition: reuturns true if map is empty
    //(if key_count is equal to 0)
    bool empty() const {return key_count == 0;}

/*
 * *************************************************************
 *              S E A R C H I N G  &  A C C E S S
 * *************************************************************
*/

    //const overloaded access function to return value
    const V& operator[](const K& key) const
    {
         return map.get(Pair<K, V>(key, V())).value;
    }

    //Postcondition: Returns value from key
    //we can change values using this one
    //test[0] = 12;
    V& operator[](const K& key)
    {
        return map.get(Pair<K, V>(key, V())).value;
    }

    V get(const K& key)
    {
        return map.get(key);
    }
    bool contains(const Pair<K, V>& target) const
    {
        return map.contains(target);
    }


/*
 * *************************************************************
 *              M O D I F I E R     F U N C T I O N S
 * *************************************************************
*/

    //Postcondition: Inserts a Pair into our map
    void insert(const K& k, const V& v)
    {
        map.insert(Pair<K,V>(k, v));
    }

    void erase(const K& key)
    {
        map.remove(key);
    }

    void clear()
    {
        map.clear_tree();
    }

/*
 * *************************************************************
 *                       O U T P U T
 * *************************************************************
*/
    friend ostream& operator<<(ostream& outs,
                               const Map<K, V>& print_me)
    {
        outs<<print_me.map<<endl;
        return outs;
    }
/*
 * *************************************************************
 *                      I T E R A T O R S
 * *************************************************************
*/
    Iterator begin()
    {
        return Iterator(map.begin());
    }
    Iterator end()
    {
        return Iterator(NULL);
    }

    Iterator lower_bound(string startingKey)
    {
        return map.find(startingKey);
    }

    Iterator upper_bound(string endingKey)
    {
        return ++map.find(endingKey);
    }
/*
 * *************************************************************
 *                  E X T R A  F U N C T I O N S
 * *************************************************************
*/
    //Postcondition: manually creates a map object
    void make()
    {
        Pair<int, int> test(0, 5);
        map.insert(test);
        key_count++;

        Pair<int, int> test2(1, 12);
        map.insert(test2);
        key_count++;
    }

    bool is_valid(){return map.is_valid();}

private:
    //amount of keys in map
    int key_count;
    //out map variable made of a BTree
    BPlusTree<Pair<K, V> > map;

};
#endif // MAP_H
