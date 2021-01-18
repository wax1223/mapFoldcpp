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
    int rowCount;
    int columnCount;
};

class MapFoldList
{
    Key key;
    std::vector<LinerOrdering> validLinearOrdering;

public:
    bool addKey(Key k)
    {
        return false;
    }
    void addElem(Key k, LinerOrdering l)
    {
    }
};

bool nextPerm(LinerOrdering &p)
{
    return std::next_permutation(p.begin(), p.end());
}

LinerOrdering initLinearOrdering(int r, int c)
{
    assert(r > 1);
    assert(c > 1);
    LinerOrdering l;
    l.reserve(r * c);
    for (int i = 0; i < r * c; i++)
    {
        l.push_back(i + 1);
    }
    return l;
}

int two_times_n_map()
{
    return 0;
}

IndegreeList genIndegreeList(LinerOrdering l, Map m)
{
    IndegreeList idl;
    return idl;
}

bool testLinearOrdering(IndegreeList idl, LinerOrdering l)
{
    return false;
}

Map genMVmap(LinerOrdering l)
{
    Map m;
    return m;
}

bool testEast(LinerOrdering l)
{
    return false;
}

bool testWest(LinerOrdering l)
{
    return false;
}

bool testNorth(LinerOrdering l)
{
    return false;
}

bool testSouth(LinerOrdering l)
{
    return false;
}

Key mapToKey(Map m)
{
    (void)(m);
    Key k;
    k = 0;
    return k;
}

Map keyToMap(Key k)
{
    Map m;
    return m;
}

void TestPerm()
{
    int rowCount = 2;
    int columnCount = 2;
    LinerOrdering l = initLinearOrdering(rowCount, columnCount);
    do
    {
        for (int i : l)
        {
            std::cout << i;
        }
        std::cout << " ";
    } while (nextPerm(l));
}
void Test()
{
    TestPerm();
}

void start()
{
    MapFoldList mfl;
    int rowCount = 2;
    int columnCount = 2;
    LinerOrdering l = initLinearOrdering(rowCount, columnCount);
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
}

int main(int argc, char const *argv[])
{
    Test();
    return 0;
}
