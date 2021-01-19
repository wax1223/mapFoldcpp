#include <iostream>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <cstdint>
#include <vector>
#include <cassert>


enum MV
{
    UNUSED, // The unused is setted for map to key function
    M,
    V
};
using Key = uint32_t;
using LinerOrdering = std::vector<uint8_t>;
using IndegreeList = std::vector<std::vector<uint8_t>>;
using MVPattern = std::vector<MV>;

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
    int rc = m.rowMvs.size();
    int cc = m.columnMvs[0].size();

    idl.resize(rc);

    for (auto &i : idl)
    {
        i.resize(cc);
    }

    //rows
    for (int r = 0; r < rc; r++)
    {
        auto &thisrow = m.rowMvs[r];
        for (int i = 0; i < thisrow.size(); i++)
        {
            bool isFront = r % 2 == i % 2 ? true : false;
            if (thisrow[i] == M && isFront)
            {
                idl[r][i + 1]++;
            }
            else if (thisrow[i] == M && !isFront)
            {
                idl[r][i]++;
            }
            else if (thisrow[i] == V && isFront)
            {
                idl[r][i]++;
            }
            else
            {
                idl[r][i + 1]++;
            }
        }
    }
    //columns
    for (int r = 0; r < rc - 1; r++)
    {
        auto &thisColumn = m.columnMvs[r];
        for (int j = 0; j < thisColumn.size(); j++)
        {
            bool isFront = r % 2 == j % 2 ? true : false;
            if (thisColumn[j] == M && isFront)
            {
                idl[r + 1][j]++;
            }
            else if (thisColumn[j] == M && !isFront)
            {
                idl[r][j]++;
            }
            else if (thisColumn[j] == V && isFront)
            {
                idl[r][j]++;
            }
            else
            {
                idl[r + 1][j]++;
            }
        }
    }

    return idl;
}

bool testLinearOrdering(IndegreeList idl, LinerOrdering l)
{
    return false;
}

//TODO: maybe add a reference parameter for map and then we will
// no need to allocate memory each time when generating MV map.
Map genMVmap(LinerOrdering l, int rowCount, int columnCount)
{
    Map m;
    m.rowMvs.resize(rowCount);
    m.columnMvs.resize(rowCount - 1);

    std::vector<std::vector<uint8_t>> invertIndexList;
    invertIndexList.resize(rowCount);
    for (auto &l : invertIndexList)
    {
        l.resize(columnCount);
    }

    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            invertIndexList[i][j] = l[i * rowCount + j];
        }
    }

    //Rows
    for (int r = 0; r < rowCount; r++)
    {
        auto &thisRow = m.rowMvs[r];
        thisRow.reserve(columnCount - 1);
        for (int c = 0; c < columnCount - 1; c++)
        {
            bool isUpper = invertIndexList[r][c] > invertIndexList[r][c + 1] ? true : false;
            bool isFront = r % 2 == c % 2 ? true : false;

            if (isFront && isUpper)
            {
                thisRow.push_back(V);
            }
            else if (isFront && !isUpper)
            {
                thisRow.push_back(M);
            }
            else if (!isFront && isUpper)
            {
                thisRow.push_back(M);
            }
            else
            {
                thisRow.push_back(V);
            }
        }
    }

    //Columns
    for (int r = 0; r < rowCount - 1; r++)
    {
        auto &thisColumn = m.columnMvs[r];
        thisColumn.reserve(columnCount);
        for (int c = 0; c < columnCount; c++)
        {
            bool isUpper = invertIndexList[r][c] > invertIndexList[r + 1][c] ? true : false;
            bool isFront = r % 2 == c % 2 ? true : false;

            if (isFront && isUpper)
            {
                thisColumn.push_back(V);
            }
            else if (isFront && !isUpper)
            {
                thisColumn.push_back(M);
            }
            else if (!isFront && isUpper)
            {
                thisColumn.push_back(M);
            }
            else
            {
                thisColumn.push_back(V);
            }
        }
    }

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
    // static std::unordered_map<int32_t, uint8_t> convertmap = {
    //     {1, 1}
    // };
    /*
    ['MVVV', '1'],
    ['VMVV', '2'],
    ['VVMV', '3'],
    ['VVVM', '4'],
    ['VMMM', '5'],
    ['MVMM', '6'],
    ['MMVM', '7'],
    ['MMMV', '8']
    */

    Key k;
    k = 0;
    int rc = m.rowMvs.size();
    int cc = m.rowMvs[0].size();
    auto &rowMv = m.rowMvs;
    auto &columnMv = m.rowMvs;

    return k;
}
/*
Map keyToMap(Key k)
{
    Map m;
    return m;
}
*/
void testPerm()
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

#define MoV(f) ((f > 1) ? 'V' : 'M')
void testGenMVmap()
{
    int rowCount = 2;
    int columnCount = 5;

    LinerOrdering l = initLinearOrdering(rowCount, columnCount);
    Map m = genMVmap(l, rowCount, columnCount);

    for (int r = 0; r < m.rowMvs.size(); r++)
    {
        auto &tr = m.rowMvs[r];
        for (int i = 0; i < tr.size(); i++)
        {
            std::cout << MoV(tr[i]);
        }
        std::cout << " ";
    }
    for (int c = 0; c < m.columnMvs.size(); c++)
    {
        auto &tc = m.columnMvs[c];
        for (int i = 0; i < tc.size(); i++)
        {
            std::cout << MoV(tc[i]);
        }
        std::cout << " ";
    }
}

void testIndexList()
{
    int rowCount = 2;
    int columnCount = 5;
    LinerOrdering l = initLinearOrdering(rowCount, columnCount);
    Map map = genMVmap(l, rowCount, columnCount);
    IndegreeList idl = genIndegreeList(l, map);

    for (auto &r : idl)
    {

        for (auto &c : r)
        {
            std::cout << (int)c;
        }
        std::cout << " ";
    }
}

void test()
{
    testPerm();
    testGenMVmap();
    testIndexList();
}

void start()
{
    MapFoldList mfl;
    int rowCount = 2;
    int columnCount = 2;
    LinerOrdering l = initLinearOrdering(rowCount, columnCount);
    do
    {
        Map map = genMVmap(l, rowCount, columnCount);
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
    test();
    return 0;
}
