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
    //rlis.run();
    cout << endl;
    // vector<int> res = rlis.backward_lis(0, 5);
    // for (int i = 0; i < res.size(); i++) {
    //     cout << res[i] << " ";
    // }

    query_map_t approx = rlis.two_approx();
    for (int i = 0; i < queries.size(); i++) {
        cout << "approx of " << queries[i].first << "," << queries[i].second << " ";
        cout << approx[queries[i]] << endl;
    }
}
