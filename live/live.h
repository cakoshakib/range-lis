#include <vector>
#include <map>
#define INF 1e5

typedef std::map<std::pair<int,int>, int> query_map_t;
typedef std::pair<int,int> query_t;

class LISLive {
public:
    LISLive(std::vector<int> sequence, std::vector<query_t> queries);
    std::vector<std::vector<int>> run();

private:
    std::vector<int> seq;
    std::vector<query_t> queries;
    int n;

    std::vector<int> lis(int lb, int ub);
};