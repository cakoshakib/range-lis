#include "rlis.h"
#include <math.h>
using namespace std;

vector<int> RLIS::r_range_max(
    map<double, vector<vector<int>>>& B,
    ShortNode *node,
    int r,
    int beta
) {
    int max_i = 0;
    int a_r = seq[r];
    for (int i = node->mid+1; i < r; i++) {
        int a_i = seq[i];
        if (a_i < a_r && (max_i == -1 || B[beta-1][i][0] > B[beta-1][max_i][0]))
            max_i = i;
    }
    vector<int> longest = B[beta-1][max_i];
    longest.push_back(a_r);
    return longest;
}

vector<int> RLIS::l_range_max(
    map<double, vector<vector<int>>>& C,
    ShortNode *node,
    int l,
    int alpha 
) {
    int max_i = -1;
    int a_l = seq[l];
    for (int i = l+1; i <= node->mid; i++) {
        int a_i = seq[i];
        if (a_i > a_l && (max_i == -1 || C[alpha-1][i][alpha-2] > C[alpha-1][max_i][alpha-2]))
            max_i = i;
    }
    vector<int> longest = C[alpha-1][max_i];
    longest.push_back(a_l);
    return longest;
}

void RLIS::preprocess_dp(ShortNode *node) {
    int start = node->start, mid = node->mid, end = node->end;
    node->LR = map<double, vector<vector<int>>>();

    // TODO: probably doing some accidental O(n) ops here

    // Compute B[r][beta] for all [n/2+1, r]
    map<double, vector<vector<int>>> B;
    for (int beta = 1; beta < ceil(barrier); beta++) {
        B[beta] = vector<vector<int>>(end-mid);
        node->LR[beta] = vector<vector<int>>(end-mid-1);
        for (int r = mid+1; r <= end; r++) {
            int a_r = seq[r];
            if (beta == 1) {
                B[beta][r] = {a_r};
            } else {
                B[beta][r] = r_range_max(B, node, r, beta);
            }

            // Update R side of LR 
            if (r > mid+1 && node->LR[beta][r-1][0] > B[beta][r][0]) {
                node->LR[beta][r] = node->LR[beta][r-1];
            } else {
                node->LR[beta][r] = B[beta][r];
            }
        }
    }

    // Compute C[l][alpha] for all [1, n/2]
    map<double, vector<vector<int>>> C;
    for (int alpha = 1; alpha < ceil(barrier); alpha++) {
        C[alpha] = vector<vector<int>>(mid-start+1);
        for (int l = start; l <= mid; l++) {
            int a_l = seq[l];
            if (alpha == 1) {
                C[alpha][l] = {a_l};
            } else {
                C[alpha][l] = l_range_max(C, node, l, alpha);
            }

            // Update R side of LR 
            if (l > start && node->LR[alpha][l-1][alpha-1] > C[alpha][l][alpha-1]) {
                node->LR[alpha][l] = node->LR[alpha][l-1];
            } else {
                node->LR[alpha][l] = C[alpha][l];
            }
        }

    }

    if (mid+1 == end) return;

    node->left = new ShortNode(start, mid);
    node->right = new ShortNode(mid+1, end);
    preprocess_dp(node->left), preprocess_dp(node->right);
}