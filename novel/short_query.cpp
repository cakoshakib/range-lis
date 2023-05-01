#include "rlis.h"
#include <math.h>
#include <algorithm>
using namespace std;

vector<int> RLIS::r_range_max(
    map<double, map<int, vector<int>>>& B,
    ShortNode *node,
    int r,
    int beta
) {
    cout << beta << " " << r << " " << node->mid+1 << endl;
    int max_i = -1;
    int a_r = seq[r];
    for (int i = (node->mid)+1; i < r; i++) {
        int a_i = seq[i];
        if (a_i < a_r && (max_i == -1 || B[beta-1][max_i].size() == 0 || B[beta-1][i][0] > B[beta-1][max_i][0]))
            max_i = i;
    }
    if (max_i == -1) return {};
    vector<int> longest = B[beta-1][max_i];
    longest.push_back(a_r);
    return longest;
}

vector<int> RLIS::l_range_max(
    map<double, map<int, vector<int>>>& C,
    ShortNode *node,
    int l,
    int alpha 
) {
    int max_i = -1;
    int a_l = seq[l];
    for (int i = l+1; i <= node->mid; i++) {
        int a_i = seq[i];
        if (a_i > a_l && (max_i == -1 || C[alpha-1][max_i].size() != alpha || C[alpha-1][i][alpha-2] > C[alpha-1][max_i][alpha-2]))
            max_i = i;
    }
    if (max_i == -1) return {};
    vector<int> longest = C[alpha-1][max_i];
    longest.push_back(a_l);
    return longest;
}

void RLIS::preprocess_dp(ShortNode *node) {
    int start = node->start, mid = node->mid, end = node->end;
    node->LR = map<double, map<int, vector<int>>>();
    cout << start << " " << mid << " " << end << endl;

    // TODO: probably doing some accidental O(n) ops here

    // Compute B[r][beta] for all [n/2+1, r]
    map<double, map<int, vector<int>>> B;
    for (int beta = 1; beta < ceil(barrier); beta++) {
        cout << "Working on beta " << beta << " " << end-start+1 << endl;
        for (int r = mid+1; r <= end; r++) {
            cout << "r " << r << endl;
            int a_r = seq[r];
            B[beta][r].resize(beta);
            node->LR[beta][r].resize(beta);
            if (beta == 1) {
                cout << "Setting B[" << beta << "][" << r << "] to " << a_r << endl;
                cout << B[beta][r].size() << endl;
                B[beta][r] = {a_r};
                cout << B[beta][r][0] << endl;
                cout << "Done\n";
            } else {
                cout << "Entered r_range_max\n";
                B[beta][r] = r_range_max(B, node, r, beta);
                cout << "Exiting\n";
            }

            // Update R side of LR 
            if (r > mid+1 && (B[beta][r].size() == 0 || node->LR[beta][r-1][0] > B[beta][r][0])) {
                node->LR[beta][r] = node->LR[beta][r-1];
            } else {
                node->LR[beta][r] = B[beta][r];
            }
        }
    }

    // Compute C[l][alpha] for all [1, n/2]
    map<double, map<int, vector<int>>> C;
    for (int alpha = 1; alpha < ceil(barrier); alpha++) {
        for (int l = start; l <= mid; l++) {
            int a_l = seq[l];
            C[alpha][l].resize(alpha);
            node->LR[alpha][l].resize(alpha);
            if (alpha == 1) {
                cout << "Setting C[" << alpha << "][" << l << "] to " << a_l << endl;
                C[alpha][l] = {a_l};
            } else {
                cout << "Entered l_range_max\n";
                C[alpha][l] = l_range_max(C, node, l, alpha);
                cout << "Exiting\n";
            }

            // Update R side of LR 
            cout << alpha << " " << l << endl;
            cout << C[alpha][l].size() << endl;
            cout << node->LR[alpha][l-1].size() << endl;
            if (l > start && (C[alpha][l].size() != alpha || node->LR[alpha][l-1][alpha-1] > C[alpha][l][alpha-1])) {
                cout << "here\n";
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

int RLIS::compatible_beta(
    map<double, map<int, vector<int>>>& LR, 
    int peak, 
    int alpha, 
    query_t q,
    ShortNode *node
) {
    int r = q.second;
    int best_beta = 0;
    int longest_length = 0;
    // binary saerch
    int lo = 1, hi = min((int)ceil(barrier), node->end - node->mid + 1);
    while (lo < hi) {
        int beta = (lo + hi) / 2;
        if (peak < LR[beta][r][0]) {
            int ln = LR[beta][r].size();
            if (longest_length > ln) {
                best_beta = beta;
                longest_length = ln;
            }
            lo = beta + 1;
        } else {
            hi = beta;
        }
    }
    return best_beta;
}

vector<int> RLIS::short_query(ShortNode *node, query_t q) {
    int i = q.first, j = q.second;
    if (i <= node->mid && node->mid < j) {
        map<double, map<int, vector<int>>> LR = node->LR;
        pair<int, int> alpha_beta;
        int longest_alpha_beta = 0;
        for (int alpha = 1; alpha < ceil(barrier); alpha++) {
            if (LR[alpha][i].size() != alpha) continue;
            int peak = LR[alpha][i][alpha-1];
            // Find corresponding beta to maximize alpha + beta
            int beta = compatible_beta(LR, peak, alpha, q, node);
            int ln = LR[alpha][i].size() + LR[beta][j].size();
            if (ln > longest_alpha_beta) {
                alpha_beta = {alpha, beta};
                longest_alpha_beta = ln;
            }
        }
        int alpha = alpha_beta.first, beta = alpha_beta.second;
        // cout << "Reached\n";
        // cout << longest_alpha_beta << endl;
        // cout << alpha_beta.first << " " << alpha_beta.second << endl;
        // cout << LR[alpha][i].size() << " " << LR[beta][j].size() << endl;
        vector<int> result;
        cout << LR[alpha][i][0] << endl;
        result.reserve(longest_alpha_beta);
        result.insert(
            result.end(), 
            LR[alpha][i].begin(), 
            LR[alpha][i].end()
        );
        result.insert(
            result.end(),
            LR[beta][j].begin(),
            LR[beta][j].end()
        );
        return result;
    } else if (j <= node->mid) {
        return short_query(node->left, q);
    } else {
        return short_query(node->right, q);
    }
    return {};
}
