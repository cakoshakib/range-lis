#include "rlis.h"
#include <iostream>
#include <algorithm>

using namespace std;

unsigned int RLIS::stack_search(vector<PatienceNode*>& stack_tops, int x) {
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

vector<PatienceNode*> RLIS::forward_lis(unsigned int lb, unsigned int ub) {
    if (ub < lb) {
        cout << "Invalid parameters to forward_lis, j > i\n";
        exit(1);
    }
    vector<PatienceNode*> stack_tops;
    vector<PatienceNode*> res;
    for (unsigned int i = lb; i <= ub; i++) {
        unsigned int pos = stack_search(stack_tops, seq[i]);
        if (stack_tops.size() > 0 && pos == 0) {
            res.push_back(stack_tops[stack_tops.size()-1]);
            continue;
        }
        PatienceNode *node = new PatienceNode(seq[i]);
        if (pos != 0) {
            node->ln = stack_tops[pos-1]->ln+1;
            node->next = stack_tops[pos-1];
        } 

        if (pos == stack_tops.size()) 
            stack_tops.push_back(node);
        else
            stack_tops[pos] = node;
        
        res.push_back(stack_tops[stack_tops.size()-1]);
    }
    return res;
}

unsigned int RLIS::bkw_stack_search(vector<PatienceNode*>& stack_tops, int x) {
    if (stack_tops.size() == 0) return 0;
    int lo = 0, hi = stack_tops.size();
    while (lo < hi) {
        int mid = (lo + hi)/2;
        if (stack_tops[mid]->val < x) {
            hi = mid;
        } else {
            lo = mid+1;
        }
    }
    return lo;
}

vector<PatienceNode*> RLIS::backward_lis(int lb, int ub) {
    if (ub < lb) {
        cout << "Invalid parameters to forward_lis, j > i\n";
        exit(1);
    }

    vector<PatienceNode*> stack_tops;
    vector<PatienceNode*> res(ub-lb+1);
    for (int i = ub; i >= lb; i--) {
        unsigned int pos = bkw_stack_search(stack_tops, seq[i]);
        if (stack_tops.size() > 0 && pos == 0) {
            res[i-lb] = stack_tops[stack_tops.size()-1];
            continue;
        }
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