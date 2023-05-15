#include <iostream>
#include "novel/rlis.h"
#include "precompute/precompute.h"
#include "live/live.h"
using namespace std;

void print_result(vector<query_t>& queries, vector<vector<int>>& res) {
    for (unsigned int i = 0; i < queries.size(); i++) {
        cout << "Query (" << queries[i].first << "," << queries[i].second << "): ";
        for (int x : res[i]) {
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

    // cout << "----NOVEL----\n";
    // RLIS rlis(sequence, queries);
    // vector<vector<int>> rlis_result = rlis.run();
    // print_result(queries, rlis_result);
    // cout << "--------\n";
    
    cout << "----PRECOMPUTE----\n";
    LISPre pre(sequence, queries);
    vector<vector<int>> pre_result = pre.run();
    print_result(queries, pre_result);
    cout << "--------\n";

    // cout << "----LIVE----\n";
    // LISLive live(sequence, queries);
    // vector<vector<int>> live_result = live.run();
    // print_result(queries, live_result);
    // cout << "--------\n";

}
