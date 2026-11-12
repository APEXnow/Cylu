# Cylu

An isometric puzzle/adventure game by Paul S.J. Millard (2001), based on the
1985 Amstrad CPC game by Geoff Foley published by Firebird Software.

Version 2.0 is a port from Allegro 4 / Dev-C++ to **Allegro 5.2** and
**Visual Studio 2026** (x64). The original game text and story are in
[readme.txt](readme.txt).

The 1996 original (MFC/WinG) and its level designer, CLD, are kept in
[history/](history/README.md).

## Building (Visual Studio 2026)

1. Open `Cylu.sln`.
2. Build. On the first build Visual Studio restores the **Allegro 5.2.11.3**
   NuGet package (and `AllegroDeps`) into `packages\`. If it doesn't, right-click
   the solution and choose **Restore NuGet Packages**.
3. Press F5 to run.

| Configuration | Allegro linking | Output |
|---|---|---|
| Debug | Allegro DLLs (debug), copied next to the exe | `bin\x64\Debug\cylu.exe`, opens in a window, console for `printf` output |
| Release | Static monolith, static C runtime | `bin\x64\Release\cylu.exe`, single exe, starts full screen |

The build copies the `data\` folder next to the exe. To run the game somewhere
else, copy `cylu.exe` and `data\` together (plus the Allegro DLLs in Debug).

The Allegro add-ons used are Image, Font, Primitives and Dialog. You can change
them under **Project Properties > Allegro 5**.

## Running

| Option | Effect |
|---|---|
| `-window` | Start in a window |
| `-fullscreen` | Start full screen |
| `-norotate` | Don't rotate the bouncy text and menu stars |
| `-novsync` | Don't limit the frame rate to the 70Hz game clock |
| `-nosound` | Reserved, as the game has no sound yet |

**Alt+Enter** or **F11** switches between windowed and full screen. The game
still renders at its original 640x480 and is scaled to fit, keeping the 4:3
aspect ratio. You can resize the window freely.

Saved games are stored in `%APPDATA%\Apex\Cylu\savegame`. An old `savegame` from
version 1.06 uses the same format and can be copied there.

## Project layout

| Path | Contents |
|---|---|
| `main.cpp` | Game logic, rendering and menus (the original code, ported) |
| `maze.*`, `anim.*`, `sprite.*` | Level storage and sprite animation (unchanged logic) |
| `platform.*` | Allegro 5 layer: window, scaling, 70Hz clock, keyboard |
| `gfx.*` | Drawing helpers and the sprite loader |
| `cyluobjs.h` | Sprite numbers (`CYLU_*`), matching `data\manifest.txt` |
| `data\sprites\*.png` | The 187 sprites, converted from the old `cylu.dat` |
| `data\level.cld` | The level map |
| `alpha\`, `scene\` | Original source art (letter bitmaps and POV-Ray/POVLab scenes) |
| `tools\extract_dat.py` | Converter from the Allegro 4 `cylu.dat` to the PNGs (needs Python and Pillow) |

## Notes on the port

* The Allegro 4 game ran off `retrace_count` (70Hz). The Allegro 5 version
  runs a 70Hz timer, so movement, fuel use and animation keep their original
  speed on any monitor refresh rate.
* Sprites were stored as 24-bit RLE sprites with magenta (255,0,255)
  transparency. They're now PNGs with an alpha channel. `draw_lit_rle_sprite`
  (used to fade the edges of the map while scrolling) is reproduced with a
  tinted draw.
* `cylu.dat` has been retired. It can still be found in the Git history
  (baseline commit) if you ever need to re-run the converter.
