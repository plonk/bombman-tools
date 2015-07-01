#include <iostream>
#include "picojson.h"
#include "game_state.h"

#include <clocale>
#include <cstdlib>

using namespace std;

const int Width = 15, Height = 15;

static wstring& at(vector<wstring>& map, int x, int y)
{
    return map[y * Width + x];
}

static wstring& at(vector<wstring>& map, Coords coords)
{
    return map[coords[1] * Width + coords[0]];
}

static wstring& at(vector<wstring>& map, Position pos)
{
    return map[pos.y * Width + pos.x];
}

#ifdef _WIN32
#include <windows.h>
wstring utf8_to_wstring(string orig)
{
    int bufsiz = MultiByteToWideChar(CP_UTF8, 0, orig.c_str(), orig.size(), 
                                     NULL, 0);
    wchar_t* buf = new wchar_t[bufsiz + 1];
    MultiByteToWideChar(CP_UTF8, 0, orig.c_str(), orig.size(), 
                        buf, bufsiz + 1);
    wstring wstr(buf);
    delete [] buf;
    return wstr;
}
#else
wstring utf8_to_wstring(string orig)
{
    char *old_locale = setlocale(LC_ALL, "C.UTF-8");

    if (!old_locale) {
        throw std::runtime_error("failed to set locale");
    }
        

    size_t wsize = mbstowcs(NULL, orig.c_str(), 0);
    if (wsize == (size_t) -1) {
        throw std::runtime_error("failed to convert to wide string");
    }
    wchar_t* wbuf = new wchar_t[wsize + 1];
    mbstowcs(wbuf, orig.c_str(), wsize+1);

    wstring res(wbuf);
    delete [] wbuf;

    setlocale(LC_ALL, old_locale);

    return res;
}
#endif

#ifdef _WIN32
string wstring_to_native(wstring orig)
{
    int bufsiz = WideCharToMultiByte(CP_ACP, 0, orig.c_str(), -1,
                                     NULL, 0,
                                     NULL, NULL);
    char* buf = new char[bufsiz+1];
    WideCharToMultiByte(CP_ACP, 0, orig.c_str(), -1,
                        buf, bufsiz+1,
                        NULL, NULL);
    string res(buf);
    return res;
}
#else
string wstring_to_native(wstring orig)
{
    size_t bufsiz = wcstombs(NULL, orig.c_str(), -1);
    char* buf = new char[bufsiz+1];
    wcstombs(buf, orig.c_str(), bufsiz + 1);
    string res(buf);
    delete[] buf;
    return res;
}
#endif

wstring render_map(const GameState& game_state)
{
    vector<wstring> map(Width*Height);

    for (int y = 0; y < Height; y++)
        for (int x = 0; x < Width; x++)
            at(map, x, y) = L"　";
    
    for (auto coords : game_state.blocks)
        at(map, coords) = L"□";

    for (auto bomb : game_state.bombs)
        at(map, bomb.pos) = L"●";

    for (auto item : game_state.items)
        at(map, item.pos) = utf8_to_wstring(item.name);

    for (auto coords : game_state.fires)
        at(map, coords) = L"火";

    for (auto coords : game_state.walls)
        at(map, coords) = L"■";

    for (auto player : game_state.players)
        at(map, player.pos) = utf8_to_wstring(player.ch);

    wstring res;
    for (int y = 0; y < Height; y++) {
        for (int x = 0; x < Width; x++) {
            res += map[y * Width + x];
        }
        res += L"\n";
    }
    return res;
}

int
main ()
{
#ifndef _WIN32
    setlocale(LC_ALL, "");
#endif
    picojson::value v;

    cin >> v;

    GameState state (v);

    cout << wstring_to_native(render_map(v)).c_str();
}
