#include "rlis.h"
#include <math.h>
#include <algorithm>
#include <limits>
using namespace std;

vector<int> RLIS::r_range_max(
    map<double, map<int, vector<int>>>& B,
    ShortNode *node,
    int r,
    unsigned int beta
) {
    int max_i = -1;
    int a_r = seq[r];
    for (int i = (node->mid)+1; i < r; i++) {
        int a_i = seq[i];
        if (a_i < a_r && 
            B[beta-1][i].size() == beta-1 && (
            max_i == -1 || 
            B[beta-1][i][0] > B[beta-1][max_i][0]
        ))
            max_i = i;
    }
    if (max_i == -1) return {};
    vector<int> longest = B[beta-1][max_i];
    longest.push_back(a_r);
    return longest;
}

void RLIS::r_range_tree(Node *node, map<int, PatienceNode*>& B_beta) {
    // Construct tree that allows for log(n) r_range_max computation
    int start = node->start, mid = node->mid, end = node->end;
    // maximum base 
    // sort points
    vector<pair<int,int>> sorted_seq;
    for (int i = start; i <= mid; i++) sorted_seq.push_back({seq[i], i});
    sort(sorted_seq.begin(), sorted_seq.end());

    for (int i = 0; i <= mid-start; i++) {
        int index = sorted_seq[i].second;
        if (!B_beta[index]) continue;
        if (node->l1.size() == 0 || B_beta[index]->base > node->l1[node->l1.size()-1]) {
            node->l1.push_back(B_beta[index]->base);
            node->l2.push_back(sorted_seq[i]);
        }
    }
    // recurse
    if (mid+1 >= end) return;
    node->left = new Node(start, mid-1);
    node->right = new Node(mid+1, end);
    r_range_tree(node->left, B_beta);
    r_range_tree(node->right, B_beta);
}

int RLIS::r_range_max_tree(Node *node, int r) {
    int ret = -1;
    int mx_base = -1e5;
    while (node != nullptr) {
        if (node->mid < r) {
            pair<int,int> x = {seq[r], 1e5};
            int i = lower_bound(node->l2.begin(), node->l2.end(), x) - node->l2.begin() - 1;
            
            if (i != -1 && node->l1[i] > mx_base) {
                mx_base = node->l1[i];
                ret = node->l2[i].second;
            }
            node = node->right;
        } else {
            node = node->left;
        }
    }
    return ret;
}

void RLIS::l_range_tree(Node *node, map<int, PatienceNode*>& C_alpha, int alpha) {
    // Construct tree that allows for log(n) l_range_max computation
    // need the minimum peak 
    int start = node->start, mid = node->mid, end = node->end;

    // sort points
    vector<pair<int,int>> sorted_seq;
    for (int i = mid; i <= end; i++) sorted_seq.push_back({seq[i], i});
    sort(sorted_seq.begin(), sorted_seq.end());

    for (int i = 0; i <= end-mid; i++) {
        int index = sorted_seq[i].second;
        if (!C_alpha[index]) continue;
        if (node->l1.size() == 0 || C_alpha[index]->peak > node->l1[node->l1.size()-1]) {
            node->l1.push_back(C_alpha[index]->peak);
            node->l2.push_back(sorted_seq[i]);
        }
    }

    // recurse
    if (mid+1 >= end) return;
    node->left = new Node(start, mid);
    node->right = new Node(mid+1, end);
    l_range_tree(node->right, C_alpha, alpha);
    l_range_tree(node->left, C_alpha, alpha);
}

int RLIS::l_range_max_tree(Node *node, int l) {
    int ret = -1;
    int min_peak = 1e5;
    while (node != nullptr) {
        if (node->mid > l) {
            pair<int,int> x = {seq[l], -1e9};
            unsigned int i = upper_bound(node->l2.begin(), node->l2.end(), x) - node->l2.begin();
            
            if (i != node->l1.size() && node->l1[i] < min_peak) {
                min_peak = node->l1[i];
                ret = node->l2[i].second;
            }
            node = node->left;
        } else {
            node = node->right;
        }
    }
    return ret;
}

vector<int> RLIS::l_range_max(
    map<double, map<int, vector<int>>>& C,
    ShortNode *node,
    int l,
    unsigned int alpha 
) {
    int max_i = -1;
    int a_l = seq[l];
    for (int i = l+1; i <= node->mid; i++) {
        int a_i = seq[i];
        if (a_i > a_l && 
            C[alpha-1][i].size() == alpha-1 && (
            max_i == -1 || 
            C[alpha-1][i][alpha-2] < C[alpha-1][max_i][alpha-2]
        ))
            max_i = i;
    }
    if (max_i == -1) return {};
    vector<int> longest(alpha);
    longest[0] = a_l;
    for (unsigned int i = 1; i < alpha; i++) {
        longest[i] = C[alpha-1][max_i][i-1];
    }
    return longest;
}

