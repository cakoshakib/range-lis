#include "rlis.h"
using namespace std;

void RLIS::build_approx_tree(Node* node) {
    int start = node->start, mid = node->mid, end = node->end;
    //cout << start << " " << mid << " " << end << endl;

    vector<int> memo = node->list;

    // Calculate LIS([i, n/2]) and LIS([n/2+1,j])
    vector<PatienceNode*> bcw = backward_lis(start, mid);
    for (int i = start; i <= mid; i++) {
        memo[i-start] = bcw[i-start]->ln;
    }
    vector<PatienceNode*> frw = forward_lis(mid+1, end);
    for (int j = mid+1; j <= end; j++) {
        memo[j-start] = frw[j-mid-1]->ln;
    }
    node->list = memo;

    if (mid+1 == end) return;

    node->left = new Node(start, mid);
    node->right = new Node(mid+1, end);
    build_approx_tree(node->left), build_approx_tree(node->right);
}

int RLIS::answer_approx_query(Node *node, int i, int j) {
    if (i <= node->mid && node->mid < j) {
        return max(node->list[i-node->start], node->list[j-node->start]);
    } else if (j <= node->mid) {
        return answer_approx_query(node->left, i, j);
    } else {
        return answer_approx_query(node->right, i, j);
    }
}

query_map_t RLIS::two_approx() {
    int n = seq.size();

    Node *root = new Node(0, n-1);
    build_approx_tree(root);

    query_map_t approx;
    for (query_t q : queries) {
        int i = q.first, j = q.second;
        approx[q] = answer_approx_query(root, i, j);
        cout << "Query (" << i << "," << j << ") approximation: " << approx[q] << endl;
    }

    return approx;
}
