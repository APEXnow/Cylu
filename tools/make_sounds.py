"""Synthesise Cylu's sound effects (sfxr-style), written as 16-bit mono WAVs.

Usage:  python tools/make_sounds.py [outdir]      (default: data/sounds)

Every sound is generated from the recipes below, so they're original and
can be tweaked and regenerated at any time.  Needs numpy.
"""
import os, sys, wave
import numpy as np

RATE = 22050
rng = np.random.default_rng(1996)          # fixed seed: same sounds every run

def t_axis(dur):
    return np.arange(int(RATE * dur)) / RATE

def sweep(f0, f1, dur, curve=1.0):
    """Frequency track from f0 to f1 (curve > 1 lingers near f0)."""
    x = np.linspace(0, 1, int(RATE * dur)) ** curve
    return f0 + (f1 - f0) * x

def phase(freq):
    return np.cumsum(freq) / RATE

def osc(freq, shape='square', duty=0.5):
    p = phase(freq) % 1.0
    if shape == 'square':   return np.where(p < duty, 1.0, -1.0)
    if shape == 'saw':      return 2 * p - 1
    if shape == 'triangle': return 1 - 4 * np.abs(p - 0.5)
    if shape == 'sine':     return np.sin(2 * np.pi * p)
    raise ValueError(shape)

def noise(n, smooth=1):
    s = rng.uniform(-1, 1, n)
    if smooth > 1:                          # crude low-pass by sample-and-hold
        s = np.repeat(s[::smooth], smooth)[:n]
    return s

def env(n, attack=0.005, release=None, sustain=0.0):
    """Attack, optional flat sustain (s), then decay to zero."""
    a = int(RATE * attack); s = int(RATE * sustain)
    e = np.ones(n)
    if a: e[:a] = np.linspace(0, 1, a)
    d = n - a - s
    if d > 0: e[a + s:] = np.linspace(1, 0, d) ** 1.5
    return e

def lowpass(x, cutoff):
    k = np.exp(-2 * np.pi * cutoff / RATE)
    y = np.empty_like(x); acc = 0.0
    for i, v in enumerate(x):
        acc = (1 - k) * v + k * acc; y[i] = acc
    return y

def note(f, dur, shape='triangle', vol=1.0, attack=0.005):
    x = osc(np.full(int(RATE * dur), f), shape) * env(int(RATE * dur), attack)
    return x * vol

def cat(*parts, gap=0.0):
    g = np.zeros(int(RATE * gap))
    out = []
    for p in parts: out += [p, g]
    return np.concatenate(out[:-1])

def mix(*parts):
    n = max(len(p) for p in parts)
    return sum(np.pad(p, (0, n - len(p))) for p in parts)

NOTE = lambda name: 440.0 * 2 ** ((
    {'C':-9,'D':-7,'E':-5,'F':-4,'G':-2,'A':0,'B':2}[name[0]] + 12 * (int(name[-1]) - 4)) / 12)

# --------------------------------------------------------------------------
# The sounds.  Each returns a numpy array in the range -1..1.
# --------------------------------------------------------------------------

def step():          # droid moves one square: soft servo tick
    d = 0.07
    x = osc(sweep(190, 110, d), 'square', 0.3) * 0.5 + noise(int(RATE*d), 3) * 0.3
    return lowpass(x * env(int(RATE*d), 0.002), 1800) * 0.55

def bump():          # walks into a wall: dull thud
    d = 0.16
    x = osc(sweep(95, 45, d), 'sine') + noise(int(RATE*d), 6) * 0.5
    return lowpass(x * env(int(RATE*d), 0.002), 600) * 0.65

def pickup():        # object into a pocket: rising blip
    d = 0.14
    f = np.concatenate([np.full(int(RATE*0.05), 520.0), sweep(780, 1100, d - 0.05)])
    return osc(f, 'square', 0.4) * env(len(f), 0.002) * 0.4

def drop():          # object put down: falling blip
    d = 0.16
    return osc(sweep(900, 320, d, 0.7), 'square', 0.4) * env(int(RATE*d), 0.002) * 0.4

def deposit():       # object deposited in the base computer: chime
    def bell(n, d):
        f = NOTE(n)
        return mix(note(f, d, 'sine', 0.6), note(f * 2, d, 'sine', 0.2), note(f * 3, d * 0.5, 'sine', 0.08))
    return cat(bell('C6', 0.09), bell('E6', 0.09), bell('G6', 0.6)) * 0.7

