#ifndef BPLUSTREE_H
#define BPLUSTREE_H

#include "mylib.h"
#include "arrayfunctions.h"
using namespace std;

template <class T>
class BPlusTree
{
public:
    /*
     * *************************************************************
     *          N E S T E D     I T E R A T O R     C L A S S
     * *************************************************************
     */
    // This iterator is used to iterate through the childless nodes
    class Iterator
    {
    public:
        friend class BPlusTree;

        Iterator(BPlusTree<T> *_it = NULL, int _key_ptr = 0)
            : it(_it), key_ptr(_key_ptr) {}

        // overloaded dereference operator
        T operator*()
        {

            assert(key_ptr < it->data_count);

            // return data entry specified by key_ptr
            return it->data[key_ptr];
        }

        // overloaded incrementation operator
        Iterator operator++(int un_used)
        {
            (void)un_used; // warning fix: unused parameter
            Iterator temp;
            temp = *this;

            // if we haven't reached the end of data
            // container, add one to key_ptr
            if (key_ptr + 1 != it->data_count)
            {
                key_ptr++;
            }
            // move on to the next container,
            // and start at the beginning
            else
            {
                it = it->next;
                key_ptr = 0;
            }

            return temp;
        }

        // overloaded incrementation operator
        Iterator operator++()
        {
            // if we haven't reached the end of the
            // data container, add one to key_ptr
            if (key_ptr + 1 != it->data_count)
            {
                key_ptr++;
            }
            // move on to the next container
            // and start at the beginning
            else
            {
                it = it->next;
                key_ptr = 0;
            }

            return *this;
        }

        // overloaded equality operator
        friend bool operator==(const Iterator &lhs,
                               const Iterator &rhs)
        {
            // trees must be the same, and position of pointer must
            // be the same
            return (lhs.it == rhs.it && lhs.key_ptr == rhs.key_ptr);
        }

        // overlaoded inequality operator
        friend bool operator!=(const Iterator &lhs,
                               const Iterator &rhs)
        {
            // If the iterators are not equal based on the first condition
            if (lhs.it != rhs.it)
                return true;

            // If the iterators point to the same element, compare their keys
            if (lhs.key_ptr != rhs.key_ptr)
                return true;

            // Otherwise, the iterators are equal
            return false;
        }
        // check if pointer is NULL
        bool is_null() { return !it; }

    private:
        BPlusTree<T> *it;
        int key_ptr;
    };

    /*
     * *************************************************************
     *                      C O N S T R U C T O R
     * *************************************************************
     */
    BPlusTree(bool dups = true)
    {
        child_count = 0;
        data_count = 0;
        dups_ok = dups;
        next = NULL;
    }

    /*
     * *************************************************************
     *      B I G   T H R E E   (AND THEIR ASSOCIATED FUNCTIONS)
     * *************************************************************
     */
    // Destructor! Uses clear_Tree
    ~BPlusTree()
    {
        clear_tree();
    }

    // Copy constructor
    BPlusTree(const BPlusTree<T> &other)
    {
        // clear tree first
        clear_tree();
        copy_tree(other);
    }

    // overloaded assignment operator
    BPlusTree<T> &operator=(const BPlusTree<T> &RHS)
    {
        if (this != &RHS)
        {
            // Clean up the current object
            clear_tree();
            // Copy data from RHS
            copy_tree(RHS);
        }
        return *this; // Return *this for chaining
    }

    // postcondition: clears tree, used in destructor
    void clear_tree()
    {
        // go through all children
        for (int i = 0; i < child_count; i++)
        {
            // recursively call clear tree
            subset[i]->clear_tree();
            // delete the child and set it to NULL
            delete subset[i];
            subset[i] = NULL;
        }

        // set data count and child count of roots to zero
        data_count = 0;
        child_count = 0;
        return;
    }

    // copy_tree. Used in copy constructor
    // and overloaded assignment operator
    void copy_tree(const BPlusTree<T> &other)
    {
        BPlusTree<T> *temp = NULL;

        // we will use temp to keep our leafs linked
        copy_helper(other, temp);
    }

