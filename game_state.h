#include <vector>
#include <tuple>
#include <algorithm>
#include <map>
#include <functional>

using namespace std;
using namespace picojson;

typedef function<void (value)> Setter;
typedef map<string, Setter> SetterMap;

template <typename T>
static vector<T> array_to_vector(value arrval);
template <>
vector<int> array_to_vector(value arrval);

class Coords
{
public:
    Coords(value v)
    {
        _data = array_to_vector<int>(v);
    }

    int operator[] (int i)
    {
        return _data[i];
    }
private:
    vector<int> _data;
};

static void set_member_vars(const object& obj, const SetterMap& setters)
{
    for_each(obj.begin(),
             obj.end(),
             [&](pair<string,value> obj)
    {
        auto it = setters.find(obj.first);
        if (it != setters.end())   // has key?
        {
            it->second(obj.second);
        }
        else
        {
            cerr << "set_member_vars: setter for '" << obj.first << "' not found." << endl;
        }
    });
}

class Position
{
public:
    int x;
    int y;

    Position(value v)
    {
        object obj = v.get<object>();
        set_member_vars(obj,
        {
            {"x", [&](value u)
                {
                    x = (int) u.get<double>();
                }
            },
            {"y", [&](value u)
                {
                    y = (int) u.get<double>();
                }
            },
        });
    }

    Position() : x(0), y(0)
    {
    }
};

class Player
{
public:
    int id;
    string name;
    Position pos;
    int power;
    int setBombLimit;
    bool isAlive;
    int setBombCount;
    int totalSetBombCount;
    string ch;

    Player(value v)
    {
        object obj = v.get<object>();
        set_member_vars(obj,
        {
            {"id", [&](value u)
                {
                    id = (int) u.get<double>();
                }
            },
            {"name", [&](value u)
                {
                    name = u.to_str();
                }
            },
            {"pos", [&](value u)
                {
                    pos = Position(u);
                }
            },
            {"power", [&](value u)
                {
                    power = (int) u.get<double>();
                }
            },
            {"setBombLimit", [&](value u)
                {
                    setBombLimit = (int) u.get<double>();
                }
            },
            {"isAlive", [&](value u)
                {
                    isAlive = u.get<bool>();
                }
            },
            {"setBombCount", [&](value u)
                {
                    setBombCount = (int) u.get<double>();
                }
            },
            {"totalSetBombCount", [&](value u)
                {
                    totalSetBombCount = (int) u.get<double>();
                }
            },
            {"ch", [&](value u)
                {
                    ch = u.to_str();
                }
            },
        });
    }
};

class Bomb
{
public:
    Position pos;
    int timer;
    int power;

    Bomb(value v)
    {
        object obj = v.get<object>();
        set_member_vars(obj,
        {
            {"pos", [&](value u)
                {
                    pos = Position(u);
                }
            },
            {"timer", [&](value u)
                {
                    timer = (int) u.get<double>();
                }
            },
            {"power", [&](value u)
                {
                    power = (int) u.get<double>();
                }
            },
        });
    }
};


class Item
{
public:
    Position pos;
    string name;

    Item(value v)
    {
        object obj = v.get<object>();
        set_member_vars(obj,
        {
            {"pos", [&](value u)
                {
                    pos = Position(u);
                }
            },
            {"name", [&](value u)
                {
                    name = u.to_str();
                }
            },
        });
    }
};

template <typename T>
static vector<T> array_to_vector(value arrval)
{
    picojson::array arr = arrval.get<picojson::array>();
    vector<T> res;

    transform(arr.begin(), arr.end(), back_inserter(res),
              [&](value v)
    {
        return T(v);
    });
    return res;
}

template <>
vector<int> array_to_vector(value arrval)
{
    picojson::array arr = arrval.get<picojson::array>();
    vector<int> res;

    transform(arr.begin(), arr.end(), back_inserter(res),
              [&](value v)
    {
        return (int) v.get<double>();
    });
    return res;
}

class GameState
{
public:
    int turn;
    vector<Coords> walls;
    vector<Coords> blocks;
    vector<Player> players;
    vector<Bomb>   bombs;
    vector<Item>   items;
    vector<Coords> fires;

    GameState(value v)
    {
        object obj = v.get<object>();
        set_member_vars(obj,
        {
            {"turn", [&](value u)
                {
                    turn = (int) u.get<double>();
                }
            },
            {"walls", [&](value u)
                {
                    walls = array_to_vector<Coords>(u);
                }
            },
            {"blocks", [&](value u)
                {
                    blocks = array_to_vector<Coords>(u);
                }
            },
            {"players", [&](value u)
                {
                    players = array_to_vector<Player>(u);
                }
            },
            {"bombs", [&](value u)
                {
                    bombs = array_to_vector<Bomb>(u);
                }
            },
            {"items", [&](value u)
                {
                    items = array_to_vector<Item>(u);
                }
            },
            {"fires", [&](value u)
                {
                    fires = array_to_vector<Coords>(u);
                }
            },
        });
    }
};
