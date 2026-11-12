"""Minimal reader for Allegro 4 packfiles/datafiles (password + LZSS)."""
import struct

N, F, THRESHOLD = 4096, 18, 2

def lzss_decode(src):
    out = bytearray(); buf = bytearray(N); r = N - F; flags = 0; p = 0; n = len(src)
    while True:
        flags >>= 1
        if not (flags & 256):
            if p >= n: break
            flags = src[p] | 0xFF00; p += 1
        if flags & 1:
            if p >= n: break
            c = src[p]; p += 1
            out.append(c); buf[r] = c; r = (r + 1) & (N - 1)
        else:
            if p + 1 >= n: break
            i = src[p]; j = src[p + 1]; p += 2
            i |= (j & 0xF0) << 4; j = (j & 0x0F) + THRESHOLD
            for k in range(j + 1):
                c = buf[(i + k) & (N - 1)]
                out.append(c); buf[r] = c; r = (r + 1) & (N - 1)
    return bytes(out)

def xor_pw(data, pw):
    if not pw: return data
    out = bytearray(data); L = len(pw)
    for i in range(len(out)): out[i] ^= pw[i % L]
    return bytes(out)

def open_packfile(raw, pw=b""):
    mask = 0
    for i, c in enumerate(pw): mask ^= c << ((i & 3) * 8)
    if pw: mask ^= 42
    hdr = struct.unpack('>I', raw[:4])[0] ^ mask
    body = xor_pw(raw[4:], pw)
    if hdr == 0x736C6821: return lzss_decode(body)
    if hdr == 0x736C682E: return body
    raise ValueError("bad packfile header %08x" % hdr)

def read_datafile(data):
    """Return list of dicts: name, type, props, raw (object payload)."""
    if data[:4] != b'ALL.': raise ValueError('not a datafile')
    p = 4
    count = struct.unpack('>i', data[p:p+4])[0]; p += 4
    objs = []
    while len(objs) < count:
        props = {}
        while data[p:p+4] == b'prop':
            ptype = data[p+4:p+8].decode('latin1'); sz = struct.unpack('>i', data[p+8:p+12])[0]
            props[ptype] = data[p+12:p+12+sz].decode('latin1'); p += 12 + sz
        otype = data[p:p+4].decode('latin1'); p += 4
        fsize, dsize = struct.unpack('>ii', data[p:p+8]); p += 8
        raw = data[p:p+fsize]; p += fsize
        if dsize < 0: raw = lzss_decode(raw)
        objs.append(dict(name=props.get('NAME', ''), type=otype, props=props, raw=raw))
    return objs