    // used to keep track of temp pointer that tracks our leafs
    void copy_helper(const BPlusTree<T> &other, BPlusTree<T> *&temp)
    {
        // handle the linked leafs
        if (other.is_leaf())
        {
            this->next = temp;
            temp = this;
        }

        // copy data from other
        for (int i = 0; i < other.data_count; ++i)
            data[i] = other.data[i];
        data_count = other.data_count;
        child_count = other.child_count;

        // recursively go through children (starting rightmost)
        for (int i = other.child_count - 1; i >= 0; i--)
        {
            subset[i] = new BPlusTree<T>;
            subset[i]->copy_helper(*other.subset[i], temp);
        }
    }

    /*
     * *************************************************************
     *              S E A R C H I N G  &  A C C E S S
     * *************************************************************
     */
    // true if entry can be found in the array
    bool contains(const T &entry) const
    {
        // reuturns first data in array that is greater than
        // or equal to entry. Will return size of array
        // if no data is larger than entry
        int i = first_ge(data, data_count, entry);

        // if i is less than size of array(not equal to) and
        // the data at position i is equal to entry. Then we found
        // our entry!!
        bool found = (i < data_count && data[i] == entry);

        // we found the entry!
        if (found)
            return true;
        else
        {
            // if not found and is leaf, return false
            if (is_leaf())
                return false;
            // if there are still children, keep searching
            else
                return subset[i]->contains(entry);
        }
    }