def forcefield_off():   # CPU chip disables a forcefield: zap dying away
    d = 0.8
    n = int(RATE * d)
    buzz = osc(sweep(140, 40, d, 0.6), 'saw')
    crackle = noise(n, 2) * (rng.uniform(size=n) > 0.6)
    trem = 0.6 + 0.4 * np.sign(np.sin(2 * np.pi * phase(sweep(28, 6, d))))
    x = (buzz * 0.7 + crackle * 0.6) * trem * env(n, 0.003)
    return lowpass(x, 3000) * 0.75

def teleport():      # key used on a teleporter: sweeping warble
    d = 0.9
    base = np.concatenate([sweep(250, 1300, d * 0.55, 0.8), sweep(1300, 400, d * 0.45, 1.2)])
    vib = 1 + 0.08 * np.sin(2 * np.pi * 22 * t_axis(len(base) / RATE))
    x = osc(base * vib, 'triangle') * 0.7 + osc(base * vib * 1.5, 'sine') * 0.3
    return x * env(len(x), 0.03) * 0.5

def refuel():        # refuel: stepped rising tone
    steps = [NOTE(n) for n in ('C4', 'E4', 'G4', 'C5', 'E5', 'G5', 'C6')]
    return cat(*[note(f, 0.07, 'square', 0.35, 0.002) for f in steps])

def fuel_low():      # fuel running low: two warning beeps
    b = note(880, 0.11, 'square', 0.35, 0.002)
    return cat(b, b, gap=0.08)

def menu_move():     # menu cursor / pocket select: tick
    return osc(np.full(int(RATE*0.03), 1500.0), 'square', 0.25) * env(int(RATE*0.03), 0.001) * 0.3

def menu_select():   # menu choice confirmed
    return cat(note(NOTE('E5'), 0.07, 'square', 0.3, 0.002), note(NOTE('B5'), 0.16, 'square', 0.3, 0.002))

def whoosh():        # bouncy text flying in
    d = 0.35
    n = int(RATE * d)
    x = noise(n)
    cut = np.linspace(400, 3500, n)
    y = np.empty(n); acc = 0.0
    for i in range(n):                       # sweeping low-pass
        k = np.exp(-2 * np.pi * cut[i] / RATE); acc = (1 - k) * x[i] + k * acc; y[i] = acc
    e = np.sin(np.linspace(0, np.pi, n)) ** 2
    return y * e * 1.2

def game_over():     # life support failed: descending jingle
    seq = [('G4', 0.22), ('E4', 0.22), ('C4', 0.22), ('G3', 0.7)]
    return cat(*[mix(note(NOTE(n), d, 'triangle', 0.6), note(NOTE(n) / 2, d, 'sine', 0.3)) for n, d in seq], gap=0.02)

def win():           # test completed: ascending fanfare
    seq = [('C5', 0.12), ('E5', 0.12), ('G5', 0.12), ('C6', 0.12), ('G5', 0.1), ('C6', 0.8)]
    return cat(*[mix(note(NOTE(n), d, 'square', 0.22, 0.003), note(NOTE(n) / 2, d, 'triangle', 0.35)) for n, d in seq], gap=0.015)

SOUNDS = [step, bump, pickup, drop, deposit, forcefield_off, teleport, refuel,
          fuel_low, menu_move, menu_select, whoosh, game_over, win]

def write_wav(path, x):
    peak = np.max(np.abs(x))
    if peak > 0.98: x = x * (0.98 / peak)        # never clip
    pcm = (x * 32767).astype('<i2')
    with wave.open(path, 'wb') as w:
        w.setnchannels(1); w.setsampwidth(2); w.setframerate(RATE)
        w.writeframes(pcm.tobytes())

def main():
    out = sys.argv[1] if len(sys.argv) > 1 else os.path.join('data', 'sounds')
    os.makedirs(out, exist_ok=True)
    for fn in SOUNDS:
        write_wav(os.path.join(out, fn.__name__ + '.wav'), fn())
    print('wrote %d sounds to %s' % (len(SOUNDS), out))

if __name__ == '__main__':
    main()
