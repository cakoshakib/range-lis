#include "live.h"
#include <iostream>
#include <algorithm>
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
        ans.push_back(res);
    }
    return ans;
}

unsigned int LISLive::stack_search(vector<PatienceNode*>& stack_tops, int x) {
    if (stack_tops.size() == 0) return 0;
    int lo = 0, hi = stack_tops.size();
    while (lo < hi) {
        int mid = (lo + hi)/2;
        if (stack_tops[mid]->val > x) {
            hi = mid;
        } else {
            lo = mid+1;
        }
    }
    return lo;
}

vector<int> LISLive::lis(unsigned int lb, unsigned int ub) {
    if (n < lb) {
        cout << "Invalid parameters to lis\n";
        exit(1);
    }

    vector<PatienceNode*> stack_tops;
    for (unsigned int i = lb; i <= ub; i++) {
        unsigned int pos = stack_search(stack_tops, seq[i]);
        PatienceNode *node = new PatienceNode(seq[i]);
        if (pos != 0) {
            node->ln = stack_tops[pos-1]->ln+1;
            node->next = stack_tops[pos-1];
        } 

        if (pos == stack_tops.size()) 
            stack_tops.push_back(node);
        else
            stack_tops[pos] = node;
    }
    PatienceNode *longest = stack_tops[stack_tops.size()-1];

    vector<int> res;
    while (longest != nullptr) {
        res.push_back(longest->val);
        longest = longest->next;
    }
    reverse(res.begin(), res.end());
    return res;
}