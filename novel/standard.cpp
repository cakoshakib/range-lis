#include "rlis.h"
#include <iostream>
#include <algorithm>

using namespace std;

vector<int> RLIS::forward_lis(int lb, int ub) {
    if (ub < lb) {
        cout << "Invalid parameters to forward_lis, j > i\n";
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

vector<int> RLIS::backward_lis(int lb, int ub) {
    if (ub < lb) {
        cout << "Invalid parameters to forward_lis, j > i\n";
        exit(1);
    }
    vector<int> d(ub-lb+2, -INF);
    d[0] = INF;
    vector<int> res(ub-lb+1, INF);
    int longest = 0;
    for (int i = ub; i >= lb; i--) {
        int l = lower_bound(d.begin(), d.end(), seq[i], greater<int>()) - d.begin();
        d[l] = seq[i];
        longest = max(longest, l);
        res[i - lb] = longest;
    }
    return res;

}