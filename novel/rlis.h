#include <vector>
#include <utility>
#include <iostream>
#include <map>
#include <set>
#define INF 1e5

typedef std::map<std::pair<int,int>, int> query_map_t;
typedef std::pair<int,int> query_t;

class ApproxNode {
public:
    int mid, start, end;
    int data;
    ApproxNode *left, *right;
    std::vector<int> approx;
    ApproxNode(int start, int end) {
        this->start = start;
        this->end = end;
        this->mid = mid_point(start, end);
        this->left = nullptr;
        this->right = nullptr;
        std::vector<int> approximation(end-start+1);
        this->approx = approximation;
    }

    int mid_point(int x, int y) {
        return x + ((y - x) / 2);
    }

};

class RLIS {
public:
    RLIS(std::vector<int> sequence, std::vector<query_t> queries);
    std::vector<std::vector<int>> run();
    std::vector<int> forward_lis(int i, int j);
    std::vector<int> backward_lis(int i, int j);
    query_map_t two_approx();
    std::map<int, std::vector<int>> beta_sample(double beta);
    std::vector<int> long_query(query_t query, std::map<int, std::vector<int>> sample);

private:
    // Variables
    std::vector<int> seq;
    std::vector<query_t> queries;
    int c = 2;
    int n;

    // Primary Algorithms

    // Helper Functions
    void build_approx_tree(ApproxNode *node);
    int answer_query(ApproxNode *node, int i, int j);

};