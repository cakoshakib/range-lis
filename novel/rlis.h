#include "../utility.h"
#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <set>
#define INF 1e5

typedef std::map<std::pair<int,int>, int> query_map_t;
typedef std::pair<int,int> query_t;

class Node {
public:
    int mid, start, end;
    Node *left, *right;
    std::vector<int> list;
    std::vector<int> l1;
    std::vector<std::pair<int,int>> l2;

    Node(int start, int end) {
        // Start, Mid, End of interval
        this->start = start;
        this->end = end;
        this->mid = mid_point(start, end);
        // Left and right nodes of tree
        this->left = nullptr;
        this->right = nullptr;
        // Actual data (list of values)
        std::vector<int> list(end-start+1);
        this->list= list;
    }

    int mid_point(int x, int y) {
        return x + ((y - x) / 2);
    }

};

class ShortNode {
public:
    int mid, start, end;
    ShortNode *left, *right;
    std::map<double, std::map<int, PatienceNode*>> LR;

    ShortNode(int start, int end) {
        // Start, Mid, End of interval
        this->start = start;
        this->end = end;
        this->mid = mid_point(start, end);
        // Left and right nodes of tree
        this->left = nullptr;
        this->right = nullptr;
    }

    int mid_point(int x, int y) {
        return x + ((y - x) / 2);
    }

};

class RLIS {
public:
    RLIS(std::vector<int> sequence, std::vector<query_t> queries);
    std::vector<std::vector<int>> run();
    std::vector<int> short_query(query_t query);
    void preprocess_dp(ShortNode *node);
    // Standard LIS computations
    std::vector<PatienceNode*> forward_lis(unsigned int lb, unsigned int ub);
    std::vector<PatienceNode*> backward_lis(int lb, int ub);

private:
    // Variables
    std::vector<int> seq;
    std::vector<query_t> queries;
    int c = 2;
    int n;
    double barrier;

    // Building two approximation 
    void build_approx_tree(Node *node);
    int answer_approx_query(Node *node, int i, int j);
    query_map_t two_approx();

    // Handling long LIS querires
    std::vector<int> long_query(query_t query, std::map<int, std::vector<PatienceNode*>> sample);
    std::map<int, std::vector<PatienceNode*>> beta_sample(double beta);
    unsigned int stack_search(std::vector<PatienceNode*>& stack_tops, int x);
    unsigned int bkw_stack_search(std::vector<PatienceNode*>& stack_tops, int x);

    // Handling short LIS queries
    std::vector<int> short_query(ShortNode *node, query_t q);

    int compatible_beta(
        std::map<double, std::map<int, PatienceNode*>>& LR, 
        int peak, 
        query_t q,
        ShortNode *node
    );
    void r_range_tree(Node *node, std::map<int, PatienceNode*>& B);
    std::vector<int> r_range_max(
        std::map<double, std::map<int, std::vector<int>>>& B,
        ShortNode *node,
        int r,
        unsigned int beta
    );
    int r_range_max_tree(Node *node, int r);

    void l_range_tree(Node *node, std::map<int, PatienceNode*>& C_alpha);
    int l_range_max_tree(Node *node, int l);

    void l_range_tree(Node *node);
    std::vector<int> l_range_max(
        std::map<double, std::map<int, std::vector<int>>>& C,
        ShortNode *node,
        int l,
        unsigned int alpha
    );



};