    // used when cout << map[2]; should crash if
    // no entry associated with key
    const T &get(const T &entry) const
    {
        const bool debug = false;
        (void)debug; // warning fix: unused variable
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf())
        {
            if (found)
                return data[i];
            else
                assert(found);
        }
        if (found) // inner node
            return subset[i + 1]->get(entry);
        // or just return true?
        else // not found yet...
            return subset[i]->get(entry);
    }
    // Used with non constant get
    T &get_existing(const T &entry)
    {
        const bool debug = false;
        (void)debug; // warning fix: unused variable
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf())
        {
            if (found)
                return data[i];
            else
                assert(found);
        }
        if (found) // inner node
            return subset[i + 1]->get_existing(entry);
        // or just return true?
        else // not found yet...
            return subset[i]->get_existing(entry);
    }

    // Postcondition: used when map[2] = 32.
    // insert entry in tree if not already present
    T &get(const T &entry)
    {
        // insert entry if entry is not already in BTREE
        const bool debug = false;
        (void)debug; // warning fix: unused variable
        if (!contains(entry))
            insert(entry);

        // return value after getting it
        return get_existing(entry);
    }

    /*
     * *************************************************************
     *                      C A P A C I T Y
     * *************************************************************
     */
    // Postcondition: returns size of the tree
    int size() const
    {
        // COUNTS THE NUMBER OF KEYS IN THE BTREE
        // traverse like print does
        int count = 0;
        if (is_leaf())
        {
            return data_count;
        }
        int k = data_count;
        for (int i = child_count - 1; i >= 0; --i)
        {
            count += subset[i]->size();
            if (k > 0)
            {
                count += 1;
                --k;
            }
        }
        return count;
    }

    // Postcondition: returns true if tree is empty
    bool empty() const
    {
        return data_count == 0;
    }

    /*
     * *************************************************************
     *                      I N S E R T
     * *************************************************************
     */
    void insert(const T &entry)
    {
        loose_insert(entry);
        if (data_count > MAXIMUM)
        {
            // shallow copy this to a temp
            BPlusTree *temp = new BPlusTree<T>;
            temp->shallow_copy(*this);

            // shallow clear this
            data_count = 0;
            child_count = 1;

            // The root should have no entries, and everything
            // else should be moved down one level.
            subset[0] = temp;

            // call fix excess
            fix_excess(0);
        }
    }

    /*
     * *************************************************************
     *                      R E M O V E
     * *************************************************************
     */

    // remove entry from the tree
    void remove(const T &entry)
    {
        loose_remove(entry);
        if (data_count == 0 && child_count == 1)
        {
            // fix the root of the entire tree so that it no
            // longer has zero entries
            // set a temp pointer that points to the only child
            BPlusTree *temp = new BPlusTree<T>;
            temp = subset[0];

            // copy all data and subsets to root
            copy_tree(*temp);

            // delete original child
            delete temp;
        }
    }
    /*
     * *************************************************************
     *                       O U T P U T
     * *************************************************************
     */
    // Post: print a readable version of the tree
    void print_tree(int level = 0, ostream &outs = cout) const
    {
        // Stopping case if there are no children
        if (is_leaf())
        {
            // output tree info
            for (int i = data_count - 1; i >= 0; --i)
                cout << setw(level * 7) << data[i] << endl;
        }
        else
        {
            // go through all children, starting at the rightmost child
            int j = data_count;
            for (int i = child_count - 1; i >= 0; --i)
            {
                subset[i]->print_tree(level + 1, outs);
                // Output parent data after children data
                if (j > 0)
                {
                    cout << setw(level * 7) << data[j - 1] << endl;
                    j--;
                }
            }
        }
    }
    friend ostream &operator<<(ostream &outs,
                               const BPlusTree<T> &print_me)
    {
        print_me.print_tree(0, outs);
        return outs;
    }

    /*
     * *************************************************************
     *                       I T E R A T O R S
     * *************************************************************
     */
    // Post: Iterator begins at lowest/leftmost node of tree
    Iterator begin()
    {
        return Iterator(get_smallest_node());
    }
    Iterator end()
    {
        return Iterator(NULL);
    }

    // return an iterator to this key. NULL if not there.
    Iterator find(const T &entry)
    {
        const bool debug = false;
        (void)debug; // warning fix: unused variable
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);
        if (is_leaf())
        {
            if (found)
                return Iterator(this, i);
            else
                assert(found);
        }
        if (found) // inner node
            return subset[i + 1]->find(entry);
        // or just return true?
        else // not found yet...
            return subset[i]->find(entry);
    }

    // returns amount of childless nodes
    int sizeIT()
    {
        int count = 0;
        if (data_count == 0 && child_count == 0 && next == NULL)
            return 0;
        for (BPlusTree<T>::Iterator it = begin();
             it != end(); it++)
            ++count;
        return count;
    }

    /*
     * *************************************************************
     *                  E X T R A  F U N C T I O N S
     * *************************************************************
     */
    // performs a shallow copy
    void shallow_copy(const BPlusTree<T> &other)
    {
        // copy data
        copy_array(data, other.data,
                   data_count, other.data_count);
        // copy children
        copy_array(subset, other.subset,
                   child_count, other.child_count);
    }

    // checks if BPLusTree is valid
    bool is_valid()
    {
        // check if data is sorted
        if (!checkSort(data, data_count))
            return false;
        // check if data count is not more than max
        if (data_count > MAXIMUM)
            return false;
        // check if leaf
        if (is_leaf())
            return true;

        // check rest of rules
        for (int i = 0; i < data_count; ++i)
        {
            if (data[i] <= subset[i]->data[subset[i]->data_count - 1])
                return false;
        }

        for (int i = 0; i < data_count; ++i)
        {
            if (data[i] > subset[i + 1]->data[0])
                return false;
        }

        for (int i = 0; i < data_count; ++i)
        {
            if (data[i] < subset[i]->data[0])
                return false;
            T smallest;
            subset[i + 1]->get_smallest(smallest);
            if (data[i] != smallest)
                return false;
        }

        // recursively go through subsets and verify their validity
        for (int i = 0; i < child_count; ++i)
        {
            if (!(subset[i]->is_valid()))
            {
                return false;
            }
        }

        return true;
    }

