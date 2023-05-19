#include "rbtree.h"
#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <algorithm>

using namespace std;

int main_bk() {
    RBtree<int, int> rb;
    int a, b;
    ifstream in("in.txt");
    if (in.is_open()) {
        while (in >> a >> b) {
            // cout << a << b << endl;
            rb.put(a, b);
            rb.output();
            cout << '\n';
        }
    }
    in.close();
    rb.output();
    while (!rb.isEmpty()) {
        cout << rb.max() << endl;
        assert(rb.max() == rb.deleteMax());
    }
    return 0;
}

int test (int n) {
    RBtree<int, int> rb;
    map<int, int> mmap;
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        // if (i % 1000 == 0) {
        //     cout << i / 1000 << "%" << endl;
        // }
        int tmp = rand() % 3000000;
        rb.put(tmp, tmp);
        // cout << tmp << endl;
        mmap[tmp] = 1;
    }
    vector<int> vec;
    for (auto i : mmap)
        vec.push_back(i.first);
    random_shuffle(vec.begin(), vec.end());
    int cnt = 0;
    for (auto i : mmap) {
        cout << "cnt " << cnt << " remove " << i.first << endl;
        rb.deleteNode(vec[cnt]);
        cnt += 1;
        assert(rb.isBalanced());
        assert(rb.is23Tree());
    }
    return 0;
}

int main()
{
    main_bk();
    return 0;
}
