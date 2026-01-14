#ifndef ARRAYFUNCTIONS_H
#define ARRAYFUNCTIONS_H

#include "mylib.h"
#include "random.h"

using namespace std;

/*
 * *************************************************************
 *          F U N C T I O N     D E C L A R A T I O N S
 * *************************************************************
 */
// Postcondition: return the larger of the two items
template <class T>
T maximal(const T &a, const T &b);

// Postcondition: swaps the two items
template <class T>
void swap(T &a, T &b);

// Postcondition: returns index of the largest item in the data
template <class T>
int index_of_maximal(T data[], int n);

// Postconditon: insert entry into the sorted array data with length n
template <class T>
void ordered_insert(T data[], int &n, T entry);

// Postcondition: return the first element in data that is
// not less than entry
template <class T>
int first_ge(const T data[], int n, const T &entry);

// Postcondition: append entry to the right of data
template <class T>
void attach_item(T data[], int &n, const T &entry);

// Postcondition: insert entry at index i in data
template <class T>
void insert_item(T data[], int i, int &n, T entry);

// Postcondition: remove the last element in data and place
// it in entry
template <class T>
void detach_item(T data[], int &n, T &entry);

// Postcondition: delete item at index i and place it in entry
template <class T>
void delete_item(T data[], int i, int &n, T &entry);

// Postcondition: append data 2 to the right of data 1
template <class T>
void merge(T data1[], int &n1, T data2[], int &n2);

// move n/2 elements from the right of data 1
template <class T>
void split(T data1[], int &n1, T data2[], int &n2);

// Postcondition: copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[],
                int &dest_size, int src_size);

// Postcondition: print array data
template <class T>
void print_array(const T data[], int n, int pos = -1);

//-------------- Vector Extra operators: ---------------------

// Extra operators for Vectors
// Postcondition: print vector list
template <typename T>
ostream &operator<<(ostream &outs, const vector<T> &list);

// Postcondition: list.push_back addme
template <typename T>
vector<T> &operator+=(vector<T> &list, const T &addme);

//-------------- Extra Functions: ---------------------
// shuffle elements of a[]
template <typename T>
void shuffle(T a[], unsigned int size);

// checks if array is sorted
template <class T>
bool checkSort(T data[], int size);

/*
 * *************************************************************
 *            F U N C T I O N     D E F I N I T I O N S
 * *************************************************************
 */

// Postcondition: return the larger of the two items
template <class T>
T maximal(const T &a, const T &b)
{
    if (a >= b)
        return a;
    else
        return b;
}

// Postcondition: returns index of the largest item in the data
template <class T>
int index_of_maximal(T data[], int n)
{
    int largest = 0, largestIndex = 0;
    for (int i = 0; i < n; ++i)
    {
        if (data[i] >= largest)
        {
            largest = data[i];
            largestIndex = i;
        }
    }
    return largestIndex;
}

// Postcondition: swaps the two items
template <class T>
void Swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Postcondition: return the first element in data that is
// not less than entry
template <class T>
int first_ge(const T data[], int n, const T &entry)
{
    // go through data
    for (int i = 0; i < n; ++i)
    {
        if (data[i] >= entry)
            return i;
    }
    // if there is no data larger than entry
    // return the size, indicating that all entries
    // are less than the target
    return n;
}

// Postcondition: insert entry at index i in data
template <class T>
void insert_item(T data[], int i, int &n, T entry)
{
    // add one to data count
    n = n + 1;
    // shift over existing entries
    for (int j = n - 1; j > i; --j)
    {
        data[j] = data[j - 1];
    }
    // place entry
    data[i] = entry;
}

template <class T>
void delete_item(T data[], int i, int &n, T &entry)
{
    // place item at index i at entry
    entry = data[i];

    for (int j = i + 1; j < n; j++)
    {
        data[j - 1] = data[j];
    }
    // set final element to 0
    data[n - 1] = 0;
    n--;

    bool debug = false;
    if (debug)
    {
        cout << "DEBUGGING" << endl;
        for (int k = 0; k < n; ++k)
            cout << data[k] << " ";
        cout << "\nENTRY" << endl;
        cout << entry << endl;
        cout << endl;
    }
}

// Postcondition: append entry to the right of data
template <class T>
void attach_item(T data[], int &n, const T &entry)
{
    data[n] = entry;
    n++;
}

// Postcondition: append data 2 to the right of data 1
template <class T>
void merge(T data1[], int &n1, T data2[], int &n2)
{
    for (int i = 0; i < n2; ++i)
    {
        data1[n1] = data2[i];
        n1++;
    }
    n2 = 0;
}

