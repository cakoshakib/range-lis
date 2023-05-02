#include <iostream>
#include "novel/rlis.h"
#include "precompute/precompute.h"
using namespace std;

void print_2d(vector<vector<int>>& res) {
    for (vector<int> v : res) {
        for (int x : v) {
            cout << x << " ";
        }
        cout << endl;
    }
}

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

    // RLIS rlis(sequence, queries);
    // vector<vector<int>> rlis_result = rlis.run();
    
    LISPre pre(sequence, queries);
    vector<vector<int>> pre_result = pre.run();
    print_2d(pre_result);

}
