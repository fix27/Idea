// Modified Game of Life (B356/S2) - OpenGL 2D orthographic view
// Compile: g++ -std=c++17 -O2 life_gl.cpp -o life_gl.exe ^
//   -I"%USERPROFILE%\scoop\apps\glfw\3.4\include" ^
//   -L"%USERPROFILE%\scoop\apps\glfw\3.4\lib-mingw-w64" ^
//   -lglfw3 -lopengl32 -lgdi32

#define _USE_MATH_DEFINES
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cmath>
#include <deque>
#include <algorithm>
#include <ctime>
#include <windows.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <gl/GL.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

const int MAX_W = 1000, MAX_H = 1000;
const int GW = 2 * MAX_W, GH = 2 * MAX_H;
const uint8_t MASK_ALIVE = 0x01, MASK_MARK = 0x10, MASK_BOTH = 0x11;
static const int NX[10] = { 0, 0, 0, 0, -1, 1,  1,  1, -1, -1 };
static const int NY[10] = { 2,-2, 1,-1,  0, 0,  1, -1,  1, -1 };
const float CELL_SCALE = 0.8f;

// ─── B356/S2 pattern presets ────────────────────────────
#include <string>
#include <vector>
struct Pattern { std::string name; int w, h; std::string data; };
std::vector<Pattern> patterns;

