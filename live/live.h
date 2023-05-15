#ifndef LIVE_H
#define LIVE_H
#include "../utility.h"
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
    unsigned int n;

    std::vector<int> lis(unsigned int lb, unsigned int ub);
    unsigned int stack_search(std::vector<PatienceNode*>& stack_tops, int x);
};
#endif