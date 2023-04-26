#include <iostream>
#include "novel/rlis.h"
using namespace std;

int main() {
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
    // cout << endl;
    // vector<int> bkw = rlis.backward_lis(0, 5);
    // for (int i = 0; i < bkw.size(); i++) {
    //     cout << bkw[i] << " ";
    // }
    // vector<int> frw = rlis.forward_lis(5, 10);
    // for (int i = 0; i < frw.size(); i++) {
    //     cout << frw[i] << " ";
    // }
    // vector<int> test;
    // test.reserve(bkw.size() + frw.size());
    // test.insert(test.end(), bkw.begin(), bkw.end());
    // test.insert(test.end(), frw.begin()+1, frw.end());
    // cout << endl;
    // for (int i = 0; i < test.size(); i++) {
    //     cout << test[i] << " ";
    // }

    // query_map_t approx = rlis.two_approx();
    // for (int i = 0; i < queries.size(); i++) {
    //     cout << "approx of " << queries[i].first << "," << queries[i].second << " ";
    //     cout << approx[queries[i]] << endl;
    // }
}
