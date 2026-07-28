╔══════════════════════════════════════════════════════════════╗
║         B356/S2 — Life with Asymmetric Cross Neighbourhood  ║
║             156 verified unique objects after 200+ steps     ║
╚══════════════════════════════════════════════════════════════╝

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
1. RULE B356/S2
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Each cell has 10 neighbours in an asymmetric cross:

              (-1,-1)   (-1,0)   (-1,1)

    (0,-2)   (0,-1)     [X]     (0,1)    (0,2)

              (1,-1)    (1,0)    (1,1)

- Alive cell survives if exactly 2 neighbours (S2)
- Dead cell is born if 3, 5, or 6 neighbours (B356)

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
2. PATTERN LIBRARY — 156 Verified Unique Objects
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

All patterns are final-state configurations verified by running
the simulation for 200+ steps with the correct 10-cell neighbourhood.
Duplicates (same final configuration from different seeds) removed.

Category             | Period | Count | Naming
─────────────────────|────────|───────|──────────────────────────
Still Lifes         |   1    |   4   | Still_1 … Still_4
Oscillators p=2     |   2    |  23   | Osc_p2_1 … Osc_p2_23
Oscillators p=4     |   4    |  71   | Osc_p4_1 … Osc_p4_71
Oscillators p=5     |   5    |   4   | Osc_p5_1 … Osc_p5_4
Oscillators p=10    |  10    |   1   | Osc_p10_1
Spaceships p=2      |   2    |  11   | Ship_p2_1 … Ship_p2_11
Spaceships p=5      |   5    |   9   | Ship_p5_1 … Ship_p5_9
Spaceships p=8      |   8    |  22   | Ship_p8_1 … Ship_p8_22
Spaceships p=10     |  10    |  11   | Ship_p10_1 … Ship_p10_11

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
3. CATEGORY ORGANISATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

The UI organises patterns into 9 categories plus "All":

   All            — all 156 patterns
   Still Lifes     — static period-1 configurations (4)
   Osc p=2         — period-2 oscillators (23)
   Osc p=4         — period-4 oscillators (71)
   Osc p=5         — period-5 oscillators (4)
   Osc p=10        — period-10 oscillator (1)
   Ship p=2        — period-2 spaceships (11)
   Ship p=5        — period-5 spaceships (9)
   Ship p=8        — period-8 spaceships (22)
   Ship p=10       — period-10 spaceships (11)

Switching categories: click the category button (shows current
cat) to open the category dropdown, then click a category name.

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
4. UI CONTROLS
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Keyboard:
  Space          — play/pause
  Right Arrow    — step one generation
  Left Arrow     — previous pattern
  Right Arrow*   — next pattern  (*when pattern field focused)
  Up/Down Arrow  — previous/next pattern
  F              — zoom to fit (reset camera to show all cells)
  E              — place current pattern with 90-degree rotation
  H              — toggle help overlay
  R              — randomise grid (seeds ~120 random cells)
  C              — clear grid
  Esc            — close dropdown / help

Mouse:
  Left drag      — pan camera (with inertia throw on release)
  Scroll         — zoom in/out
  Right-click    — place pattern at mouse position
  Left-click     — (same as right-click)

UI Buttons (top bar, left to right):
  [Play/Pause]   — toggle simulation
  [Step]         — single step
  [<] [>]        — previous/next pattern
  [Load]         — place selected pattern at cursor
  [Random]       — randomise grid
  [Clear]        — clear grid
  [cat: name]    — current category (click to open dropdown)
  [pat: name]    — current pattern (click to open dropdown)
  [x]90 / [ ]90  — toggle 90-degree rotation on place
  [speed slider] — simulation speed

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
5. COMPILATION
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Requires MinGW and GLFW 3.4:

  g++ -std=c++17 -O2 life_gl.cpp -o life_gl.exe ^
    -I"%USERPROFILE%\scoop\apps\glfw\3.4\include" ^
    -L"%USERPROFILE%\scoop\apps\glfw\3.4\lib-mingw-w64" ^
    -lglfw3 -lopengl32 -lgdi32

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
6. ANALYSIS MODE
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

Run with --analyze to classify all patterns after 200 steps:
  life_gl.exe --analyze

Run with --dump-cells to export all cell coordinates:
  life_gl.exe --dump-cells

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
7. TECHNICAL NOTES
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━

- Grid: 2000x2000 toroidal (1000x1000 logical centre)
- Rendering: 2D orthographic, green quads on dark background
- Coordinates: j→X (column), i→Y (row), Y inverted
- Font: Consolas via wglUseFontBitmapsA
- API: Legacy OpenGL + GLFW 3.4 + windows.h
- No GLU dependencies
- 90-degree rotation (E key): rotates pattern bounding box and
  flips coordinates; works with both keyboard and mouse placement
- The asymmetric 10-cell neighbourhood makes standard Life
  analysis tools inapplicable — all patterns are verified
  empirically by simulation

━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
Rule: B356/S2, 10-cell asymmetric cross neighbourhood.
All 156 objects verified by 200+ generation simulation.
Library restructured July 2026.
━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━
