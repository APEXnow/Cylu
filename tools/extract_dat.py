"""Convert the Allegro 4 cylu.dat into files Allegro 5 can load.

Usage:  python tools/extract_dat.py [cylu.dat] [outdir]

Writes one PNG per sprite/bitmap (magenta 255,0,255 -> transparent),
the MAZE object as level.cld, and data/manifest.txt (index -> file).
"""
import os, struct, sys
from PIL import Image
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import datlib

MASK = (255, 0, 255)

def rgba(r, g, b):
    return (0, 0, 0, 0) if (r, g, b) == MASK else (r, g, b, 255)

def decode_bmp24(raw):
    bits, w, h = struct.unpack('>hhh', raw[:6]); assert bits == 24
    px = raw[6:]; assert len(px) == w * h * 3
    img = Image.new('RGBA', (w, h))
    img.putdata([rgba(px[i], px[i+1], px[i+2]) for i in range(0, len(px), 3)])
    return img

def decode_rle24(raw):
    bits, w, h, _size = struct.unpack('>hhhi', raw[:10]); assert bits == 24
    p = 10; pixels = []
    for y in range(h):
        row = []
        while True:
            c = struct.unpack('<i', raw[p:p+4])[0]; p += 4
            if (c & 0xFFFFFFFF) == 0x00FF00FF: break
            if c < 0:
                row += [(0, 0, 0, 0)] * (-c)
            else:
                for _ in range(c):
                    row.append(rgba(raw[p], raw[p+1], raw[p+2])); p += 3
        row += [(0, 0, 0, 0)] * (w - len(row))
        assert len(row) == w, (len(row), w)
        pixels += row
    assert p == len(raw), (p, len(raw))
    img = Image.new('RGBA', (w, h)); img.putdata(pixels)
    return img

def main():
    src = sys.argv[1] if len(sys.argv) > 1 else 'cylu.dat'
    out = sys.argv[2] if len(sys.argv) > 2 else 'data'
    os.makedirs(os.path.join(out, 'sprites'), exist_ok=True)
    objs = datlib.read_datafile(datlib.open_packfile(open(src, 'rb').read(), b'NULU'))
    with open(os.path.join(out, 'manifest.txt'), 'w', newline='\n') as man:
        for i, o in enumerate(objs):
            t = o['type']
            if t == 'RLE ' or t == 'BMP ':
                img = decode_rle24(o['raw']) if t == 'RLE ' else decode_bmp24(o['raw'])
                rel = 'sprites/%s.png' % o['name'].lower()
                img.save(os.path.join(out, rel))
            elif t == 'MAZE':
                rel = 'level.cld'
                open(os.path.join(out, rel), 'wb').write(o['raw'])
            else:
                rel = '-'
            man.write('%d %s %s\n' % (i, o['name'], rel))
    print('extracted %d objects to %s' % (len(objs), out))

if __name__ == '__main__':
    main()
