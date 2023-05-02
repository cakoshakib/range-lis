#include "precompute.h"
#include <iostream>
using namespace std;

LISPre::LISPre(vector<int> _sequence, vector<query_t> _queries) {
    this->seq = _sequence;
    this->queries = _queries;
    this->n = _sequence.size();
}

vector<vector<int>> LISPre::run() {
    map<query_t, int> q_ans = all_lis();
    vector<vector<int>> ans;
    for (query_t q : queries) {
        vector<int> res = {q_ans[q]};
        ans.push_back(res);
    }
    return ans;
}

map<query_t, int> LISPre::all_lis() {
    map<query_t, int> q_ans;
    for (int start = 0; start < n; start++) {
        vector<int> res = lis(start);
        for (int end = start; end < n; end++) { 
            //cout << "Query answer (" << start << "," << end << ") " << res[end-start] << endl; 
            q_ans[{start, end}] = res[end-start];
        }
    }
    return q_ans;
}

vector<int> LISPre::lis(int lb) {
    if (n < lb) {
        cout << "Invalid parameters to lis\n";
        exit(1);
    }
    vector<int> d(n-lb+1, INF);
    d[0] = -INF;
    vector<int> res(n-lb, INF);
    int longest = 0;
    for (int i = lb; i < n; i++) {
        int l = lower_bound(d.begin(), d.end(), seq[i]) - d.begin();
        d[l] = seq[i];
        longest = max(longest, l);
        res[i - lb] = longest;
    }
    return res;
}