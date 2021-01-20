#include <iostream>
#include <algorithm>
#include <stack>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <chrono>

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
    using LinearOrderingList = std::vector<LinerOrdering>;
    std::unordered_map<Key, LinearOrderingList> map;
    char orderTocharTable[36] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
        'u', 'v', 'w', 'x', 'y', 'z'};

public:
    bool addKey(Key k)
    {
        Key bk = k;
        while (true)
        {
            int v = k / 10;
            if (v != 0)
            {
                k = v;
            }
            else
            {
                break;
            }
        }
        if (k > 4)
        {
            return false;
        }
        auto iter = map.find(bk);
        if (iter == map.end())
        {
            LinearOrderingList l;
            map[bk] = l;
        }
        return true;
    }
    void addElem(Key k, LinerOrdering l)
    {
        auto iter = map.find(k);
        assert(iter != map.end());
        map[k].push_back(l);
    }
    size_t getLen()
    {
        return map.size();
    }
    void print()
    {
        for (auto &kl : map)
        {
            Key k = kl.first;
            LinearOrderingList l = kl.second;
            std::string allLO;
            std::string oneLO;
            for (auto &&r : l)
            {
                oneLO.clear();
                if (!allLO.empty())
                    allLO += ',';
                for (auto &c : r)
                {
                    char t = orderTocharTable[c];
                    oneLO += t;
                }
                allLO += oneLO;
            }
            std::cout << k << ":" << allLO << std::endl;
        }
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
    int halflen = l.size() / 2;
    for (int i = 0; i < l.size(); i++)
    {
        const uint8_t e = l[i];
        int thisi = e / halflen;
        int thisj = e - halflen * thisi - 1;
        if (thisj == -1)
        {
            thisi--;
            thisj = halflen - 1;
        }
        if (idl[thisi][thisj] == 0)
        {
            if (thisi > 0)
            {
                // not in the first row
                if (idl[thisi - 1][thisj] > 0)
                    idl[thisi - 1][thisj]--;
            }

            if (thisi < idl.size() - 1)
            {
                // not in the last row
                if (idl[thisi + 1][thisj] > 0)
                    idl[thisi + 1][thisj]--;
            }

            if (thisj > 0)
            {
                //not in the first column
                if (idl[thisi][thisj - 1] > 0)
                    idl[thisi][thisj - 1]--;
            }
            if (thisj < halflen - 1)
            {
                if (idl[thisi][thisj + 1] > 0)
                    idl[thisi][thisj + 1]--;
            }
        }
        else
        {
            return false;
        }
    }
    for (auto &r : idl)
    {
        for (auto &c : r)
        {
            assert(c == 0);
        }
    }

    return true;
}