private:
    /*
     * *************************************************************
     *              P R I V A T E   V A R I A B L E S
     * *************************************************************
     */
    static const int MINIMUM = 1;
    static const int MAXIMUM = 2 * MINIMUM;

    // true if duplicate keys may be inserted
    bool dups_ok;

    // holds the keys
    T data[MAXIMUM + 1];

    // number of keys in data
    int data_count;

    // number of children
    int child_count;

    // every child of the root is also the root
    // of a smaller BPlusTree/BTree
    BPlusTree *subset[MAXIMUM + 2];

    // FOR BPLUS TREE ONLY
    // pointer that connects childless nodes
    // essentially a linked list
    BPlusTree *next;

    /*
     * *************************************************************
     *              P R I V A T E   F U N C T I O N S
     * *************************************************************
     */
    // returns true if nodes is childless
    bool is_leaf() const
    {
        return child_count == 0;
    }

    /*
     * *************************************************************
     *              U S E D     I N     I N S E R T
     * *************************************************************
     */
    // insert element functions
    // allows MAXIMUM+1 data elements in the root
    void loose_insert(const T &entry)
    {
        // look for entry in data [ ]
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        // four cases:
        if (found)
        {
            if (is_leaf())
            {
                // 1. found / leaf: call overloaded +
                if (dups_ok)
                    // call overloaded +
                    // if mmap, we will get
                    // mmap["is"] = {0,5,12}
                    // where ints are positions
                    data[i] += entry;
                else
                    return;
            }
            // 2. found / !leaf call subset[i+1]->loose_insert
            // and fix_excess(i+1)
            else
            {
                subset[i + 1]->loose_insert(entry);
                if (subset[i + 1]->data_count > MAXIMUM)
                {
                    fix_excess(i + 1);
                }
            }
        }
        else
        {
            // 3. !found / leaf insert entry at position data[i]
            if (is_leaf())
                insert_item(data, i, data_count, entry);
            // 4. !found / !leaf call subset[i]->loose_insert
            // and fix_excess(i)
            else
            {
                subset[i]->loose_insert(entry);
                if (subset[i]->data_count > MAXIMUM)
                {
                    fix_excess(i);
                }
            }
        }
    }

    // fix excess of data elements in child i
    void fix_excess(int i)
    {
        T temp;
        BPlusTree *rightChild = new BPlusTree<T>;

        // Add a new subset at location i + 1 of this node
        insert_item(subset, i + 1, child_count, rightChild);

        // split subset[i] (both the subset array and the data array)
        // and move half into subset[i+1]
        //(this is the subset we created in step 1.)
        split(subset[i]->data, subset[i]->data_count,
              subset[i + 1]->data, subset[i + 1]->data_count);
        split(subset[i]->subset, subset[i]->child_count,
              subset[i + 1]->subset, subset[i + 1]->child_count);

        // detach the last data item of subset[i] and
        // bring it and insert it into this node's data[]
        detach_item(subset[i]->data, subset[i]->data_count,
                    temp);
        insert_item(data, i, data_count, temp);

        // deal with pointers here
        if (subset[i]->is_leaf())
        {
            // trasnfer the middle entry to the right
            //(detached item is inserted into left of subset[i+1])
            insert_item(rightChild->data, 0,
                        rightChild->data_count, temp);

            // appropriatly link childless nodes
            subset[i + 1]->next = subset[i]->next;
            subset[i]->next = subset[i + 1];
        }
    }

    /*
     * *************************************************************
     *              U S E D     I N     R E M O V E
     * *************************************************************
     */
    // remove element functions:
    // allows MINIMUM-1 data elements in the root
    void loose_remove(const T &entry)
    {
        T trash;

        // Make a local variable, equal to the first index such that
        // data[i[ is not less than target. If there is no such index,
        // set i equal to data count, indicating that all of the entries
        // are less than the target
        int i = first_ge(data, data_count, entry);
        bool found = (i < data_count && data[i] == entry);

        // 4 cases
        if (found)
        {
            // if found and is leaf, delete item
            if (is_leaf())
                delete_item(data, i, data_count, trash);

            // if found and not leaf key is in inner subnode
            // must find smallest, kinda messy
            else
            {
                assert(i < child_count - 1);
                subset[i + 1]->loose_remove(entry);

                // if shortage, find entry in data or subset[i]->data,
                // or subset[i+1]->data and replace it with
                // subset[index + 1] smallest
                if (subset[i + 1]->data_count < MINIMUM)
                {
                    fix_shortage(i + 1);

                    // search for entry in data and if found replace it
                    // with smallest.
                    int j = first_ge(data, data_count, entry);
                    bool found2 = (j < data_count && data[j] == entry);
                    if (found2)
                        subset[j + 1]->get_smallest(data[j]);
                    // search for it in subset[i]
                    else if (i < child_count)
                    {
                        j = first_ge(subset[i]->data,
                                     subset[i]->data_count, entry);
                        found2 = (j < subset[i]->data_count &&
                                  subset[i]->data[j] == entry);
                        if (found2)
                            subset[i]->subset[j + 1]->get_smallest(subset[i]->data[j]);
                        // otherwise look for entry in subset[i+1]
                        else if (i + 1 < child_count)
                        {
                            j = first_ge(subset[i + 1]->data,
                                         subset[i + 1]->data_count, entry);
                            found2 = (j < subset[i + 1]->data_count &&
                                      subset[i + 1]->data[j] == entry);
                            if (found2)
                                subset[i + 1]->subset[j + 1]->get_smallest(subset[i + 1]->data[j]);
                        }
                    }
                }
                // no shortage, replace data[i] with subset[i+1]->smallest
                else
                    subset[i + 1]->get_smallest(data[i]);
            }
        }
        else
        {
            // if not found and is leaf, do nothing
            if (is_leaf())
            {
                return;
            }
            // if not found and is not leaf, keep looking in the tree
            else
            {
                subset[i]->loose_remove(entry);
                if (subset[i]->data_count < MINIMUM)
                    fix_shortage(i);
            }
        }
    }

    // fix shortage of data elements in child i
    BPlusTree<T> *fix_shortage(int i)
    {
        // four cases
        // 1a. Transfer an extra entry from subset[i-1]
        if (i > 0 && i < child_count &&
            subset[i - 1]->data_count > MINIMUM)
        {
            // Borrow from left node
            transfer_right(i - 1);
        }
        // 1b. Transfer an extra entry from subset[i+1]
        else if (i < child_count - 1 &&
                 subset[i + 1]->data_count > MINIMUM)
        {
            // Borrow from right node
            transfer_left(i + 1);
        }
        // Combine subset[i] with subset[i-1]
        else if (i > 0 &&
                 subset[i - 1]->data_count == MINIMUM)
        {
            // MERGE if there is a left child
            // merge with i
            merge_with_next_sub(i - 1);
        }
        else if (i < child_count - 1 &&
                 subset[i + 1]->data_count == MINIMUM)
        {
            // Merge child i with right child
            merge_with_next_sub(i);
        }
        return subset[i];
    }

    // Postcondition: if not leaf, BTree rotate left
    // if leaf, rotates and leaves a trace
    void transfer_left(int i)
    {
        if (subset[i]->is_leaf())
        {
            // rotate and leave a trace
            rotate_left(i);

            // set data[i-1] to first entry os subset[i]
            data[i - 1] = subset[i]->data[0];
        }
        // non leaf keyss, just like BPlusTree
        else
        {
            rotate_left(i);
        }
    }

    // Postcondition: if not leaf, BTree rotate right
    // if leaf, rotates and leaves a trace
    void transfer_right(int i)
    {
        if (subset[i]->is_leaf())
        {
            // rotate and leave a trace
            rotate_right(i);

            // set subset[i+1]->data[0] to data[i]
            subset[i + 1]->data[0] = data[i];
        }
        else
        {
            rotate_right(i);
        }
    }

    // BTree Functions
    void rotate_left(int i)
    {
        T store;
        BPlusTree *subTemp = new BPlusTree<T>;

        // transfer item data[i-1] tp the back of subset[i-1]
        delete_item(data, i - 1, data_count, store);
        attach_item(subset[i - 1]->data,
                    subset[i - 1]->data_count, store);

        // first item of subset[i]->data moves up to data to replace
        // data[i-1]
        delete_item(subset[i]->data, 0,
                    subset[i]->data_count, store);
        insert_item(data, i - 1, data_count, store);

        // transfer first child of subset[i] to the end of
        // subset[i-1]
        if (!subset[i]->is_leaf())
        {
            delete_item(subset[i]->subset, 0,
                        subset[i]->child_count, subTemp);
            attach_item(subset[i - 1]->subset,
                        subset[i - 1]->child_count, subTemp);
        }
    }

    void rotate_right(int i)
    {
        T store;
        BPlusTree *subTemp = new BPlusTree<T>;

        // transfer item data[i] to the front of subset[i+1]
        delete_item(data, i, data_count, store);
        insert_item(subset[i + 1]->data, 0,
                    subset[i + 1]->data_count, store);

        // Trasnfer last item subset[i] to data[i]
        detach_item(subset[i]->data, subset[i]->data_count, store);
        insert_item(data, i, data_count, store);

        // transfer last child of subset[i] to front of subset[i+1]
        if (!subset[i]->is_leaf())
        {
            detach_item(subset[i]->subset,
                        subset[i]->child_count, subTemp);
            insert_item(subset[i + 1]->subset, 0,
                        subset[i + 1]->child_count, subTemp);
        }
    }

    // follows BTree merge, if leaf does not bring item down
    // from data to end of subset[i] and links leaves.
    void merge_with_next_sub(int i)
    {
        if (subset[i]->is_leaf())
        {
            T store;
            BPlusTree *subTemp = new BPlusTree<T>;

            // delete but do not bring down
            delete_item(data, i, data_count, store);

            // merge data and children subset[i] with subset [i+1]
            merge(subset[i]->data, subset[i]->data_count,
                  subset[i + 1]->data, subset[i + 1]->data_count);
            merge(subset[i]->subset, subset[i]->child_count,
                  subset[i + 1]->subset, subset[i + 1]->child_count);

            // delete subset[i+1], store in subTemp
            delete_item(subset, i + 1, child_count, subTemp);

            // link the childless nodes
            // whatever subset[i+1] was pointing to, subset[i] is
            // now pointing to.
            subset[i]->next = subTemp->next;

            // then delete subTemp
            delete subTemp;
        }
        else
        {
            merge_with_next_sub_original(i);
        }
    }

    // merge from BTree
    void merge_with_next_sub_original(int i)
    {
        T store;
        BPlusTree *subTemp = new BPlusTree<T>;

        // transfer data[i] to end of subset[i]
        delete_item(data, i, data_count, store);
        attach_item(subset[i]->data, subset[i]->data_count, store);

        // merge subset[i] with subset [i+1]
        merge(subset[i]->data, subset[i]->data_count,
              subset[i + 1]->data, subset[i + 1]->data_count);
        merge(subset[i]->subset, subset[i]->child_count,
              subset[i + 1]->subset, subset[i + 1]->child_count);

        // delete subset[i+1], store in subTemp, then delete it
        delete_item(subset, i + 1, child_count, subTemp);
        delete subTemp;
    }

    /*
     * *************************************************************
     *              U S E D     I N     I T E R A T O R
     * *************************************************************
     */
    // return smallest node from tree
    BPlusTree<T> *get_smallest_node()
    {
        if (is_leaf())
        {
            // return pointer to this object
            return this;
        }
        else
        {
            // continue down leftmost children
            return subset[0]->get_smallest_node();
        }
    }

    /*
     * *************************************************************
     *              H E L P E R     F U N C T I O N S
     * *************************************************************
     */

    // remove the biggest child of this tree->entry
    void remove_biggest(T &entry)
    {
        if (is_leaf())
        {
            // find index of largest item, remove it,
            // place in entry
            delete_item(data, data_count - 1, data_count, entry);
        }
        else
        {
            // continue down rightmost subset
            subset[child_count - 1]->remove_biggest(entry);
            // check to fix shortages
            if (subset[child_count - 1]->data_count < MINIMUM)
                fix_shortage(child_count - 1);
        }
    }

    // entry := leftmost leaf
    void get_smallest(T &entry)
    {
        if (is_leaf())
        {
            // set entry to left most data entry
            entry = data[0];
        }
        else
        {
            // continue down leftmost children
            subset[0]->get_smallest(entry);
        }
    }

    // entry will be rightmost leaf in tree
    void get_biggest(T &entry)
    {
        if (is_leaf())
        {
            // set entry to last item in data
            entry = data[data_count - 1];
        }
        else
        {
            // continue through rightmost children
            subset[child_count - 1]->get_biggest(entry);
        }
    }
};
#endif // BPLUSTREE_H