void init_patterns() {
    auto add_raw = [&](const char* n,int w,int h,const char* d) {
        patterns.push_back({n,w,h,d});
    };
    // Transformation helpers
    auto txt_cells = [](const char* t) {
        std::vector<std::pair<int,int>> r;
        int row=0,col=0;
        for (; *t; t++) {
            if (*t=='\n') { row++; col=0; continue; }
            if (*t=='X'||*t=='x') r.push_back({row,col});
            col++;
        }
        return r;
    };
    auto norm = [](std::vector<std::pair<int,int>> c) {
        if (c.empty()) return c;
        int mr=c[0].first,mc=c[0].second;
        for (auto& p:c) { mr=std::min(mr,p.first); mc=std::min(mc,p.second); }
        for (auto& p:c) { p.first-=mr; p.second-=mc; }
        return c;
    };
    auto bb_h = [](const std::vector<std::pair<int,int>>& c) {
        int lo=c[0].first,hi=c[0].first;
        for (auto& p:c) { lo=std::min(lo,p.first); hi=std::max(hi,p.first); }
        return hi-lo+1;
    };
    auto cells_to_data = [](const std::vector<std::pair<int,int>>& c, int& w, int& h) {
        if (c.empty()) { w=h=0; return std::string(); }
        int mr=c[0].first,mc=c[0].second,xr=c[0].first,xc=c[0].second;
        for (auto& p:c) { mr=std::min(mr,p.first); mc=std::min(mc,p.second); xr=std::max(xr,p.first); xc=std::max(xc,p.second); }
        h=xr-mr+1; w=xc-mc+1;
        std::string d(w*h,'0');
        for (auto& p:c) d[(p.first-mr)*w+(p.second-mc)]='1';
        return d;
    };
    auto add_cells = [&](const char* n, std::vector<std::pair<int,int>> cells, const char*) {
        int w,h; std::string d=cells_to_data(cells,w,h);
        patterns.push_back({n,w,h,std::move(d)});
    };
    auto add_text = [&](const char* n, const char* text) {
        auto c=norm(txt_cells(text)); int w,h; std::string d=cells_to_data(c,w,h);
        patterns.push_back({n,w,h,std::move(d)});
    };
// === Verified Unique Objects ===
// Generated from 4147 patterns, organized by behavior

// --- Still Lifes ---
add_cells("Still_1", {{{0,0},{0,1},{0,3},{1,3}}}, ""); // still pop=4
add_cells("Still_2", {{{0,0},{0,2},{1,0},{1,2}}}, ""); // still pop=4
add_cells("Still_4", {{{0,0},{0,2},{1,0},{1,2},{3,0},{3,2},{4,0},{4,2}}}, ""); // still pop=8
add_cells("Still_5", {{{0,0},{0,2},{0,7},{0,9},{1,0},{1,2},{1,7},{1,9}}}, ""); // still pop=8
// cat:Still Lifes:start=0 count=5

// --- Oscillators period=2 ---
add_cells("Osc_p2_1", {{{0,0},{0,1},{1,0},{1,1}}}, ""); // osc p=2 pop=4
add_cells("Osc_p2_2", {{{0,0},{0,3},{1,0},{1,3}}}, ""); // osc p=2 pop=4
add_cells("Osc_p2_3", {{{0,0},{0,2},{1,2},{1,3},{2,0},{2,2}}}, ""); // osc p=2 pop=6
add_cells("Osc_p2_4", {{{0,0},{0,3},{0,6},{1,0},{1,3},{1,6}}}, ""); // osc p=2 pop=6
add_cells("Osc_p2_5", {{{0,1},{0,3},{1,0},{1,1},{2,1},{2,3}}}, ""); // osc p=2 pop=6
add_cells("Osc_p2_6", {{{0,0},{0,1},{0,2},{1,0},{1,1},{1,2}}}, ""); // osc p=2 pop=6
add_cells("Osc_p2_7", {{{0,2},{1,0},{1,4},{2,0},{2,4},{3,2}}}, ""); // osc p=2 pop=6
add_cells("Osc_p2_14", {{{0,0},{0,1},{1,0},{1,1},{4,0},{4,1},{5,0},{5,1}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_15", {{{0,0},{0,1},{1,0},{1,1},{5,0},{5,1},{6,0},{6,1}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_18", {{{0,0},{0,3},{1,0},{1,3},{5,0},{5,3},{6,0},{6,3}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_19", {{{0,0},{0,1},{0,6},{0,9},{1,0},{1,1},{1,6},{1,9}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_20", {{{0,0},{0,3},{0,8},{0,11},{1,0},{1,3},{1,8},{1,11}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_23", {{{0,3},{1,0},{1,1},{1,5},{2,0},{2,1},{2,5},{3,3}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_25", {{{0,2},{1,0},{1,4},{1,5},{2,0},{2,4},{2,5},{3,2}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_26", {{{0,0},{0,1},{1,0},{1,1},{3,0},{3,1},{4,0},{4,1}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_27", {{{0,0},{0,1},{1,0},{1,1},{4,1},{4,2},{5,1},{5,2}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_29", {{{0,0},{0,3},{1,0},{1,3},{6,2},{6,3},{7,2},{7,3}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_30", {{{0,1},{0,2},{1,1},{1,2},{4,0},{4,1},{5,0},{5,1}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_31", {{{0,0},{0,1},{1,0},{1,1},{6,0},{6,3},{7,0},{7,3}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_32", {{{0,0},{0,1},{0,3},{0,4},{1,0},{1,1},{1,3},{1,4}}}, ""); // osc p=2 pop=8
add_cells("Osc_p2_33", {{{0,0},{0,1},{0,2},{0,6},{0,9},{1,0},{1,1},{1,2},{1,6},{1,9}}}, ""); // osc p=2 pop=10
add_cells("Osc_p2_36", {{{0,0},{0,2},{1,2},{1,3},{2,0},{2,2},{5,2},{5,3},{6,2},{6,3}}}, ""); // osc p=2 pop=10
add_cells("Osc_p2_41", {{{0,1},{0,2},{1,1},{1,2},{5,0},{5,3},{6,0},{6,3},{10,1},{10,2},{11,1},{11,2}}}, ""); // osc p=2 pop=12

// --- Oscillators period=4 ---
add_cells("Osc_p4_1", {{{0,0},{0,2},{1,1},{1,2}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_2", {{{0,0},{0,2},{1,0},{1,1}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_3", {{{0,3},{1,0},{1,1},{1,3}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_4", {{{0,0},{1,0},{1,2},{1,3}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_5", {{{0,0},{0,2},{0,3},{1,0}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_6", {{{0,1},{0,3},{1,0},{1,2}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_7", {{{0,0},{0,2},{1,1},{1,3}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_8", {{{0,1},{0,2},{1,0},{1,2}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_9", {{{0,2},{0,3},{1,0},{1,1}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_10", {{{0,0},{0,1},{1,2},{1,3}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_11", {{{0,0},{0,1},{1,0},{1,2}}}, ""); // osc p=4 pop=4
add_cells("Osc_p4_12", {{{0,0},{0,3},{0,4},{1,5},{2,0},{2,3},{2,4}}}, ""); // osc p=4 pop=7
add_cells("Osc_p4_13", {{{0,1},{0,2},{0,5},{1,0},{2,1},{2,2},{2,5}}}, ""); // osc p=4 pop=7
add_cells("Osc_p4_14", {{{0,0},{0,2},{1,0},{1,1},{6,0},{6,1},{7,0},{7,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_15", {{{0,0},{0,1},{1,0},{1,2},{5,0},{5,2},{6,0},{6,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_18", {{{0,0},{0,2},{1,1},{1,2},{6,3},{6,4},{7,3},{7,5}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_21", {{{0,1},{0,2},{1,0},{1,2},{3,0},{3,2},{4,1},{4,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_22", {{{0,0},{0,2},{1,0},{1,1},{3,0},{3,1},{4,0},{4,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_23", {{{0,0},{0,2},{1,0},{1,1},{4,0},{4,1},{5,0},{5,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_24", {{{0,0},{0,2},{1,0},{1,1},{5,0},{5,1},{6,0},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_25", {{{0,0},{0,2},{1,1},{1,2},{4,1},{4,2},{5,0},{5,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_26", {{{0,0},{0,2},{1,1},{1,2},{5,1},{5,2},{6,0},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_33", {{{0,0},{0,1},{1,0},{1,2},{3,0},{3,2},{4,0},{4,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_34", {{{0,0},{0,2},{1,1},{1,2},{3,1},{3,2},{4,0},{4,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_38", {{{0,0},{0,1},{1,0},{1,2},{4,4},{4,6},{5,5},{5,6}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_39", {{{0,5},{0,6},{1,4},{1,6},{4,0},{4,2},{5,0},{5,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_40", {{{0,3},{0,4},{1,3},{1,5},{5,0},{5,1},{6,2},{6,3}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_41", {{{0,0},{0,1},{1,0},{1,2},{4,0},{4,2},{5,0},{5,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_42", {{{0,0},{0,1},{1,0},{1,2},{4,0},{4,2},{5,0},{5,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_43", {{{0,1},{0,2},{1,1},{1,3},{6,0},{7,0},{7,2},{7,3}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_44", {{{0,1},{0,3},{1,2},{1,3},{6,0},{6,1},{7,0},{7,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_48", {{{0,5},{0,6},{1,4},{1,6},{4,0},{4,2},{4,3},{5,0}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_49", {{{0,7},{0,9},{1,0},{1,1},{1,7},{1,8},{2,0},{2,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_55", {{{0,0},{0,2},{1,1},{1,2},{4,5},{4,6},{4,8},{5,8}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_59", {{{0,4},{0,6},{1,5},{1,6},{4,0},{4,2},{5,1},{5,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_61", {{{0,7},{0,10},{1,7},{1,10},{3,1},{3,2},{4,0},{4,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_62", {{{0,0},{0,2},{1,1},{1,2},{3,7},{3,10},{4,7},{4,10}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_67", {{{0,1},{0,2},{1,0},{1,2},{5,0},{5,2},{6,1},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_68", {{{0,0},{0,2},{0,3},{1,0},{2,9},{2,10},{3,8},{3,10}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_69", {{{0,0},{0,2},{0,3},{1,0},{1,8},{1,10},{2,9},{2,10}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_71", {{{0,0},{0,1},{0,3},{1,3},{5,4},{5,5},{6,4},{6,5}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_77", {{{0,0},{0,3},{1,0},{1,3},{2,8},{2,10},{3,8},{3,9}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_78", {{{0,1},{0,2},{1,0},{1,2},{5,4},{5,5},{6,4},{6,6}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_79", {{{0,4},{0,6},{1,4},{1,5},{5,0},{5,2},{6,1},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_85", {{{0,0},{0,2},{1,0},{1,1},{3,8},{4,5},{4,6},{4,8}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_87", {{{0,0},{0,1},{1,0},{1,1},{3,7},{3,8},{4,6},{4,8}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_98", {{{0,0},{0,1},{1,0},{1,2},{1,8},{1,9},{2,7},{2,9}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_99", {{{0,0},{0,1},{1,0},{1,2},{3,4},{3,6},{4,5},{4,6}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_106", {{{0,3},{0,4},{1,2},{1,4},{6,2},{6,3},{7,0},{7,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_107", {{{0,6},{0,7},{0,9},{1,9},{2,0},{2,2},{3,1},{3,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_109", {{{0,0},{0,2},{1,0},{1,1},{3,4},{3,5},{4,4},{4,5}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_111", {{{0,3},{0,4},{1,3},{1,4},{6,0},{6,1},{7,0},{7,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_113", {{{0,5},{0,6},{1,5},{1,7},{4,0},{4,3},{5,0},{5,3}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_116", {{{0,0},{0,2},{0,3},{1,0},{2,7},{2,9},{3,7},{3,8}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_118", {{{0,0},{0,2},{0,3},{1,0},{5,0},{5,2},{6,0},{6,1}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_120", {{{0,1},{0,2},{1,0},{1,2},{4,4},{4,7},{5,4},{5,7}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_121", {{{0,1},{0,2},{1,1},{1,2},{5,1},{5,2},{6,0},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_124", {{{0,3},{0,5},{1,4},{1,5},{5,0},{5,2},{6,0},{6,2}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_129", {{{0,0},{0,2},{1,0},{1,2},{5,4},{5,5},{6,3},{6,5}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_132", {{{0,1},{0,3},{1,1},{1,2},{4,0},{4,3},{5,0},{5,3}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_134", {{{0,0},{0,2},{1,1},{1,2},{4,0},{4,3},{5,0},{5,3}}}, ""); // osc p=4 pop=8
add_cells("Osc_p4_139", {{{0,0},{0,2},{0,3},{1,1},{1,2},{1,3},{2,0},{2,2},{2,3}}}, ""); // osc p=4 pop=9
add_cells("Osc_p4_140", {{{0,0},{0,1},{0,3},{1,0},{1,1},{1,2},{2,0},{2,1},{2,3}}}, ""); // osc p=4 pop=9
add_cells("Osc_p4_143", {{{0,0},{0,1},{0,3},{1,3},{4,6},{4,7},{4,8},{5,6},{5,7},{5,8}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_146", {{{0,3},{0,5},{1,5},{1,6},{2,3},{2,5},{7,3},{8,0},{8,1},{8,3}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_147", {{{0,0},{0,1},{0,3},{1,3},{6,3},{6,5},{7,5},{7,6},{8,3},{8,5}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_149", {{{0,1},{0,3},{1,0},{1,1},{2,1},{2,3},{7,3},{8,3},{8,5},{8,6}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_150", {{{0,3},{0,5},{0,6},{1,3},{6,1},{6,3},{7,0},{7,1},{8,1},{8,3}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_152", {{{0,0},{0,1},{1,0},{1,2},{2,7},{3,5},{3,9},{4,5},{4,9},{5,7}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_153", {{{0,2},{1,0},{1,4},{2,0},{2,4},{3,2},{6,5},{6,6},{7,5},{7,7}}}, ""); // osc p=4 pop=10
add_cells("Osc_p4_156", {{{0,9},{1,7},{1,11},{1,12},{2,0},{2,1},{2,3},{2,7},{2,11},{2,12},{3,3},{3,9}}}, ""); // osc p=4 pop=12

// --- Oscillators period=5 ---
add_cells("Osc_p5_1", {{{0,0},{0,1},{0,2},{0,3},{0,4},{0,5}}}, ""); // osc p=5 pop=6
add_cells("Osc_p5_2", {{{0,1},{0,4},{1,0},{1,5},{2,1},{2,4}}}, ""); // osc p=5 pop=6
add_cells("Osc_p5_3", {{{0,1},{0,4},{1,0},{1,5},{2,2},{2,3},{3,0},{3,5},{4,1},{4,4}}}, ""); // osc p=5 pop=10
add_cells("Osc_p5_4", {{{0,2},{0,3},{1,0},{1,5},{2,0},{2,1},{2,4},{2,5},{3,0},{3,5},{4,2},{4,3}}}, ""); // osc p=5 pop=12

// --- Oscillators period=10 ---
add_cells("Osc_p10_1", {{{0,0},{0,1},{0,2},{0,3},{1,0},{1,1},{1,2},{1,3},{2,0},{2,1},{2,2},{2,3},{6,0},{6,1},{6,2},{6,3},{7,0},{7,1},{7,2},{7,3},{8,0},{8,1},{8,2},{8,3}}}, ""); // osc p=10 pop=24

// cat:All Oscillators:start=5 count=99

// --- Ships period=2 ---
add_cells("Ship_p2_1", {{{0,0},{0,3},{1,0},{2,0},{2,3}}}, ""); // ship p=2 pop=5
add_cells("Ship_p2_2", {{{0,0},{0,3},{1,3},{2,0},{2,3}}}, ""); // ship p=2 pop=5
add_cells("Ship_p2_3", {{{0,1},{1,0},{1,1},{1,2},{1,3},{2,1}}}, ""); // ship p=2 pop=6
add_cells("Ship_p2_4", {{{0,2},{1,0},{1,1},{1,2},{1,3},{2,2}}}, ""); // ship p=2 pop=6
add_cells("Ship_p2_5", {{{0,0},{0,2},{0,3},{1,3},{2,0},{2,2},{2,3}}}, ""); // ship p=2 pop=7
add_cells("Ship_p2_6", {{{0,0},{0,1},{0,3},{1,0},{2,0},{2,1},{2,3}}}, ""); // ship p=2 pop=7
add_cells("Ship_p2_7", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,3},{2,0},{2,1},{2,3}}}, ""); // ship p=2 pop=9
add_cells("Ship_p2_8", {{{0,0},{0,2},{0,3},{1,0},{1,1},{1,3},{2,0},{2,2},{2,3}}}, ""); // ship p=2 pop=9
add_cells("Ship_p2_11", {{{0,2},{1,0},{1,1},{1,2},{1,3},{2,2},{4,2},{5,0},{5,1},{5,2},{5,3},{6,2}}}, ""); // ship p=2 pop=12
add_cells("Ship_p2_12", {{{0,1},{1,0},{1,1},{1,2},{1,3},{2,1},{4,1},{5,0},{5,1},{5,2},{5,3},{6,1}}}, ""); // ship p=2 pop=12
add_cells("Ship_p2_13", {{{0,0},{0,1},{1,0},{1,2},{2,0},{2,2},{2,4},{2,6},{3,0},{3,2},{4,0},{4,1}}}, ""); // ship p=2 pop=12

// --- Ships period=5 ---
add_cells("Ship_p5_1", {{{0,0},{0,3},{0,4},{1,2},{1,4},{2,2},{2,4},{3,0},{3,3},{3,4}}}, ""); // ship p=5 pop=10
add_cells("Ship_p5_2", {{{0,0},{0,1},{0,4},{1,0},{1,2},{2,0},{2,2},{3,0},{3,1},{3,4}}}, ""); // ship p=5 pop=10
add_cells("Ship_p5_3", {{{0,0},{0,1},{0,4},{1,0},{1,2},{1,4},{2,0},{2,2},{2,4},{3,0},{3,1},{3,4}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_4", {{{0,0},{0,3},{0,4},{1,0},{1,2},{1,4},{2,0},{2,2},{2,4},{3,0},{3,3},{3,4}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_5", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,5},{2,0},{2,2},{2,5},{3,0},{3,1},{3,3}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_6", {{{0,2},{0,4},{0,5},{1,0},{1,3},{1,5},{2,0},{2,3},{2,5},{3,2},{3,4},{3,5}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_7", {{{0,0},{0,1},{1,0},{1,2},{1,4},{1,6},{2,0},{2,2},{2,4},{2,6},{3,0},{3,1}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_8", {{{0,5},{0,6},{1,0},{1,2},{1,4},{1,6},{2,0},{2,2},{2,4},{2,6},{3,5},{3,6}}}, ""); // ship p=5 pop=12
add_cells("Ship_p5_9", {{{0,0},{0,1},{0,3},{0,4},{1,0},{1,2},{1,7},{2,0},{2,2},{2,7},{3,0},{3,1},{3,3},{3,4}}}, ""); // ship p=5 pop=14

// --- Ships period=8 ---
add_cells("Ship_p8_1", {{{0,2},{0,8},{0,10},{0,11},{1,0},{1,2},{1,6},{1,11},{2,2},{2,8},{2,10},{2,11}}}, ""); // ship p=8 pop=12
add_cells("Ship_p8_2", {{{0,0},{0,8},{0,10},{0,11},{1,2},{1,6},{1,7},{1,8},{1,9},{1,11},{2,0},{2,8},{2,10},{2,11}}}, ""); // ship p=8 pop=14
add_cells("Ship_p8_3", {{{0,8},{0,10},{0,11},{1,0},{1,1},{1,2},{1,6},{1,7},{1,8},{1,9},{1,11},{2,8},{2,10},{2,11}}}, ""); // ship p=8 pop=14
add_cells("Ship_p8_4", {{{0,0},{0,1},{0,3},{0,6},{0,11},{1,0},{1,6},{1,10},{1,11},{1,12},{2,0},{2,1},{2,3},{2,6},{2,11}}}, ""); // ship p=8 pop=15
add_cells("Ship_p8_5", {{{0,1},{0,6},{0,9},{0,11},{0,12},{1,0},{1,1},{1,2},{1,6},{1,12},{2,1},{2,6},{2,9},{2,11},{2,12}}}, ""); // ship p=8 pop=15
add_cells("Ship_p8_6", {{{0,0},{0,1},{0,3},{0,6},{1,0},{1,2},{1,3},{1,5},{1,6},{1,7},{1,8},{2,0},{2,1},{2,3},{2,6}}}, ""); // ship p=8 pop=15
add_cells("Ship_p8_7", {{{0,2},{0,5},{0,7},{0,8},{1,0},{1,1},{1,2},{1,3},{1,5},{1,6},{1,8},{2,2},{2,5},{2,7},{2,8}}}, ""); // ship p=8 pop=15
add_cells("Ship_p8_8", {{{0,0},{0,1},{0,3},{0,6},{0,11},{0,13},{1,0},{1,2},{1,3},{1,5},{1,6},{1,7},{1,8},{2,0},{2,1},{2,3},{2,6},{2,11},{2,13}}}, ""); // ship p=8 pop=19
add_cells("Ship_p8_9", {{{0,0},{0,2},{0,7},{0,10},{0,12},{0,13},{1,5},{1,6},{1,7},{1,8},{1,10},{1,11},{1,13},{2,0},{2,2},{2,7},{2,10},{2,12},{2,13}}}, ""); // ship p=8 pop=19
add_cells("Ship_p8_10", {{{0,7},{0,10},{1,0},{1,1},{1,3},{1,6},{1,7},{1,9},{2,0},{2,2},{2,6},{3,0},{3,2},{3,6},{4,0},{4,1},{4,3},{4,6},{4,7},{4,9},{5,7},{5,10}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_11", {{{0,0},{0,3},{1,1},{1,3},{1,4},{1,7},{1,9},{1,10},{2,4},{2,8},{2,10},{3,4},{3,8},{3,10},{4,1},{4,3},{4,4},{4,7},{4,9},{4,10},{5,0},{5,3}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_12", {{{0,12},{1,14},{2,0},{2,1},{2,4},{2,10},{2,12},{3,0},{3,2},{3,4},{3,5},{4,0},{4,2},{4,4},{4,5},{5,0},{5,1},{5,4},{5,10},{5,12},{6,14},{7,12}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_13", {{{0,12},{0,13},{0,14},{1,0},{1,1},{1,5},{1,6},{2,0},{2,2},{2,4},{2,7},{3,0},{3,2},{3,4},{3,7},{4,0},{4,1},{4,5},{4,6},{5,12},{5,13},{5,14}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_14", {{{0,2},{1,0},{2,2},{2,4},{2,10},{2,13},{2,14},{3,9},{3,10},{3,12},{3,14},{4,9},{4,10},{4,12},{4,14},{5,2},{5,4},{5,10},{5,13},{5,14},{6,0},{7,2}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_15", {{{0,0},{0,1},{0,2},{1,8},{1,9},{1,13},{1,14},{2,7},{2,10},{2,12},{2,14},{3,7},{3,10},{3,12},{3,14},{4,8},{4,9},{4,13},{4,14},{5,0},{5,1},{5,2}}}, ""); // ship p=8 pop=22
add_cells("Ship_p8_16", {{{0,10},{0,11},{0,12},{1,0},{1,1},{1,3},{1,9},{1,10},{1,13},{2,0},{2,2},{2,6},{3,0},{3,2},{3,6},{4,0},{4,1},{4,3},{4,9},{4,10},{4,13},{5,10},{5,11},{5,12}}}, ""); // ship p=8 pop=24
add_cells("Ship_p8_17", {{{0,1},{0,2},{0,3},{1,0},{1,3},{1,4},{1,10},{1,12},{1,13},{2,7},{2,11},{2,13},{3,7},{3,11},{3,13},{4,0},{4,3},{4,4},{4,10},{4,12},{4,13},{5,1},{5,2},{5,3}}}, ""); // ship p=8 pop=24
add_cells("Ship_p8_19", {{{0,9},{0,11},{1,0},{1,1},{1,5},{1,8},{1,11},{2,0},{2,2},{2,4},{2,9},{2,10},{2,11},{3,0},{3,2},{3,4},{3,9},{3,10},{3,11},{4,0},{4,1},{4,5},{4,8},{4,11},{5,9},{5,11}}}, ""); // ship p=8 pop=26
add_cells("Ship_p8_20", {{{0,7},{0,8},{0,10},{1,0},{1,1},{1,4},{1,10},{2,0},{2,2},{2,4},{2,5},{2,6},{2,9},{3,0},{3,2},{3,4},{3,5},{3,6},{3,9},{4,0},{4,1},{4,4},{4,10},{5,7},{5,8},{5,10}}}, ""); // ship p=8 pop=26
add_cells("Ship_p8_21", {{{0,0},{0,2},{0,3},{1,0},{1,6},{1,9},{1,10},{2,1},{2,4},{2,5},{2,6},{2,8},{2,10},{3,1},{3,4},{3,5},{3,6},{3,8},{3,10},{4,0},{4,6},{4,9},{4,10},{5,0},{5,2},{5,3}}}, ""); // ship p=8 pop=26
add_cells("Ship_p8_22", {{{0,14},{0,16},{1,7},{1,8},{2,0},{2,1},{2,5},{2,10},{2,14},{2,16},{3,0},{3,2},{3,4},{3,6},{4,0},{4,2},{4,4},{4,6},{5,0},{5,1},{5,5},{5,10},{5,14},{5,16},{6,7},{6,8},{7,14},{7,16}}}, ""); // ship p=8 pop=28
add_cells("Ship_p8_23", {{{0,1},{1,0},{1,1},{1,2},{2,1},{2,7},{2,8},{2,9},{2,10},{2,12},{2,14},{2,15},{3,7},{3,8},{3,9},{3,13},{3,15},{4,7},{4,8},{4,9},{4,13},{4,15},{5,1},{5,7},{5,8},{5,9},{5,10},{5,12},{5,14},{5,15},{6,0},{6,1},{6,2},{7,1}}}, ""); // ship p=8 pop=34

// --- Ships period=10 ---
add_cells("Ship_p10_1", {{{0,0},{0,1},{0,3},{0,6},{1,0},{1,2},{2,0},{2,2},{3,0},{3,2},{4,0},{4,1},{4,3},{4,6}}}, ""); // ship p=10 pop=14
add_cells("Ship_p10_2", {{{0,0},{0,3},{0,4},{1,0},{1,2},{1,4},{2,0},{2,2},{2,4},{3,0},{3,2},{3,4},{4,0},{4,3},{4,4}}}, ""); // ship p=10 pop=15
add_cells("Ship_p10_3", {{{0,0},{0,1},{0,4},{1,0},{1,2},{1,4},{2,0},{2,2},{2,4},{3,0},{3,2},{3,4},{4,0},{4,1},{4,4}}}, ""); // ship p=10 pop=15
add_cells("Ship_p10_4", {{{0,0},{0,1},{1,0},{1,2},{1,7},{2,0},{2,2},{2,4},{2,6},{2,8},{3,0},{3,2},{3,7},{4,0},{4,1}}}, ""); // ship p=10 pop=15
add_cells("Ship_p10_5", {{{0,5},{0,7},{0,8},{1,0},{1,4},{1,6},{1,8},{2,3},{2,6},{2,8},{3,0},{3,4},{3,6},{3,8},{4,5},{4,7},{4,8}}}, ""); // ship p=10 pop=17
add_cells("Ship_p10_6", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,4},{1,8},{2,0},{2,2},{2,5},{3,0},{3,2},{3,4},{3,8},{4,0},{4,1},{4,3}}}, ""); // ship p=10 pop=17
add_cells("Ship_p10_7", {{{0,0},{0,1},{0,6},{1,0},{1,2},{1,6},{2,0},{2,2},{2,4},{2,7},{2,8},{3,0},{3,2},{3,6},{4,0},{4,1},{4,6}}}, ""); // ship p=10 pop=17
add_cells("Ship_p10_8", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,4},{1,6},{1,8},{2,0},{2,2},{2,5},{3,0},{3,2},{3,4},{3,6},{3,8},{4,0},{4,1},{4,3}}}, ""); // ship p=10 pop=19
add_cells("Ship_p10_9", {{{0,0},{0,1},{0,5},{0,7},{1,0},{1,2},{1,4},{2,0},{2,2},{2,4},{2,5},{2,9},{2,10},{3,0},{3,2},{3,4},{4,0},{4,1},{4,5},{4,7}}}, ""); // ship p=10 pop=20
add_cells("Ship_p10_11", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,4},{1,5},{1,6},{1,7},{1,9},{2,0},{2,2},{2,7},{3,0},{3,2},{3,4},{3,5},{3,6},{3,7},{3,9},{4,0},{4,1},{4,3}}}, ""); // ship p=10 pop=23
add_cells("Ship_p10_12", {{{0,0},{0,1},{0,3},{1,0},{1,2},{1,4},{1,5},{1,6},{1,7},{1,9},{2,0},{2,2},{2,7},{3,0},{3,2},{3,4},{3,5},{3,6},{3,7},{3,9},{4,0},{4,1},{4,3}}}, ""); // ship p=10 pop=23

// --- Ships period=40 ---

// cat:All Ships:start=104 count=53

// Total new patterns: 156
// Still Lifes: 4
// Oscillators: 99
//   Period 2: 23
//   Period 4: 71
//   Period 5: 4
//   Period 10: 1
// Ships: 53
//   Period 2: 11
//   Period 5: 9
//   Period 8: 22
//   Period 10: 11

// === init_cats() entries ===
// These go after the existing cats in init_cats()
// int next_cat = cat_n;
// cats[next_cat] = {"Unique Still",0,5};
// cats[next_cat+1] = {"Unique Osc",5,99};
// cats[next_cat+2] = {"Unique Ships",104,53};

}

inline int wrap(int idx, int size) {
    if (idx < 0) return idx + size;
    if (idx >= size) return idx - size;
    return idx;
}


class LifeEngine {
    uint8_t grid[2][GW][GH];
    int front_ = 0, gen_ = 0;
public:
    LifeEngine() { reset(); }
    void reset() { front_=0; gen_=0; memset(grid,0,sizeof(grid)); }
    int generation() const { return gen_; }
    bool alive(int i,int j) const { return grid[front_][i][j] & MASK_ALIVE; }
    bool marked(int i,int j) const { return grid[front_][i][j] & MASK_MARK; }
    void set_alive(int i,int j) { grid[front_][i][j] |= MASK_BOTH; }
    void toggle(int i,int j) {
        if (grid[front_][i][j] & MASK_ALIVE) grid[front_][i][j] &= ~MASK_BOTH;
        else grid[front_][i][j] |= MASK_BOTH;
    }
    int population() const {
        int n=0;
        for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
            if (grid[front_][i][j] & MASK_ALIVE) n++;
        return n;
    }
    void clear_all() {
        memset(grid[front_],0,sizeof(grid[0]));
        memset(grid[1-front_],0,sizeof(grid[0]));
        gen_=0;
    }
    void randomize(int target=-1,int range=160) {
        clear_all();
        int r=std::min(range,MAX_W-1);
        int area=(r*2+1)*(r*2+1);
        double prob=target<0?0.18:(double)target/area;
        for (int y=-r;y<=r;y++) for (int x=-r;x<=r;x++)
            if ((double)rand()/RAND_MAX<prob) set_alive(wrap(x+MAX_W,GW),wrap(y+MAX_H,GH));
        init_marks();
    }
    void init_marks() {
        for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
            if (grid[front_][i][j] & MASK_ALIVE) {
                grid[front_][i][j]|=MASK_MARK;
                for (int k=0;k<10;k++)
                    grid[front_][wrap(i+NX[k],GW)][wrap(j+NY[k],GH)]|=MASK_MARK;
            }
    }
    void step() {
        int src=front_, dst=1-front_;
        memset(grid[dst],0,sizeof(grid[dst]));
        for (int i=0;i<GW;i++) for (int j=0;j<GH;j++) {
            if (!(grid[src][i][j]&MASK_MARK)) continue;
            uint8_t cur=grid[src][i][j]&MASK_ALIVE;
            int o=0;
            for (int k=0;k<10;k++)
                o+=grid[src][wrap(i+NX[k],GW)][wrap(j+NY[k],GH)]&MASK_ALIVE;
            if (cur) { if (o!=2) continue; }
            else     { if (o!=3&&o!=5&&o!=6) continue; }
            grid[dst][i][j]|=MASK_BOTH;
            for (int k=0;k<10;k++)
                grid[dst][wrap(i+NX[k],GW)][wrap(j+NY[k],GH)]|=MASK_MARK;
        }
        front_=dst; gen_++;
    }
    void place_pattern(int ci,int cj,int idx) {
        if (idx<0||idx>=(int)patterns.size()) return;
        auto& p=patterns[idx];
        for (int y=0;y<p.h;y++) for (int x=0;x<p.w;x++)
            if (p.data[y*p.w+x]=='1') set_alive(wrap(ci+y-p.h/2,GW),wrap(cj+x-p.w/2,GH));
        init_marks();
    }
};

LifeEngine life;
std::deque<int> history;
int max_pop=0, cur_pattern=0, cur_cat=0;
int dropdown_mode=0;  // 0=closed, 1=category, 2=pattern
int dropdown_scroll=0;
bool paused=false, show_help=false, rotate_90=false;
int speed_level=10;
int window_w=1200, window_h=800;

// Turbo / batch stepping
int steps_per_tick=1;       // number of steps per render frame (1-500)
int search_mode=0;          // 0=normal, 1=auto-search patterns
int search_gen=0;           // current search generation
int search_interval=500;    // gens per search report

// 2D orthographic camera
float cam_zoom=1.0f, cam_ox=0.0f, cam_oy=0.0f;
float cam_vx=0.0f, cam_vy=0.0f;

double start_time=0, last_fps_time=0;
int frame_count=0;
float current_fps=0;

double last_mx=-1, last_my=-1;
bool mouse_down=false, mouse_dragging=false;

GLuint font_base=0;

// ─── UI layout ──────────────────────────────────────────────────
int random_target=15000;
enum UIAction { UI_NONE, UI_PLAY, UI_STEP, UI_PREV, UI_NEXT,
    UI_LOAD, UI_RANDOM, UI_CLEAR, UI_CATEGORY, UI_PATTERN, UI_SLIDER, UI_ROTATE };
bool ui_slider_dragging=false;
int btn_x[9], btn_w=52, cat_btn_w=80, pat_btn_w=140, slider_sw=100, rot_toggle_w=58;
void calc_layout() {
    int x=14;
    for (int i=0;i<7;i++) { btn_x[i]=x; x+=btn_w+8; }
    btn_x[7]=x; x+=cat_btn_w+8;
    btn_x[8]=x;
}
int rot_toggle_x() { return btn_x[8]+pat_btn_w+10; }
int slider_x_pos() { return rot_toggle_x()+rot_toggle_w+8; }
struct CatDef { const char* name; int start, count; };
CatDef cats[40]; int cat_n=0;
void init_cats() {
    int n=(int)patterns.size();
    cats[0]={"All",0,n}; cat_n=1;
    auto add = [&](const char* name,int start,int count) {
        cats[cat_n++]={name,start,count};
    };
    add("Still Lifes",0,4);
    add("Osc p=2",4,23);
    add("Osc p=4",27,71);
    add("Osc p=5",98,4);
    add("Osc p=10",102,1);
    add("Ship p=2",103,11);
    add("Ship p=5",114,9);
    add("Ship p=8",123,22);
    add("Ship p=10",145,11);
}
void cat_nav_prev() {
    if (cur_cat==0) cur_pattern=cur_pattern?cur_pattern-1:(int)patterns.size()-1;
    else { int s=cats[cur_cat].start,c=cats[cur_cat].count;
           cur_pattern=cur_pattern>s?cur_pattern-1:s+c-1; }
}
void cat_nav_next() {
    if (cur_cat==0) cur_pattern=(cur_pattern+1)%(int)patterns.size();
    else { int s=cats[cur_cat].start,c=cats[cur_cat].count;
           cur_pattern=cur_pattern<s+c-1?cur_pattern+1:s; }
}
int bar_y() { return window_h-36-10; }
int chart_area_y() { return window_h-120-36-10; }
// ─── Font ──────────────────────────────────────────────────────
void create_font() {
    HFONT hFont=CreateFontA(14,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,
        DEFAULT_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,
        ANTIALIASED_QUALITY,FF_DONTCARE|DEFAULT_PITCH,"Consolas");
    if (!hFont) hFont=(HFONT)GetStockObject(ANSI_FIXED_FONT);
    HDC hDC=wglGetCurrentDC();
    SelectObject(hDC,hFont);
    font_base=glGenLists(256);
    wglUseFontBitmapsA(hDC,0,256,font_base);
    DeleteObject(hFont);
}

// ─── 2D helpers ────────────────────────────────────────────────
void draw_rect_2d(int x,int y,int w,int h,float r,float g,float b,bool fill=true) {
    if (fill) {
        glColor3f(r,g,b);
        glBegin(GL_QUADS);
        glVertex2i(x,y); glVertex2i(x+w,y);
        glVertex2i(x+w,y+h); glVertex2i(x,y+h);
        glEnd();
    } else {
        glColor3f(r,g,b);
        glBegin(GL_LINE_LOOP);
        glVertex2i(x,y); glVertex2i(x+w,y);
        glVertex2i(x+w,y+h); glVertex2i(x,y+h);
        glEnd();
    }
}

void draw_text_2d(int x,int y,const char* s,float r,float g,float b) {
    if (!font_base) return;
    glColor3f(r,g,b);
    glRasterPos2i(x,y+14);
    glListBase(font_base);
    glCallLists((GLsizei)strlen(s),GL_UNSIGNED_BYTE,s);
}

// ─── Orthographic main view ─────────────────────────────────────
void enter_world_view() {
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float hw=window_w/(2.0f*cam_zoom), hh=window_h/(2.0f*cam_zoom);
    glOrtho(cam_ox-hw, cam_ox+hw, cam_oy+hh, cam_oy-hh, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void enter_overlay() {
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0,window_w,window_h,0,-1,1);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
}

void exit_overlay() {
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glEnable(GL_DEPTH_TEST);
}

// ─── Render grid (with 90° rotation: j→X, i→Y inverted) ───────
void render_cells() {
    float s=CELL_SCALE;
    float hw=window_w/(2.0f*cam_zoom), hh=window_h/(2.0f*cam_zoom);
    float wx_min=cam_ox-hw, wx_max=cam_ox+hw;
    float wy_min=cam_oy-hh, wy_max=cam_oy+hh;
    int j0=(int)floor(wx_min/s)+MAX_H, j1=(int)ceil(wx_max/s)+MAX_H;
    int i1=MAX_W-(int)floor(wy_min/s), i0=MAX_W-(int)ceil(wy_max/s);
    i0=std::max(0,i0); i1=std::min(GW-1,i1);
    j0=std::max(0,j0); j1=std::min(GH-1,j1);

    int pop=life.population();
    float max_ratio=(float)pop/(float)std::max(max_pop,1);
    float r=0.1f+0.5f*max_ratio, g=0.6f+0.4f*(1.0f-max_ratio), b=0.1f;

    glColor3f(r,g,b);
    glBegin(GL_QUADS);
    for (int i=i0;i<=i1;i++) for (int j=j0;j<=j1;j++) {
        if (!life.alive(i,j)) continue;
        float x=(j-MAX_H)*s, y=-(i-MAX_W)*s;
        glVertex2f(x,y); glVertex2f(x+s,y);
        glVertex2f(x+s,y+s); glVertex2f(x,y+s);
    }
    glEnd();

    float cell_px=cam_zoom*s;
    if (cell_px>4.0f) {
        glColor3f(0.12f,0.12f,0.14f);
        glBegin(GL_LINES);
        for (int j=j0;j<=j1;j++) {
            float x=(j-MAX_H)*s;
            glVertex2f(x,-(i0-MAX_W)*s); glVertex2f(x,-(i1-MAX_W+1)*s);
        }
        for (int i=i0;i<=i1;i++) {
            float y=-(i-MAX_W)*s;
            glVertex2f((j0-MAX_H)*s,y); glVertex2f((j1-MAX_H+1)*s,y);
        }
        glEnd();
    }
}

bool mouse_to_grid(double,double,int&,int&);
void render_cursor() {
    if (last_mx<0) return;
    int gi,gj;
    if (!mouse_to_grid(last_mx,last_my,gi,gj)) return;
    float s=CELL_SCALE;
    float x=(gj-MAX_H)*s, y=-(gi-MAX_W)*s;
    float h=s*0.15f;
    glColor3f(1,1,1);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x-h,y-h); glVertex2f(x+s+h,y-h);
    glVertex2f(x+s+h,y+s+h); glVertex2f(x-h,y+s+h);
    glEnd();
}

// ─── Chart ─────────────────────────────────────────────────────
void render_chart() {
    int n=(int)history.size();
    int cx=10, cy=chart_area_y(), cw=window_w-20, ch=120;
    draw_rect_2d(cx,cy,cw,ch,0.08f,0.08f,0.10f);
    draw_rect_2d(cx,cy,cw,ch,0.2f,0.2f,0.3f,false);
    if (n<2) {
        draw_text_2d(cx+8,cy+4,"Not enough data",0.5f,0.5f,0.5f);
        return;
    }
    int pl=50, pr=15, pt=10, pb=25;
    int gw=cw-pl-pr, gh=ch-pt-pb;
    int max_val=*std::max_element(history.begin(),history.end());
    max_val=std::max(max_val,1);
    for (int i=0;i<=4;i++) {
        int y=cy+ch-pb-(gh*i)/4;
        glColor3f(0.25f,0.25f,0.25f);
        glBegin(GL_LINES);
        glVertex2i(cx+pl,y); glVertex2i(cx+cw-pr,y);
        glEnd();
        char buf[32]; snprintf(buf,sizeof(buf),"%d",max_val*i/4);
        draw_text_2d(cx+pl-46,y-6,buf,0.5f,0.5f,0.5f);
    }
    // Sample history for display if too large
    int disp_n = std::min(n, gw); // at most one point per pixel
    int step = std::max(1, n / disp_n);
    int actual_n = (n + step - 1) / step;
    for (int i=0;i<=5;i++) {
        int gen=n*i/5, x=cx+pl+(gw*i)/5;
        char buf[32]; snprintf(buf,sizeof(buf),"%d",gen);
        draw_text_2d(x-10,cy+ch-pb+8,buf,0.5f,0.5f,0.5f);
    }
    glColor3f(0.04f,0.53f,1.0f);
    glLineWidth(2.0f);
    glBegin(GL_LINE_STRIP);
    for (int k=0;k<actual_n;k++) {
        int idx=std::min(k*step,n-1);
        int x=cx+pl+(idx*gw)/(n-1);
        int y=cy+ch-pb-(history[idx]*gh)/max_val;
        glVertex2i(x,y);
    }
    glEnd();
    glLineWidth(1.0f);
    int lx=cx+pl+gw;
    int ly=cy+ch-pb-(history[n-1]*gh)/max_val;
    glColor3f(1.0f,0.3f,0.3f);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex2i(lx,ly);
    glEnd();
    glPointSize(1.0f);
    draw_text_2d(cx+8,cy+4,"Population",0.6f,0.8f,0.6f);
    draw_text_2d(cx+cw-340,cy+ch-14,"Created by Kostya P.  NetMail: 2:5079/65.14",0.35f,0.35f,0.35f);
}

// ─── UI bar ─────────────────────────────────────────────────────
void render_pattern_list();
void render_ui_bar() {
    calc_layout();
    int by=bar_y(), bw=window_w-20;
    draw_rect_2d(10,by,bw,36,0.12f,0.12f,0.14f);
    auto btn=[&](int idx,const char* label,float r,float g,float b) {
        draw_rect_2d(btn_x[idx],by+4,btn_w,28,r,g,b);
        draw_rect_2d(btn_x[idx],by+4,btn_w,28,0.4f,0.4f,0.4f,false);
        draw_text_2d(btn_x[idx]+8,by+8,label,0.9f,0.9f,0.9f);
    };
    btn(0,paused?"Play":"Pause",paused?0.3f:0.04f,paused?0.35f:0.3f,paused?0.04f:0.04f);
    btn(1,"Step",0.2f,0.2f,0.25f);
    btn(2,"<",0.18f,0.18f,0.22f);
    btn(3,">",0.18f,0.18f,0.22f);
    btn(4,"Load",0.15f,0.2f,0.15f);
    btn(5,"Rand",0.2f,0.15f,0.1f);
    btn(6,"Clear",0.25f,0.12f,0.12f);
    // Category dropdown
    int cx=btn_x[7];
    draw_rect_2d(cx,by+4,cat_btn_w,28,0.12f,0.15f,0.2f);
    draw_rect_2d(cx,by+4,cat_btn_w,28,0.4f,0.4f,0.4f,false);
    draw_text_2d(cx+4,by+8,cats[cur_cat].name,0.9f,0.9f,0.9f);
    draw_text_2d(cx+cat_btn_w-14,by+8,dropdown_mode==1?"▲":"▼",0.5f,0.5f,0.5f);
    // Pattern dropdown
    int px=btn_x[8];
    draw_rect_2d(px,by+4,pat_btn_w,28,0.15f,0.18f,0.22f);
    draw_rect_2d(px,by+4,pat_btn_w,28,0.4f,0.4f,0.4f,false);
    char pname[48];
    int poff=cur_cat?cats[cur_cat].start:0;
    snprintf(pname,sizeof(pname),"%d/%d %s",cur_pattern-poff+1,
        cur_cat?cats[cur_cat].count:(int)patterns.size(),patterns[cur_pattern].name.c_str());
    draw_text_2d(px+6,by+8,pname,0.9f,0.9f,0.9f);
    draw_text_2d(px+pat_btn_w-16,by+8,dropdown_mode==2?"▲":"▼",0.6f,0.6f,0.6f);
    // Rotate 90 toggle
    int rx=rot_toggle_x();
    draw_rect_2d(rx,by+4,rot_toggle_w,28,rotate_90?0.04f:0.12f,rotate_90?0.3f:0.15f,rotate_90?0.04f:0.2f);
    draw_rect_2d(rx,by+4,rot_toggle_w,28,0.4f,0.4f,0.4f,false);
    draw_text_2d(rx+6,by+8,rotate_90?"[x]90":"[ ]90",rotate_90?0.3f:0.7f,rotate_90?0.8f:0.7f,rotate_90?0.3f:0.7f);
    // Speed slider
    int sx=slider_x_pos(), sy=by+8, sh=20;
    draw_text_2d(sx-44,by+8,"Speed:",0.7f,0.7f,0.7f);
    draw_rect_2d(sx,sy,slider_sw,sh,0.15f,0.15f,0.18f);
    draw_rect_2d(sx,sy,slider_sw,sh,0.3f,0.3f,0.35f,false);
    float fill=(float)(speed_level-1)/99.0f;
    draw_rect_2d(sx,sy,(int)(slider_sw*fill),sh,0.04f,0.5f,0.04f);
    char spd[16]; snprintf(spd,sizeof(spd),"%d",speed_level);
    draw_text_2d(sx+slider_sw+4,sy+2,spd,0.9f,0.9f,0.9f);
    // HUD info
    double elapsed=glfwGetTime()-start_time;
    int mins=(int)(elapsed/60), secs=(int)(elapsed)%60;
    char hud[200];
    snprintf(hud,sizeof(hud),
        "Gen:%d Pop:%d Max:%d FPS:%.0f %02d:%02d %.1fx Cells:%d",
        life.generation(),history.empty()?0:history.back(),max_pop,
        current_fps,mins,secs,cam_zoom,random_target);
    draw_text_2d(window_w-360,by+8,hud,0.6f,0.8f,0.4f);
}

// ─── Help overlay ───────────────────────────────────────────────
void render_help() {
    if (!show_help) return;
    int by=bar_y(), bw=window_w-20;
    int hy=by-106, hh=102;
    draw_rect_2d(10,hy,bw,hh,0.06f,0.06f,0.08f);
    draw_rect_2d(10,hy,bw,hh,0.3f,0.3f,0.5f,false);
    const char* help[]={
        "Space/P=PlayPause  Right=Step     [ ]=Pattern  PgUp/PgDn=Pattern",
        "R=Place pattern    E=Rotate 90    C=Clear     H=Hide help  0-9=Place pat",
        "Shift+[=Target -   Shift+]=Target +  F=Zoom to fit  Q=Quit",
        "+/-=Speed          Drag=Pan       Scroll=Zoom",
    };
    int y=hy+12;
    for (auto& line : help) { draw_text_2d(20,y,line,0.7f,0.8f,0.6f); y+=22; }
}

// ─── Dropdown panels ────────────────────────────────────────────
int dd_w=350, dd_ih=18;
int dd_vis() { return (bar_y()-20)/dd_ih; }
void render_category_list() {
    int px=10,py=10,mh=bar_y()-20,vis=dd_vis();
    draw_rect_2d(px,py,dd_w,mh,0.06f,0.06f,0.09f);
    draw_rect_2d(px,py,dd_w,mh,0.3f,0.3f,0.5f,false);
    int by=py+8;
    for (int c=0;c<cat_n;c++) {
        if (c<dropdown_scroll||c-dropdown_scroll>=vis) continue;
        int y=by+(c-dropdown_scroll)*dd_ih;
        if (c==cur_cat) draw_rect_2d(px+12,y,dd_w-24,dd_ih-2,0.15f,0.25f,0.15f);
        draw_text_2d(px+16,y,cats[c].name,c==cur_cat?0.2f:0.8f,c==cur_cat?0.8f:0.8f,c==cur_cat?0.2f:0.7f);
    }
}
void render_pattern_list() {
    int px=10,py=10,mh=bar_y()-20,vis=dd_vis();
    draw_rect_2d(px,py,dd_w,mh,0.06f,0.06f,0.09f);
    draw_rect_2d(px,py,dd_w,mh,0.3f,0.3f,0.5f,false);
    int cat=cur_cat, s=cats[cat].start, cnt=cats[cat].count;
    int by=py+8;
    draw_text_2d(px+10,by-2,cats[cat].name,0.5f,0.7f,1.0f);
    for (int i=0;i<cnt;i++) {
        int idx=i+dropdown_scroll;
        if (idx>=cnt) break;
        if (i>=vis) break;
        int p=s+idx, y=by+i*dd_ih;
        if (p==cur_pattern) draw_rect_2d(px+12,y,dd_w-24,dd_ih-2,0.15f,0.25f,0.15f);
        draw_text_2d(px+18,y,patterns[p].name.c_str(),
            p==cur_pattern?0.2f:0.7f,p==cur_pattern?0.8f:0.8f,p==cur_pattern?0.2f:0.6f);
    }
    if (cnt>vis) {
        int sbx=px+dd_w-8,sby=py+2,sbh=mh-4;
        draw_rect_2d(sbx,sby,6,sbh,0.15f,0.15f,0.18f);
        float th=(float)sbh*vis/cnt, to=(float)sbh*dropdown_scroll/cnt;
        draw_rect_2d(sbx,(int)(sby+to),6,(int)th,0.4f,0.4f,0.5f);
    }
}
int dd_hit_test(double mx,double my,int& out_idx) {
    if (!dropdown_mode) return 0;
    int px=10,py=10,mh=bar_y()-20,vis=dd_vis();
    if (mx<px||mx>px+dd_w||my<py||my>py+mh) return 0;
    if (dropdown_mode==1) {
        // Category list
        for (int c=0;c<cat_n;c++) {
            if (c<dropdown_scroll||c-dropdown_scroll>=vis) continue;
            int y=py+8+(c-dropdown_scroll)*dd_ih;
            if (my>=y&&my<y+dd_ih) { out_idx=c; return 1; }
        }
    } else {
        // Pattern list
        int cat=cur_cat, cnt=cats[cat].count;
        for (int i=0;i<vis&&i+dropdown_scroll<cnt;i++) {
            int y=py+8+i*dd_ih;
            if (my>=y&&my<y+dd_ih) { out_idx=cats[cat].start+i+dropdown_scroll; return 2; }
        }
    }
    return 0;
}

// ─── Zoom to fit ────────────────────────────────────────────────
void zoom_to_fit() {
    int pop=life.population();
    if (pop==0) return;
    int i_min=GW,i_max=0,j_min=GH,j_max=0;
    for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
        if (life.alive(i,j)) {
            if (i<i_min) i_min=i; if (i>i_max) i_max=i;
            if (j<j_min) j_min=j; if (j>j_max) j_max=j;
        }
    // With 90° rotation: j→X, i→Y(inverted)
    float cx=(j_min+j_max)*0.5f-MAX_H+0.5f;
    float cy=-((i_min+i_max)*0.5f-MAX_W+0.5f);
    cam_ox=cx*CELL_SCALE; cam_oy=cy*CELL_SCALE;
    float w=(j_max-j_min+2)*CELL_SCALE, h=(i_max-i_min+2)*CELL_SCALE;
    float zoom_x=window_w/w, zoom_y=window_h/h;
    cam_zoom=std::min(zoom_x,zoom_y)*0.85f;
    cam_zoom=std::max(0.1f,std::min(100.0f,cam_zoom));
    cam_vx=0; cam_vy=0;
}

// ─── UI hit testing ────────────────────────────────────────────
UIAction hit_test_ui(double mx,double my) {
    int by=bar_y();
    if (my<by||my>by+36) return UI_NONE;
    for (int i=0;i<7;i++)
        if (mx>=btn_x[i]&&mx<=btn_x[i]+btn_w) return (UIAction)(UI_PLAY+i);
    if (mx>=btn_x[7]&&mx<=btn_x[7]+cat_btn_w) return UI_CATEGORY;
    if (mx>=btn_x[8]&&mx<=btn_x[8]+pat_btn_w) return UI_PATTERN;
    int rx=rot_toggle_x();
    if (mx>=rx&&mx<=rx+rot_toggle_w) return UI_ROTATE;
    int sx=slider_x_pos();
    if (mx>=sx&&mx<=sx+slider_sw&&my>=by+8&&my<=by+28) return UI_SLIDER;
    return UI_NONE;
}

void update_slider(double mx) {
    float rel=(float)(mx-slider_x_pos())/(float)slider_sw;
    speed_level=std::max(1,std::min(100,(int)(rel*99.0f)+1));
}

// ─── Screen to grid ─────────────────────────────────────────────
void screen_to_world(double mx,double my,float& wx,float& wy) {
    wx=cam_ox+(mx-window_w/2.0f)/cam_zoom;
    wy=cam_oy-(my-window_h/2.0f)/cam_zoom;
}

bool mouse_to_grid(double mx,double my,int& gi,int& gj) {
    float wx,wy; screen_to_world(mx,my,wx,wy);
    gj=(int)floor(wx/CELL_SCALE+0.5f)+MAX_H;
    gi=MAX_W-(int)floor(wy/CELL_SCALE+0.5f);
    return gi>=0&&gi<GW&&gj>=0&&gj<GH;
}

bool try_toggle_cell(double mx,double my) {
    int gi,gj;
    if (!mouse_to_grid(mx,my,gi,gj)) return false;
    life.toggle(gi,gj);
    life.init_marks();
    return true;
}

// ─── GLFW callbacks ────────────────────────────────────────────
void on_key(GLFWwindow* win,int key,int,int action,int mods) {
    if (action!=GLFW_PRESS&&action!=GLFW_REPEAT) return;
    auto place_at_mouse=[&](){
        double mx,my; glfwGetCursorPos(win,&mx,&my);
        int mi,mj;
        if (mouse_to_grid(mx,my,mi,mj)) {
            if (rotate_90) {
                auto& p=patterns[cur_pattern];
                for (int y=0;y<p.h;y++) for (int x=0;x<p.w;x++)
                    if (p.data[y*p.w+x]=='1')
                        life.set_alive(wrap(mi+x-p.w/2,GW),wrap(mj+(p.w-1-y)-p.h/2,GH));
                life.init_marks();
            } else {
                life.place_pattern(mi,mj,cur_pattern);
            }
        }
        history.push_back(life.population());
        max_pop=std::max(max_pop,life.population());
        zoom_to_fit();
    };
    switch (key) {
        case GLFW_KEY_SPACE: case GLFW_KEY_P: paused=!paused; break;
        case GLFW_KEY_RIGHT:
            if (!paused) paused=true;
            life.step(); history.push_back(life.population());
            max_pop=std::max(max_pop,life.population()); break;
        case GLFW_KEY_Q: glfwSetWindowShouldClose(win,GLFW_TRUE); break;
        case GLFW_KEY_EQUAL: case GLFW_KEY_KP_ADD: speed_level=std::min(100,speed_level+5); break;
        case GLFW_KEY_MINUS: case GLFW_KEY_KP_SUBTRACT: speed_level=std::max(1,speed_level-5); break;
        case GLFW_KEY_F: zoom_to_fit(); break;
        case GLFW_KEY_E: {
            auto& p=patterns[cur_pattern];
            int nw=p.h, nh=p.w;
            std::string nd(nw*nh,'0');
            for (int y=0;y<p.h;y++) for (int x=0;x<p.w;x++)
                if (p.data[y*p.w+x]=='1') nd[x*nw+(p.w-1-y)]='1';
            p.w=nw; p.h=nh; p.data=std::move(nd);
            place_at_mouse(); break;
        }
        case GLFW_KEY_R: place_at_mouse(); break;
        case GLFW_KEY_C:
            life.clear_all(); history.clear(); max_pop=0;
            cam_ox=0; cam_oy=0; break;
        case GLFW_KEY_H: show_help=!show_help; break;
        case GLFW_KEY_PAGE_UP: cat_nav_prev(); break;
        case GLFW_KEY_PAGE_DOWN: cat_nav_next(); break;
        case GLFW_KEY_LEFT_BRACKET:
            if (mods & GLFW_MOD_SHIFT)
                random_target=std::max(1000,random_target/2);
            else cat_nav_prev();
            break;
        case GLFW_KEY_RIGHT_BRACKET:
            if (mods & GLFW_MOD_SHIFT)
                random_target=std::min(200000,random_target*2);
            else cat_nav_next();
            break;
        case GLFW_KEY_0: cur_pattern=9; place_at_mouse(); break;
        case GLFW_KEY_1: case GLFW_KEY_2: case GLFW_KEY_3:
        case GLFW_KEY_4: case GLFW_KEY_5: case GLFW_KEY_6:
        case GLFW_KEY_7: case GLFW_KEY_8: case GLFW_KEY_9:
            cur_pattern=key-GLFW_KEY_1; place_at_mouse(); break;
        default: break;
    }
}

void on_mouse_btn(GLFWwindow* win,int btn,int action,int) {
    if (action==GLFW_PRESS) {
        double mx,my; glfwGetCursorPos(win,&mx,&my);
        // If dropdown open, check hits
        if (dropdown_mode) {
            int sel;
            int r=dd_hit_test(mx,my,sel);
            if (r==1) { // category selected
                cur_cat=sel; dropdown_scroll=0;
                cur_pattern=cats[sel].start; dropdown_mode=0; return;
            }
            if (r==2) { // pattern selected
                cur_pattern=sel; dropdown_mode=0; return;
            }
            // Click outside panel closes
            if (mx<10||mx>10+dd_w||my<10||my>bar_y()-20)
                dropdown_mode=0;
            else return;
        }
        if (btn==GLFW_MOUSE_BUTTON_RIGHT) { zoom_to_fit(); return; }
        if (btn!=GLFW_MOUSE_BUTTON_LEFT) return;
        glfwGetCursorPos(win,&last_mx,&last_my);
        auto hit=hit_test_ui(last_mx,last_my);
        switch (hit) {
            case UI_PLAY: paused=!paused; break;
            case UI_STEP:
                if (!paused) paused=true;
                life.step(); history.push_back(life.population());
                max_pop=std::max(max_pop,life.population()); break;
            case UI_PREV: cat_nav_prev(); break;
            case UI_NEXT: cat_nav_next(); break;
            case UI_CATEGORY: dropdown_mode=(dropdown_mode==1)?0:1; dropdown_scroll=0; break;
            case UI_PATTERN: dropdown_mode=(dropdown_mode==2)?0:2; dropdown_scroll=0; break;
            case UI_LOAD:
                life.clear_all();
                { double cx,cy; glfwGetCursorPos(win,&cx,&cy);
                  int mi,mj;
                  if (mouse_to_grid(cx,cy,mi,mj)) {
                      if (rotate_90) {
                          auto& p=patterns[cur_pattern];
                          for (int y=0;y<p.h;y++) for (int x=0;x<p.w;x++)
                              if (p.data[y*p.w+x]=='1')
                                  life.set_alive(wrap(mi+x-p.w/2,GW),wrap(mj+(p.w-1-y)-p.h/2,GH));
                          life.init_marks();
                      } else {
                          life.place_pattern(mi,mj,cur_pattern);
                      }
                  } }
                history.clear(); history.push_back(life.population());
                max_pop=life.population(); zoom_to_fit(); break;
            case UI_RANDOM:
                life.randomize(random_target,std::min(160,MAX_W-1));
                history.clear(); history.push_back(life.population());
                max_pop=life.population(); zoom_to_fit(); break;
            case UI_CLEAR: life.clear_all(); history.clear(); max_pop=0;
                cam_ox=0; cam_oy=0; break;
            case UI_ROTATE: rotate_90=!rotate_90; break;
            case UI_SLIDER: ui_slider_dragging=true; update_slider(last_mx); break;
            default:
                mouse_down=true; mouse_dragging=false;
                break;
        }
    } else {
        if (!mouse_dragging&&mouse_down)
            try_toggle_cell(last_mx,last_my);
        mouse_down=false; ui_slider_dragging=false;
    }
}

void on_mouse_move(GLFWwindow* win,double x,double y) {
    if (ui_slider_dragging) { update_slider(x); return; }
    if (mouse_down) {
        double dx=x-last_mx, dy=y-last_my;
        if (dx*dx+dy*dy>9.0) mouse_dragging=true;
        if (mouse_dragging) {
            cam_ox-=dx/cam_zoom; cam_oy+=dy/cam_zoom;
            cam_vx=-(float)dx/cam_zoom; cam_vy=(float)dy/cam_zoom;
            last_mx=x; last_my=y;
        }
    }
}

void on_scroll(GLFWwindow*,double,double yoff) {
    if (dropdown_mode) {
        int total=(dropdown_mode==1)?cat_n:cats[cur_cat].count;
        int vis=dd_vis();
        dropdown_scroll=std::max(0,std::min(total-vis,dropdown_scroll+(yoff>0?1:-1)));
        return;
    }
    float factor=(yoff<0)?1.1f:1.0f/1.1f;
    cam_zoom*=factor;
    cam_zoom=std::max(0.1f,std::min(100.0f,cam_zoom));
}

void on_resize(GLFWwindow*,int w,int h) {
    window_w=w; window_h=h;
    glViewport(0,0,w,h);
}

// ─── Analyze all patterns ────────────────────────────────────
void analyze_patterns() {
    FILE* f=fopen("analysis_200.txt","w");
    if (!f) { fprintf(stderr,"Cannot open analysis_200.txt\n"); return; }
    int n=(int)patterns.size();
    fprintf(f,"=== Analysis of %d patterns after 200 steps (B356/S2 10-cell) ===\n\n",n);
    int dead=0, still=0, osc=0, ship=0, other=0;
    for (int idx=0;idx<n;idx++) {
        life.clear_all();
        life.place_pattern(MAX_W,MAX_H,idx);
        bool died=false; int pop0=life.population();
        // Run 200 steps, tracking states for period detection
        std::vector<std::pair<int,int>> state0, state200;
        for (int g=0;g<200;g++) {
            life.step();
            int p=life.population();
            if (p==0) { died=true; break; }
        }
        if (died) { dead++; fprintf(f,"DEAD %s\n",patterns[idx].name.c_str()); continue; }
        // Extract final state
        std::vector<std::pair<int,int>> cells;
        int mr=GW,mc=GH,xr=0,xc=0;
        for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
            if (life.alive(i,j)) { cells.push_back({i,j});
                if (i<mr) mr=i; if (i>xr) xr=i; if (j<mc) mc=j; if (j>xc) xc=j; }
        int h=xr-mr+1, w=xc-mc+1;
        if (h>200||w>200) { other++; fprintf(f,"EXPLOSIVE %s pop=%d bb=%dx%d\n",patterns[idx].name.c_str(),(int)cells.size(),w,h); continue; }
        // Normalize
        for (auto& p:cells) { p.first-=mr; p.second-=mc; }
        std::sort(cells.begin(),cells.end());
        // Detect period: compare with earlier step
        // For simplicity: if bb<=20 run period check, else classify as other
        int period=-1;
        if (cells.size()<200&&h<=20&&w<=20) {
            // Re-run and check period
            life.clear_all();
            life.place_pattern(MAX_W,MAX_H,idx);
            std::vector<std::vector<std::pair<int,int>>> states;
            for (int g=0;g<100;g++) {
                // Extract normalized state at this gen
                std::vector<std::pair<int,int>> sc;
                int smr=GW,smc=GH;
                for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
                    if (life.alive(i,j)) { sc.push_back({i,j});
                        if (i<smr) smr=i; if (j<smc) smc=j; }
                for (auto& p:sc) { p.first-=smr; p.second-=smc; }
                std::sort(sc.begin(),sc.end());
                // Check against previous states
                for (int t=1;t<=(int)states.size()&&t<=50;t++) {
                    if (sc==states[(int)states.size()-t]) { period=t; break; }
                }
                if (period>0) break;
                states.push_back(sc);
                life.step();
            }
        }
        // Check if spaceship (center of mass moves)
        bool is_ship=false;
        if (period>0) {
            life.clear_all();
            life.place_pattern(MAX_W,MAX_H,idx);
            double com_r0=0,com_c0=0; int pp0=0;
            for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
                if (life.alive(i,j)) { com_r0+=i; com_c0+=j; pp0++; }
            com_r0/=pp0; com_c0/=pp0;
            for (int s=0;s<period;s++) life.step();
            double com_r1=0,com_c1=0; int pp1=0;
            for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
                if (life.alive(i,j)) { com_r1+=i; com_c1+=j; pp1++; }
            com_r1/=pp1; com_c1/=pp1;
            double dr=com_r1-com_r0, dc=com_c1-com_c0;
            double dist=sqrt(dr*dr+dc*dc);
            if (dist>0.5) is_ship=true;
        }
        if (period==0) { still++; fprintf(f,"STILL %s pop=%d bb=%dx%d\n",patterns[idx].name.c_str(),(int)cells.size(),w,h); }
        else if (is_ship) { ship++; fprintf(f,"SHIP %s period=%d pop=%d bb=%dx%d\n",patterns[idx].name.c_str(),period,(int)cells.size(),w,h); }
        else { osc++; fprintf(f,"OSC %s period=%d pop=%d bb=%dx%d\n",patterns[idx].name.c_str(),period,(int)cells.size(),w,h); }
    }
    fprintf(f,"\n=== Summary: dead=%d still=%d osc=%d ship=%d other=%d total=%d ===\n",dead,still,osc,ship,other,dead+still+osc+ship+other);
    fclose(f);
    printf("Analysis written to analysis_200.txt\n");
}

// ─── Dump pattern cells ─────────────────────────────────────
void dump_pattern_cells() {
    FILE* f=fopen("pattern_cells.txt","w");
    if (!f) { fprintf(stderr,"Cannot open pattern_cells.txt\n"); return; }
    int n=(int)patterns.size();
    for (int idx=0;idx<n;idx++) {
        fprintf(f,"%d %s|",idx,patterns[idx].name.c_str());
        // Place at (MAX_W, MAX_H) and dump alive cells
        life.clear_all();
        life.place_pattern(MAX_W,MAX_H,idx);
        for (int i=0;i<GW;i++) for (int j=0;j<GH;j++)
            if (life.alive(i,j)) fprintf(f,"%d,%d ",i-MAX_W,j-MAX_H);
        fprintf(f,"\n");
    }
    fclose(f);
    printf("Dumped %d patterns to pattern_cells.txt\n",n);
}

// ─── Main ──────────────────────────────────────────────────────
int main(int argc, char** argv) {
    srand((unsigned int)time(nullptr));
    // Move pattern init before GLFW — no GLFW dependency
    init_patterns();
    init_cats();
    if (argc>1 && strcmp(argv[1],"--analyze")==0) {
        analyze_patterns();
        return 0;
    }
    if (argc>1 && strcmp(argv[1],"--dump-cells")==0) {
        dump_pattern_cells();
        return 0;
    }
    if (!glfwInit()) { fprintf(stderr,"GLFW init failed\n"); return 1; }
    GLFWwindow* win=glfwCreateWindow(window_w,window_h,
        "Game of Life B356/S2",nullptr,nullptr);
    if (!win) { glfwTerminate(); return 1; }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1);
    glfwSetKeyCallback(win,on_key);
    glfwSetMouseButtonCallback(win,on_mouse_btn);
    glfwSetCursorPosCallback(win,on_mouse_move);
    glfwSetScrollCallback(win,on_scroll);
    glfwSetWindowSizeCallback(win,on_resize);

    create_font();
    glClearColor(0.04f,0.04f,0.05f,1.0f);
    glDisable(GL_DEPTH_TEST);

    life.randomize(random_target,std::min(120,MAX_W-1));
    history.push_back(life.population());
    max_pop=life.population();
    zoom_to_fit();
    start_time=glfwGetTime();
    last_fps_time=glfwGetTime();

    double last_step=glfwGetTime();

    while (!glfwWindowShouldClose(win)) {
        double now=glfwGetTime();
        frame_count++;
        if (now-last_fps_time>=1.0) {
            current_fps=frame_count/(now-last_fps_time);
            frame_count=0; last_fps_time=now;
        }
        if (!paused) {
            double elapsed=now-last_step;
            double interval=1.0/speed_level;
            if (elapsed>=interval) {
                int steps=(int)(elapsed/interval);
                steps=std::min(steps,10000);
                for (int s=0;s<steps;s++) {
                    life.step();
                    history.push_back(life.population());
                    max_pop=std::max(max_pop,life.population());
                }
                last_step=now-fmod(elapsed,interval);
            }
        }
        // Pan inertia
        if (!mouse_dragging) {
            cam_vx*=0.92f; cam_vy*=0.92f;
            cam_ox+=cam_vx; cam_oy+=cam_vy;
            if (fabsf(cam_vx)<0.001f) cam_vx=0;
            if (fabsf(cam_vy)<0.001f) cam_vy=0;
        }

        glClear(GL_COLOR_BUFFER_BIT);
        enter_world_view();
        render_cells();
        render_cursor();

        enter_overlay();
        render_ui_bar();
        if (dropdown_mode==1) render_category_list();
        else if (dropdown_mode==2) render_pattern_list();
        else if (show_help) render_help();
        else render_chart();
        exit_overlay();

        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    if (font_base) glDeleteLists(font_base,128);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
