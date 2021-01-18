#include <iostream>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <cassert>

enum MV
{
    M,
    V
};
using Key = uint32_t;
using LinerOrdering = std::vector<uint8_t>;
using IndegreeList = std::vector<std::vector<uint8_t>>;

struct MVPattern
{
    std::vector<MV> mvs;
};

struct Map
{
    std::vector<MVPattern> rowMvs;
    std::vector<MVPattern> columnMvs;
};

class MapFoldList
{
    Key key;
    std::vector<LinerOrdering> validLinearOrdering;

public:
    bool addKey(Key k)
    {
    }
    void addElem(Key k, LinerOrdering l)
    {
    }
};

bool nextPerm(LinerOrdering &p)
{
    return std::next_permutation(p.begin(), p.end());
}

LinerOrdering initLinearOrdering(int n)
{
    assert(n > 1);
    LinerOrdering l;
    l.reserve(n * 2);
    for (int i = 0; i < n * 2; i++)
    {
        l.push_back(i);
    }
    return l;
}

int two_times_n_map()
{
}

IndegreeList genIndegreeList(LinerOrdering l, Map m)
{
}

bool testLinearOrdering(IndegreeList idl, LinerOrdering l)
{
}

Map genMVmap(LinerOrdering l)
{
}

bool testEast(LinerOrdering l)
{
}

bool testWest(LinerOrdering l)
{
}

bool testNorth(LinerOrdering l)
{
}

bool testSouth(LinerOrdering l)
{
}

Key mapToKey(Map m)
{
}

Map keyToMap(Key k)
{
}

int main(int argc, char const *argv[])
{

    MapFoldList mfl;
    LinerOrdering l = initLinearOrdering(2);
    do
    {
        Map map = genMVmap(l);
        IndegreeList idl = genIndegreeList(l, map);

        Key k = mapToKey(map);

        // Not a valid mv pattern
        if (k <= 0)
        {
            continue;
        }

        if (mfl.addKey(k))
        {
            if (testLinearOrdering(idl, l) && testSouth(l) && testEast(l) && testWest(l))
            {
                mfl.addElem(k, l);
            }
        }
    } while (nextPerm(l));

    return 0;
}
