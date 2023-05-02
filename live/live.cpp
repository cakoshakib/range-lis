#include "live.h"
#include <iostream>
using namespace std;

LISLive::LISLive(vector<int> _sequence, vector<query_t> _queries) {
    this->seq = _sequence;
    this->queries = _queries;
    this->n = _sequence.size();
}

vector<vector<int>> LISLive::run() {
    vector<vector<int>> ans;
    for (query_t q : queries) {
        vector<int> res = lis(q.first, q.second);
        ans.push_back({res[res.size() - 1]});
    }
    return ans;
}

vector<int> LISLive::lis(int lb, int ub) {
    if (n < lb) {
        cout << "Invalid parameters to lis\n";
        exit(1);
    }
    vector<int> d(ub-lb+2, INF);
    d[0] = -INF;
    vector<int> res(ub-lb+1, INF);
    int longest = 0;
    for (int i = lb; i <= ub; i++) {
        int l = lower_bound(d.begin(), d.end(), seq[i]) - d.begin();
        d[l] = seq[i];
        longest = max(longest, l);
        res[i - lb] = longest;
    }
    return res;
}