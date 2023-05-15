#include "precompute.h"
#include <iostream>
#include <algorithm>
using namespace std;

LISPre::LISPre(vector<int> _sequence, vector<query_t> _queries) {
    this->seq = _sequence;
    this->queries = _queries;
    this->n = _sequence.size();
}

vector<vector<int>> LISPre::run() {
    map<query_t, vector<int>> q_ans = all_lis();
    vector<vector<int>> ans;
    for (query_t q : queries) {
        vector<int> res = q_ans[q];
        ans.push_back(res);
    }
    return ans;
}

map<query_t, vector<int>> LISPre::all_lis() {
    map<query_t, vector<int>> q_ans;
    for (unsigned int start = 0; start < n; start++) {
        vector<PatienceNode*> res = lis(start);
        for (unsigned int end = start; end < n; end++) { 
            vector<int> query_lis;
            //cout << "Query answer (" << start << "," << end << ") " << endl; 
            PatienceNode *ptr = res[end-start];
            while (ptr != nullptr) {
                //cout << ptr->val << ",";
                query_lis.push_back(ptr->val);
                ptr = ptr->next;
            }
            //cout << endl;

            reverse(query_lis.begin(), query_lis.end());
            q_ans[{start, end}] = query_lis;
        }
    }
    return q_ans;
}

unsigned int LISPre::stack_search(vector<PatienceNode*>& stack_tops, int x) {
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

vector<PatienceNode*> LISPre::lis(unsigned int lb) {
    if (n < lb) {
        cout << "Invalid parameters to lis\n";
        exit(1);
    }
    vector<PatienceNode*> stack_tops;
    vector<PatienceNode*> res(n-lb);
    for (unsigned int i = lb; i < n; i++) {
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
        
        res[i-lb] = stack_tops[stack_tops.size()-1];
    }

    return res;
}