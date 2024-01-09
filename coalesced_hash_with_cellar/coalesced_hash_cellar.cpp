#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

template <typename T>
struct Cell {
    bool free = true;
    T value;
    int nextIndex = -1;
};

template <typename T>
struct Coalesced_Hash {
    int total_size;
    int cellar_size;
    int bottom_size;
    vector<Cell<T>> hashtable;
    
    int hashFunc(T value) {
        return value % bottom_size;
    }
    
    Coalesced_Hash() = delete;

    Coalesced_Hash(int bottom_size, int total_size = -1) {
        if(total_size == -1)
            total_size = bottom_size / 0.86    + 1;
        this->total_size = total_size;
        this->bottom_size = bottom_size;
        this->cellar_size = total_size - bottom_size;
        
        hashtable.resize(total_size);
    }

    void insertVal(T value) {
        int key = hashFunc(value);
        if(hashtable[key].free) {
            hashtable[key].value = value;
            hashtable[key].free = false;
            return;
        }
        /// coliziune, se cauta loc liber incepand de sus, probabil in cellar daca nu a fost epuizat.
        
        int indexChainEnd = findKeyOfCellarWithEmptyNext(key);
        int top_index = total_size - 1;

        while(hashtable[top_index].free == false)
            --top_index;
        if(top_index < 0)
            throw "Hashtable too small!";
        
        hashtable[top_index].free = false;
        hashtable[top_index].value = value;
        hashtable[indexChainEnd].nextIndex = top_index;                                                                         
    }

    bool findVal(T value) {
        int key = hashFunc(value);
        while(hashtable[key].nextIndex != -1) {
            if(hashtable[key].free == false && hashtable[key].value == value) {
                cout<<"BLALBLABALBASDF LA KEY "<< key <<'\n';
                return true;
            }
            
            key = hashtable[key].nextIndex;
        }
        if(hashtable[key].free == false && hashtable[key].value == value) {
            cout<<"BLALBLABALBASDF LA KEY "<< key <<'\n';
            return true;
        }
        return false;
    }

    void deleteVal(T value) {
        int hashval = hashFunc(value);
        int key = hashval;
        int valueIndex = -1;
        int before_last_key = -1;
        while(hashtable[key].nextIndex != -1) {
            if(hashtable[key].value == value) {
                valueIndex = key;
                break;
            }
            before_last_key = key;
            key = hashtable[key].nextIndex;
        }

        if(hashtable[key].nextIndex != -1) { /// reparare inlantuire dupa stergerea elementului daca e cazul
            if(before_last_key == -1) /// se schimba capul de lista daca a fost sters (daca valoarea hash este libera)
                hashtable[hashval].value = hashtable[key].value;
            
            hashtable[before_last_key].nextIndex = hashtable[key].nextIndex;
        }
        
        hashtable[key].free = true;
    }

    void printTable() {
        cout<<"\n---------------table print-----------------\n";
        for(int i = 0; i < total_size; ++i) {
            cout << "Cell " << i << ' ' << (hashtable[i].free ? "free\n" : "occupied by");
            if(hashtable[i].free == false)
                cout << ' ' << hashtable[i].value << ". Next index in collision chain is " << hashtable[i].nextIndex <<'\n';
            if(i == bottom_size - 1) /// delimiteaza cellarul
                cout<<"             ---cellar---  \n";
        }
        cout<<"-------------------------------------------\n";
    }

    private:
        int findKeyOfCellarWithEmptyNext(int key) {
            while(hashtable[key].nextIndex != -1)
                key = hashtable[key].nextIndex;
            return key;
        }
};

template <>
int Coalesced_Hash<string>::hashFunc(string value) {
    int sum = 0;
    for(auto chr : value)
        sum+=chr;
    sum %= bottom_size;
    return sum;
}

void integerHashTest() {
    ifstream fin("inputfile.txt");
    int n;
    int x, before_last_x;
    fin >> n;
    
    Coalesced_Hash<int> hash(n);
    std::cout << "Marimea efectiva a hashtable-ului este: " << hash.hashtable.size() << "\n\n"; 

    for(int i = 0; i < n; ++i) {
        before_last_x = x;
        fin >> x;
        hash.insertVal(x);
    }

    hash.printTable();

    std::cout << before_last_x << ':' << (hash.findVal(before_last_x) ? "gasit" : "negasit") << '\n';

    hash.deleteVal(before_last_x);
    std::cout << "S-a sters " << before_last_x << '\n';

    std::cout << before_last_x << ':' << (hash.findVal(before_last_x) ? "gasit" : "negasit") << '\n';

    hash.printTable();
}

void stringHashTest() {
    ifstream fin("inputfilestrings.txt");
    int n;
    string x, before_last_x;
    fin >> n;
    
    Coalesced_Hash<string> hash(n);
    std::cout << "Marimea efectiva a hashtable-ului este: " << hash.hashtable.size() << "\n\n"; 

    for(int i = 0; i < n; ++i) {
        before_last_x = x;
        fin >> x;
        hash.insertVal(x);
    }

    hash.printTable();

    std::cout << before_last_x << ':' << (hash.findVal(before_last_x) ? "gasit" : "negasit") << '\n';

    hash.deleteVal(before_last_x);
    std::cout << "S-a sters " << before_last_x << '\n';

    std::cout << before_last_x << ':' << (hash.findVal(before_last_x) ? "gasit" : "negasit") << '\n';

    hash.printTable();
}


int main() {
    //integerHashTest();
    stringHashTest();
    return 0;
}

