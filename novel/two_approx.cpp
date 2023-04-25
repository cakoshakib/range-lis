#include "rlis.h"
using namespace std;

void RLIS::build_approx_tree(ApproxNode *node) {
    int start = node->start, mid = node->mid, end = node->end;
    //cout << start << " " << mid << " " << end << endl;

    vector<int> memo = node->approx;

    // Calculate LIS([i, n/2]) and LIS([n/2+1,j])
    vector<int> bcw = backward_lis(start, mid);
    for (int i = start; i <= mid; i++) {
        memo[i-start] = bcw[i-start];
    }
    vector<int> frw = forward_lis(mid+1, end);
    for (int j = mid+1; j <= end; j++) {
        memo[j-start] = frw[j-mid-1];
    }
    node->approx = memo;

    if (mid+1 == end) return;

    node->left = new ApproxNode(start, mid);
    node->right = new ApproxNode(mid+1, end);
    build_approx_tree(node->left), build_approx_tree(node->right);
}

int RLIS::answer_query(ApproxNode *node, int i, int j) {
    if (i <= node->mid && node->mid < j) {
        return max(node->approx[i-node->start], node->approx[j-node->start]);
    } else if (j <= node->mid) {
        return answer_query(node->left, i, j);
    } else {
        return answer_query(node->right, i, j);
    }
}

query_map_t RLIS::two_approx() {
    cout << "Performing two approximation\n";
    int n = seq.size();

    ApproxNode *root = new ApproxNode(0, n-1);
    cout << "Building approximation tree\n";
    build_approx_tree(root);

    query_map_t approx;
    for (query_t q : queries) {
        int i = q.first, j = q.second;
        approx[q] = answer_query(root, i, j);
        cout << "Query (" << i << "," << j << ") approximation: " << approx[q] << endl;
    }

    return approx;
}
