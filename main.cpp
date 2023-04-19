#include <iostream>
#include "novel/rlis.h"
using namespace std;

int main() {
    cout << "Hello, world!\n";

    int n, q;
    cin >> n >> q;

    vector<int> sequence(n);
    for (int i = 0; i < n; i++) {
        cin >> sequence[i];
    }
    vector<pair<int,int>> queries(q);
    for (int i = 0; i < q; i++) {
        cin >> queries[i].first >> queries[i].second;
    }

    RLIS rlis(sequence, queries);
    rlis.run();
}