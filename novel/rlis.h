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
    std::map<double, std::vector<std::vector<int>>> LR;

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
    std::vector<int> long_query(query_t query, std::map<int, std::vector<int>> sample);
    std::map<int, std::vector<int>> beta_sample(double beta);

    // Handling short LIS queries
    std::vector<int> r_range_max(
        std::map<double, std::vector<std::vector<int>>>& B,
        ShortNode *node,
        int r,
        int beta
    );
    std::vector<int> l_range_max(
        std::map<double, std::vector<std::vector<int>>>& C,
        ShortNode *node,
        int l,
        int alpha
    );

    // Standard LIS computations
    std::vector<int> forward_lis(int i, int j);
    std::vector<int> backward_lis(int i, int j);


};