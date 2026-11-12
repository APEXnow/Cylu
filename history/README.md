# Cylu history

Cylu has been written three times:

| Version | Year | Platform | Graphics | Where |
|---|---|---|---|---|
| 1 | 1996 | Windows 95, MFC + WinG, Visual C++ 4 (later opened in VC++ 6) | Hand-drawn 8-bit sprites with separate masks | [`1996/CYLU`](1996/CYLU) |
| 2 | 2001 | DOS/Windows/Linux, Allegro 4, DJGPP then Dev-C++ | Pre-rendered with POV-Ray and POVLab | First commit of this repo |
| 3 | 2026 | Windows 11 x64, Allegro 5.2, Visual Studio 2026 | Version 2's renders as PNGs | Root of this repo |

## 1996: the original Windows game (`1996/CYLU`)

- MFC document/view application (`CCyluApp`, `CCyluView`) using **WinG** for
  fast back-buffer drawing and an 8-bit palette (`PALETTE.PAL`).
- Each sprite is a pair of bitmaps under `SPRITES/SPRITE` and `SPRITES/MASK`,
  drawn with the classic mask-then-sprite blit for transparency.
- `SPRITES.SSF` is a small animation script
  (`ANIMATION, sequence, start ID, frames, ROTATE | BOUNCE | STATIC`) read by
  `CPictureBox` and `CAnimate`. It's the ancestor of `g_pSpriteList` in the
  2001 code.
- `CMaze` and the `.CLD` level format (file ID 12345) survive almost unchanged
  in every later version.

## 1996: CLD, the Cylu Level Designer (`1996/CLD`)

An MFC editor for `.CLD` level files, with a palette of 20×20 tile icons
(`BITMAPS/*.BMP`) for walls, forcefields, keys, teleporters, fuel, CPUs, objects
and trees. It shares `Common.hpp` with the game. The format hasn't changed, so
CLD's files are the same format the current game loads from `data/level.cld`.

## How the level changed

All versions use the same 52×66 map, with 24 objects to collect, 31 forcefields
and their CPU chips, 30 fuel cells and 10 teleporters and keys. The main
difference: the 1996 map has 236 trees (object 11, an animated "bouncing" tree).
In 2001 they became walls and the tree object was removed.

## Not included

IDE caches (`.ncb`, `.opt`, `.aps`, `.plg`), Paint Shop Pro browse files and the
old 1996 `Cylu.exe`. The exe needs WinG, which doesn't run on modern Windows.
