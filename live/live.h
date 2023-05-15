#include <vector>
#include <map>
#define INF 1e5

typedef std::map<std::pair<int,int>, int> query_map_t;
typedef std::pair<int,int> query_t;

class PatienceNode {
public:
    int val, ln;
    PatienceNode *next;
    PatienceNode(int val) {
        this->val = val;
        ln = 0;
        next = nullptr;
    }
};

class LISLive {
public:
    LISLive(std::vector<int> sequence, std::vector<query_t> queries);
    std::vector<std::vector<int>> run();

private:
    std::vector<int> seq;
    std::vector<query_t> queries;
    unsigned int n;

    std::vector<int> lis(unsigned int lb, unsigned int ub);
    unsigned int stack_search(std::vector<PatienceNode*>& stack_tops, int x);
};