// Postcondition: remove the last element in data and place
// it in entry
template <class T>
void detach_item(T data[], int &n, T &entry)
{
    entry = data[n - 1];
    --n;
}

// Postconditon: insert entry into the sorted array data with length n
template <class T>
void ordered_insert(T data[], int &n, T entry)
{
    // place entry at the end of extended array
    n = n + 1;
    data[n - 1] = entry;
    ;
    // swap until it reaches its correct place
    for (int j = n - 1; j > 0; --j)
    {
        if (data[j] <= data[j - 1])
        {
            T temp = data[j];
            data[j] = data[j - 1];
            data[j - 1] = temp;
        }
    }

    bool debug = false;
    if (debug)
    {
        cout << "DEBUGGING" << endl;
        for (int k = 0; k < n; ++k)
            cout << data[k] << " ";
        cout << "\nENTRY" << endl;
        cout << entry << endl;
        cout << endl;
    }
}

// move n/2 elements from the right of data 1 to data 2
template <class T>
void split(T data1[], int &n1, T data2[], int &n2)
{
    T trash;
    int half = n1 / 2;
    for (int i = 0; i < half; ++i)
    {
        detach_item(data1, n1, trash);
        insert_item(data2, 0, n2, trash);
    }

    bool debug = false;
    if (debug)
    {
        cout << "DEBUGGING Data1" << endl;
        for (int k = 0; k < n1; ++k)
            cout << data1[k] << " ";
        cout << "\nData2" << endl;
        for (int k = 0; k < n2; ++k)
            cout << data2[k] << " ";
        cout << endl;
    }
}

// Postcondition: copy src[] into dest[]
template <class T>
void copy_array(T dest[], const T src[],
                int &dest_size, int src_size)
{
    for (int i = 0; i < src_size; ++i)
    {
        dest[i] = src[i];
        dest_size++;
    }
}

// checks if array is sorted
template <class T>
bool checkSort(T data[], int size)
{
    for (int i = 0; i < size - 1; ++i)
    {
        if (data[i] > data[i + 1])
            return false;
    }
    return true;
}

// shuffle elements of a[]
template <typename T>
void shuffle(T a[], unsigned int size)
{
    Random r;

    for (unsigned int i = 0; i < size; ++i)
    {
        // random position
        int pos = r.GetNext(0, static_cast<int>(size) - 1);
        // swap current element with the
        // element at random position
        T temp = a[i];
        a[i] = a[pos];
        a[pos] = temp;
    }
}
// Postcondition: adds addme to list
template <typename T>
vector<T> &operator+=(vector<T> &list, const T &addme)
{
    list.push_back(addme);
    return list;
}

// Postcondition: print array data
template <class T>
void print_array(const T data[], int n, int pos)
{
    (void)pos;
    for (int k = 0; k < n; ++k)
        cout << data[k] << " ";
}

// Extra operators for Vectors
// Postcondition: print vector list
template <typename T>
ostream &operator<<(ostream &outs, const vector<T> &list)
{
    outs << "{";
    for (size_t i = 0; i < list.size(); ++i)
    {
        outs << list[i];
        if (i < list.size() - 1)
            outs << ",";
    }
    outs << "}";

    return outs;
}

// intersection of two vectors
template <class T>
vector<T> intersection(vector<T> first, vector<T> second)
{
    // sort the two vectors
    sort(first.begin(), first.end());
    sort(second.begin(), second.end());

    bool debug = false;
    vector<T> temp;
    std::set_intersection(first.begin(), first.end(),
                          second.begin(), second.end(),
                          std::back_inserter(temp));

    if (debug)
    {
        for (size_t i = 0; i < temp.size(); ++i)
        {
            cout << temp[i] << endl;
        }
    }
    return temp;
}

// Union of two vectors
template <class T>
vector<T> VUnion(vector<T> first, vector<T> second)
{
    // sort the vectors
    sort(first.begin(), first.end());
    sort(second.begin(), second.end());

    bool debug = false;
    vector<T> temp;
    std::set_union(first.begin(), first.end(),
                   second.begin(), second.end(),
                   std::back_inserter(temp));
    if (debug)
    {
        for (size_t i = 0; i < temp.size(); ++i)
        {
            cout << temp[i] << endl;
        }
    }
    return temp;
}

template <class T>
void VSort(vector<T> &first)
{
    // sort the vectors
    sort(first.begin(), first.end());
}
#endif // ARRAYFUNCTIONS_H
