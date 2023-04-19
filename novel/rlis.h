#include <vector>
#include <utility>
#include <map>

typedef std::map<std::pair<int,int>, int> query_map_t;
typedef std::pair<int,int> query_t;

class RLIS {
public:
    RLIS(std::vector<int> sequence, std::vector<query_t> queries);
    std::vector<int> run();

private:
    // Variables
    std::vector<int> seq;
    std::vector<query_t> queries;

    // Primary Algorithms
    query_map_t two_approx();

    // Helper Functions
    void build_approx_tree(ApproxNode *node);

};