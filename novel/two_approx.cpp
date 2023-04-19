#include "rlis.h"
using namespace std;

class ApproxNode {
public:
    int mid, start, end;
    int data;
    ApproxNode *left, *right;
    ApproxNode(int start, int end) {
        this->start = start;
        this->end = end;
        this->mid = mid_point(start, end);
        this->left = nullptr;
        this->right = nullptr;
    }

    int mid_point(int x, int y) {
        return x + ((y - x) / 2);
    }

};

void RLIS::build_approx_tree(ApproxNode *node) {
    return;
}


query_map_t RLIS::two_approx() {
    vector<int> seq = this->seq;
    int n = seq.size();

    ApproxNode *root = new ApproxNode(0, n-1);
    build_approx_tree(root);

    query_map_t approx;

    return approx;
}