//TODO: maybe add a reference parameter for map and then we will
// no need to allocate memory each time when generating MV map.
Map genMVmap(LinerOrdering l, int rowCount, int columnCount)
{
    Map m;
    m.rowMvs.resize(rowCount);
    m.columnMvs.resize(rowCount - 1);

    // std::vector<std::vector<uint8_t>> indexList;
    std::vector<std::vector<uint8_t>> invertedIndexList;
    // indexList.resize(rowCount);
    invertedIndexList.resize(rowCount);
    for (int i = 0; i < invertedIndexList.size(); i++)
    {
        // indexList[i].resize(columnCount);
        invertedIndexList[i].resize(columnCount);
    }

    // for (int i = 0; i < rowCount; i++)
    // {
    //     for (int j = 0; j < columnCount; j++)
    //     {
    //         indexList[i][j] = l[i * rowCount + j];
    //     }
    // }

    //Invert index
    for (int i = 0; i < rowCount; i++)
    {
        for (int j = 0; j < columnCount; j++)
        {
            auto &v = l[i * columnCount + j];
            int thisi = v / columnCount;
            int thisj = v - thisi * columnCount - 1;
            if (thisj == -1)
            {
                thisi--;
                thisj = columnCount - 1;
            }
            invertedIndexList[thisi][thisj] = i * columnCount + j;
        }
    }

    //Rows
    for (int r = 0; r < rowCount; r++)
    {
        auto &thisRow = m.rowMvs[r];
        thisRow.reserve(columnCount - 1);
        for (int c = 0; c < columnCount - 1; c++)
        {
            bool isUpper = invertedIndexList[r][c] > invertedIndexList[r][c + 1] ? true : false;
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
            bool isUpper = invertedIndexList[r][c] > invertedIndexList[r + 1][c] ? true : false;
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
    std::stack<uint8_t> s;
    int halfLen = l.size() / 2;

    for (int i = 0; i < l.size(); i++)
    {
        uint8_t elem = l[i];

        if (halfLen % 2 != 0 && elem % halfLen == 0)
            continue;
        if (s.empty())
        {
            s.push(elem);
            continue;
        }

        uint8_t top = s.top();
        int k = elem % halfLen;
        if (k % 2 == 0 && elem - top == 1)
        {
            s.pop();
        }
        else if (k % 2 == 1 && top - elem == 1)
        {
            s.pop();
        }
        else
        {
            assert(elem != top);
            s.push(elem);
        }
    }
    return s.empty();
}

bool testWest(LinerOrdering l)
{
    std::stack<uint8_t> s;
    int halfLen = l.size() / 2;

    for (int i = 0; i < l.size(); i++)
    {
        uint8_t elem = l[i];

        if (elem % halfLen == 1 ||
            (halfLen % 2 == 0 && elem % halfLen == 0))
            continue;

        if (s.empty())
        {
            s.push(elem);
            continue;
        }

        uint8_t top = s.top();
        int k = elem % halfLen;
        if (k == 0)
            k = halfLen;

        if (k % 2 == 0 && top - elem == 1)
        {
            s.pop();
        }
        else if (k % 2 == 1 && elem - top == 1)
        {
            s.pop();
        }
        else
        {
            assert(elem != top);
            s.push(elem);
        }
    }
    return s.empty();
}

bool testNorth(LinerOrdering l)
{
    return true;
}

bool testSouth(LinerOrdering l)
{
    std::stack<uint8_t> s;
    int halfLen = l.size() / 2;

    for (int i = 0; i < l.size(); i++)
    {
        uint8_t elem = l[i];

        // if (halfLen % 2 == 0 && elem % halfLen == 1)
        //     continue;

        if (s.empty())
        {
            s.push(elem);
            continue;
        }

        uint8_t top = s.top();
        if (abs(top - elem) == halfLen)
        {
            s.pop();
        }
        else
        {
            assert(elem != top);
            s.push(elem);
        }
    }
    return s.empty();
}

Key mapToKey(Map m)
{
    static std::unordered_map<int32_t, uint8_t> convertmap = {
        {1222, 1},
        {2122, 2},
        {2212, 3},
        {2221, 4},
        {2111, 5},
        {1211, 6},
        {1121, 7},
        {1112, 8},
    };
    Key k;
    k = 0;
    int rc = m.rowMvs.size();
    int cc = m.rowMvs[0].size();
    auto &rowMv = m.rowMvs;
    auto &columnMv = m.columnMvs;
    auto &firstrow = m.rowMvs[0];
    auto &secondrow = m.rowMvs[1];
    for (int i = 0; i < rowMv[0].size(); i++)
    {
        uint8_t up = firstrow[i];
        uint8_t down = secondrow[i];
        uint8_t left = columnMv[0][i];
        uint8_t right = columnMv[0][i + 1];
        int v = up * 1000 + down * 100 + left * 10 + right;
        auto iter = convertmap.find(v);
        if (iter == convertmap.end())
            return 0;

        k = k * 10 + iter->second;
    }
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
void testMapToKey()
{
    int rowCount = 2;
    int columnCount = 5;
    // LinerOrdering l = initLinearOrdering(rowCount, columnCount);
    // LinerOrdering l = {1, 2, 4, 3}; //8
    // LinerOrdering l = {3,5,10,8,6,7,9,4,2,1}; // 4666
    LinerOrdering l = {1, 2, 4, 9, 7, 6, 8, 10, 5, 3}; // 8222
    Map map = genMVmap(l, rowCount, columnCount);
    IndegreeList idl = genIndegreeList(l, map);

    Key k = mapToKey(map);
    std::cout << k;
}

void testMfl()
{
    MapFoldList mfl;
    int rowCount = 2;
    int columnCount = 5;
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

        mfl.addKey(k);
    } while (nextPerm(l));
    std::cout << mfl.getLen() << std::endl;
}

void testTestLinearOrdergin()
{
    int rowCount = 2;
    int columnCount = 2;
    LinerOrdering l = {1, 3, 2, 4};
    Map map = genMVmap(l, rowCount, columnCount);
    IndegreeList idl = genIndegreeList(l, map);

    Key k = mapToKey(map);
    std::cout << testLinearOrdering(idl, l);
}
void testButterfly()
{
    LinerOrdering l = {1, 2, 4, 3};
    std::cout << testEast(l) << std::endl;
    std::cout << testWest(l) << std::endl;
    std::cout << testSouth(l) << std::endl;
}

void test()
{
    // testPerm();
    // testGenMVmap();
    // testIndexList();
    // testMapToKey();
    // testMfl();
    testTestLinearOrdergin();
    testButterfly();
}

void findmapfold(int rows, int columns)
{
    MapFoldList mfl;
    int rowCount = rows;
    int columnCount = columns;
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
    mfl.print();
    std::cout << "total: " << mfl.getLen() << std::endl;
}

int main(int argc, char const *argv[])
{
    // test();
    if (argc < 3)
        return 0;
    int rows = std::stoi(argv[1]);
    int columns = std::stoi(argv[2]);
    auto start = std::chrono::high_resolution_clock::now();
    findmapfold(rows, columns);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
    return 0;
}
