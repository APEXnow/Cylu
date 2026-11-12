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
| `-level <file>` | Play a different `.cld` level (CLD's "Test in Cylu" uses this) |

**Alt+Enter** or **F11** switches between windowed and full screen. The game
still renders at its original 640x480 and is scaled to fit, keeping the 4:3
aspect ratio. You can resize the window freely.

Saved games are stored in `%APPDATA%\Apex\Cylu\savegame`. An old `savegame` from
version 1.06 uses the same format and can be copied there.

## CLD, the Cylu Level Designer

`cld\` is the level editor, written with MFC in 1996 and brought back to life
alongside the game. It's the second project in `Cylu.sln` and builds `cld.exe`
into the same folder as `cylu.exe`.

**It needs MFC.** In the Visual Studio Installer, choose **Modify**, then
**Desktop development with C++**, and tick **C++ MFC for latest build tools
(x86 & x64)**.

To edit the game's level, open **`data\level.cld` in the repository**, not the
copy in `bin\`. The build overwrites the copy in `bin\` every time.

| Action | How |
|---|---|
| Place the selected object | Left click or drag (Place mode, **P**) |
| Erase | Right click or drag, or Delete mode (**D**) |
| Lines and boxes | **L** / **B** then drag; hold Shift for a filled box |
| Choose an object | Object palette, **S** (Select Object), or Ctrl+click a tile on the map |
| Change the attribute | **]** and **[** (wall type, forcefield number, teleporter number...) |
| Undo / redo | Ctrl+Z / Ctrl+Y |
| Zoom | Ctrl + mouse wheel, Ctrl++ and Ctrl+- |
| Grid / attribute numbers | **G** / **A** |
| Parse Level | **F7**: statistics plus a list of problems; double-click one to jump to it |
| Test in Cylu | **F5**: plays the level being edited in the game, without saving it |
| Test in Cylu 1996 | **Shift+F5**: the same, in the original 1996 game |

Placing the droid moves it, because a level has exactly one starting point.
`cld\levelcheck.cpp` holds the level checks. It's plain C++, so it can be
tested away from MFC.

## Cylu 1996

`cylu96\` rebuilds the original Windows 95 game from 1996 (MFC and GDI), the
version CLD was written for, as `cylu96.exe`. The untouched source stays in
`history\1996\CYLU`. `cylu96\` holds lightly modernised copies, with every change
marked `2026`. Its bitmaps, animation script (`SPRITES.SSF`) and level come
straight from the archive. It needs MFC, like CLD.

Start a game with **File > New Game**. Arrows move, **Shift** selects a pocket,
**P** picks up, **D** drops and **U** uses. Chips, objects and refuel cells are
used at the base computer: stand on the start square facing north. Run
`cylu96.exe <level.cld>` to play another level. CLD's **Test in Cylu 1996**
(Shift+F5) does this for the level being edited.

## Project layout

| Path | Contents |
|---|---|
| `main.cpp` | Game logic, rendering and menus (the original code, ported) |
| `maze.*`, `anim.*`, `sprite.*` | Level storage and sprite animation (unchanged logic) |
| `platform.*` | Allegro 5 layer: window, scaling, 70Hz clock, keyboard |
| `gfx.*` | Drawing helpers and the sprite loader |
| `objects.h` | Object numbers and attributes, shared by the game and CLD |
| `cld\` | CLD, the level designer (MFC) |
| `cylu96\` | The 1996 game, rebuilt (MFC) |
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