void RLIS::preprocess_dp(ShortNode *node) {
    int start = node->start, mid = node->mid, end = node->end;
    node->LR = map<double, map<int, PatienceNode*>>();

    // Compute B[r][beta] for all [n/2+1, r]
    map<double, map<int, PatienceNode*>> B;
    for (int beta = 1; beta < ceil(barrier); beta++) {
        Node *mx_tree = new Node(mid+1, end);
        if (beta > 1)
            r_range_tree(mx_tree, B[beta-1]);
        for (int r = mid+1; r <= end; r++) {
            int a_r = seq[r];
            if (beta == 1) {
                B[beta][r] = new PatienceNode(a_r);
                B[beta][r]->base = a_r;
                B[beta][r]->peak = a_r;
            } else {
                int i = r_range_max_tree(mx_tree, r);
                if (i == -1) {
                    B[beta][r] = nullptr;
                } else {
                    B[beta][r] = new PatienceNode(a_r);
                    B[beta][r]->next = B[beta-1][i];
                    B[beta][r]->ln = (B[beta-1][i]->ln) + 1;
                    B[beta][r]->base = (B[beta-1][i]->base);
                    B[beta][r]->peak = a_r;
                }


                // cout << "Setting B[" << beta << "][" << r << "] to ";
                // if (!B[beta][r])  {
                //     cout << "NONE\n";
                // } else {
                //     for (PatienceNode* ptr = B[beta][r]; ptr != nullptr; ptr = ptr->next) {
                //         cout << ptr->val << ",";
                //     }
                //     cout << endl;
                // }
            }

            // Update R side of LR 
            if (r > mid+1 && 
                node->LR[beta][r-1] && (
                !B[beta][r] || node->LR[beta][r-1]->base > B[beta][r]->base
            )) {
                node->LR[beta][r] = node->LR[beta][r-1];
            } else {
                node->LR[beta][r] = B[beta][r];
            }
        }
    }

    // Compute C[l][alpha] for all [1, n/2]
    map<double, map<int, PatienceNode*>> C;
    for (int alpha = 1; alpha < ceil(barrier); alpha++) {
        Node *mn_tree = new Node(start, mid);
        if (alpha > 1) 
            l_range_tree(mn_tree, C[alpha-1], alpha-1);
        for (int l = mid; l >= start; l--) {
            int a_l = seq[l];
            // C[alpha][l].resize(alpha);
            // node->LR[alpha][l].resize(alpha);
            if (alpha == 1) {
                C[alpha][l] = new PatienceNode(a_l);
                C[alpha][l]->base = a_l;
                C[alpha][l]->peak = a_l;
            } else {
                int i = l_range_max_tree(mn_tree, l);

                if (i == -1) {
                    C[alpha][l] = nullptr;
                } else {
                    C[alpha][l] = new PatienceNode(a_l);
                    C[alpha][l]->next = C[alpha-1][i];
                    C[alpha][l]->ln = (C[alpha-1][i]->ln) + 1;
                    C[alpha][l]->base = a_l;
                    C[alpha][l]->peak = C[alpha-1][i]->peak;
                }

            }

            // Update L side of LR 
            if (l < mid && 
                node->LR[alpha][l+1] && (
                !C[alpha][l] || node->LR[alpha][l+1]->peak < C[alpha][l]->peak
            )) {
                node->LR[alpha][l] = node->LR[alpha][l+1];
            } else {
                node->LR[alpha][l] = C[alpha][l];
            }
            // cout << "Setting LR[" << alpha << "][" << l << "] to ";
            // if (!node->LR[alpha][l])  {
            //     cout << "NONE\n";
            // } else {
            //     for (PatienceNode* ptr = node->LR[alpha][l]; ptr != nullptr; ptr = ptr->next) {
            //         cout << ptr->val << ",";
            //     }
            //     cout << endl;
            // }
        }

    }
    if (mid+1 == end) return;

    node->left = new ShortNode(start, mid);
    node->right = new ShortNode(mid+1, end);
    preprocess_dp(node->left), preprocess_dp(node->right);
}

int RLIS::compatible_beta(
    map<double, map<int, PatienceNode*>>& LR, 
    int peak, 
    query_t q,
    ShortNode *node
) {
    int r = q.second;
    int best_beta = 0;
    // binary search 
    int lo = 1, hi = min((int)ceil(barrier), node->end - node->mid + 1);
    while (lo < hi) {
        int beta = (lo + hi) / 2;
        if (LR[beta][r] && peak < LR[beta][r]->base) {
            best_beta = beta;
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
        map<double, map<int, PatienceNode*>> LR = node->LR;
        // Alpha = 0
        int alpha = 0, beta = compatible_beta(LR, -1e5, q, node);
        int longest_alpha_beta = LR[beta][j]->ln;
        pair<int, int> alpha_beta = {0, beta};

        // 1 <= alpha < c*sqrt(n)
        for (alpha = 1; alpha < ceil(barrier); alpha++) {
            if (!LR[alpha][i]) continue;
            int peak = LR[alpha][i]->peak;
            // Find corresponding beta to maximize alpha + beta
            int beta = compatible_beta(LR, peak, q, node);
            int ln = beta != 0 ? LR[alpha][i]->ln + LR[beta][j]->ln : LR[alpha][i]->ln;
            if (ln > longest_alpha_beta) {
                alpha_beta = {alpha, beta};
                longest_alpha_beta = ln;
            }
        }
        alpha = alpha_beta.first, beta = alpha_beta.second;

        vector<int> result;
        if (alpha != 0) {
            for (PatienceNode *alpha_ptr = LR[alpha][i]; alpha_ptr != nullptr; alpha_ptr = alpha_ptr->next) {
                result.push_back(alpha_ptr->val);
            }
        }
        if (beta != 0) {
            for (PatienceNode *beta_ptr = LR[beta][j]; beta_ptr != nullptr; beta_ptr = beta_ptr->next) {
                result.push_back(beta_ptr->val);
            }
            reverse(result.begin()+alpha, result.end());
        }
        return result;
    } else if (j <= node->mid) {
        return short_query(node->left, q);
    } else {
        return short_query(node->right, q);
    }
    return {};
}
