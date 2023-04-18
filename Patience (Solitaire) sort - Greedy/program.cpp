#include <iostream>
#include <fstream>
#include <stack>
#include <queue>

using namespace std;

ifstream fin("input.txt");
ofstream fout("output.txt");

class ComparatorClass {
public:
    bool operator()(stack<int>* a, stack<int>* b) {
        if (a->empty())
            return false;
        if (b->empty())
            return true;
        if (a->top() > b->top())
            return true;
        return false;
    }
};

void Enqueue(priority_queue<stack<int>*, vector<stack<int>*>, ComparatorClass >& pq) {
    int n, x;
    fin >> n;
    queue< stack<int> *> tmpq;
    for(int i = 1; i <= n; ++i) {
        fin>>x;
        while(!pq.empty() && x > pq.top()->top()) {
            tmpq.push(pq.top());
            pq.pop();
        }

        if(pq.empty())
            pq.push(new stack<int>);
        pq.top()->push(x);
        stack<int>* stkptr = pq.top();
        pq.pop();
        pq.push(stkptr); /// update heap

        while(!tmpq.empty()) {
            pq.push(tmpq.front());
            tmpq.pop();
        }
    }
    
}

vector<int>* Merge(priority_queue<stack<int>*, vector<stack<int>*>, ComparatorClass >& pq) { /// & to avoid copying
    vector<int>* vec = new vector<int>;
    while (!pq.empty()) {
        vec->push_back(pq.top()->top());
        pq.top()->pop();
        if (pq.top()->empty())
            pq.pop();
        else {
            stack<int>* stkptr = pq.top();
            pq.pop();
            pq.push(stkptr); /// update heap
        }
    }
    return vec;
}

int main() {
    priority_queue<stack<int>*, vector<stack<int>*>, ComparatorClass > pq;
    Enqueue(pq);
    vector<int>* result;
    result = Merge(pq);
    for (auto& x : (*result))
        fout << x << ' ';
    result->clear();
    delete result;
    return 0;
}