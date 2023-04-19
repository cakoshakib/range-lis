#include <vector>
#include <utility>

class RLIS {
public:
    RLIS(std::vector<int> sequence, std::vector<std::pair<int,int>> queries);
    std::vector<int> run();

private:
    std::vector<int> seq;
    std::vector<std::pair<int,int>> queries;

};