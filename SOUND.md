# Sound System Specification -- RetroWave 4M

---

## Overview

The RetroWave 4M sound system covers four areas:

1. **Text-to-Speech (TTS)** -- offline voice synthesis for accessibility and BASIC programs.
2. **Sampled audio** -- playback and recording of audio files.
3. **Synthesis engine** -- real-time instrument synthesis with presets.
4. **SoundFont instruments** -- SF2-based instrument library.

A tracker-style **F4 Music Utility** provides accessible, mouse-free composition. A separate
**Graphical DAW** companion app provides a fuller, LMMS-like experience.

Audio is the only subsystem permitted to run in the background while BASIC executes.

---

## 1. Text-to-Speech (TTS)

### Engine

- **Piper (primary):** offline, high-quality neural voices. Preferred when a matching Piper voice
  model is installed.
- **eSpeak NG (fallback):** used when Piper is not available for the requested language; broad
  language coverage.
- **No cloud connection required.** All TTS runs fully offline.

### Voice loading and memory

- Piper voices are loaded **lazily** (not at boot).
- Cached voices use an **LRU policy** with a configurable memory budget (`TTS_CACHE_MB`).
- Under memory pressure, least-recently-used voices are unloaded.

### Queue semantics (normative)

TTS output is **queued**.

- Every call to `TTS` creates one *utterance* and pushes it to a FIFO queue.
- The system plays **only one utterance at a time** (no overlap).
- If `TTS` is called while speech is playing, the new utterance is **queued** and does NOT
  implicitly interrupt the current utterance.

Use `SAYSTOP` / `SAYFLUSH` to explicitly control playback and the queue.

### BASIC TTS commands

#### `TTS text$ [, lang$ [, mode$]]`

Speaks the given text.

- `text$` -- the text to speak (UTF-8).
- `lang$` (optional) -- BCP-47 language tag or short code (e.g. `"it-IT"`, `"en-US"`, `"en"`).
  If omitted, the current default Piper voice is used.
- `mode$` (optional) -- `"TEXT"` (default, natural reading) or `"CODE"` (for BASIC listings).

#### `SAYSTOP`

Stops current utterance immediately and clears the entire queue.

#### `SAYFLUSH`

Does NOT interrupt the current utterance; clears all pending queued utterances so nothing is
spoken after the current one finishes.

#### `SAYSTATUS()` -> status

Returns: `0` = idle, `1` = speaking, `2` = paused.

#### `SAYQUEUE()` -> n

Returns the number of queued utterances **excluding** the one currently playing.

### Examples

```basic
10 TTS "Loading..."
20 TTS "Graphics ready."
30 TTS "Network ready."
40 ' Spoken in order. No overlap. No implicit interruption.
```

```basic
10 TTS "One"
20 TTS "Two"
30 TTS "Three"
40 SAYFLUSH
50 ' Finishes "One", then stops (Two and Three are flushed).
```

```basic
10 TTS "This is a long sentence."
20 SAYSTOP
30 ' Speech stops immediately; queue is cleared.
```

---

## 2. Sampled Audio

- **Formats:** WAV, MP3, OGG (playback); WAV (record).
- **Playback:** single channel; looping supported.
- **Recording:** live audio input to WAV file.
- Max file size: 200 MB (system-wide sandbox limit).

---

## 3. Synthesisers

### Engine types

| Type | Description |
|------|-------------|
| Virtual Analog | Classic oscillators (sawtooth, square, sine, triangle, noise); ADSR envelope; low-pass / high-pass / band-pass filters |
| FM | Frequency modulation synthesis; operator stacks; classic DX7-style patches |
| Granular | Grain-based time-stretching and pitch-shifting |

### Common parameters

- **Oscillators:** waveform, frequency, detune.
- **ADSR envelope:** Attack, Decay, Sustain, Release times and levels.
- **Filters:** cutoff frequency, resonance, filter type.
- **Effects:** reverb, delay, chorus, equalisation.
- **Portamento:** glide between notes; configurable time.

### Synthwave Preset Bank

Inspired by 1980s synthesisers (Roland Juno, Oberheim, DX7-style FM pads). Presets are named
with stable machine-readable identifiers; UI display names may be localised.

#### LEAD_SYNTH
`NEON_VIPER_LEAD`, `LASER_FIFTH_LEAD`, `CHROME_PULSE_LEAD`, `ARCADE_SOLO_LEAD`,
`NIGHT_RIDER_PORTA`

#### PAD
`VHS_GLOW_PAD`, `MIDNIGHT_HORIZON_PAD`, `LUSH_ANALOG_PAD`, `GLASS_DAWN_PAD`,
`SLOW_DRIFT_PAD`

#### ARPEGGIO_BASS
`GRID_RUNNER_ARP_BASS`, `STEP_SEQ_MONO_BASS`, `METRO_PULSE_ARP`, `NEON_CHASE_BASS`,
`CLOCKWORK_ARP_BASS`

#### PLUCK
`PIXEL_PLUCK`, `CRYSTAL_PICK_PLUCK`, `SHORT_GATE_PLUCK`, `FM_KEY_PLUCK`, `BUBBLY_PLUCK`

#### CHOIR_VOX
`VOX_AAH_STACK`, `FORMANT_CHOIR_PAD`, `VAPOR_VOX`, `TALKBOX_LEAD`, `NEON_CHOIR_HIT`

#### DRUMS_808
`EIGHT_O_EIGHT_PUNCH_KIT`, `LINN_NIGHT_KIT`, `ELECTRO_TOMS`, `GATED_SNARE_80S`, `TAPE_HATS`

#### ATMOSPHERES
`CITY_RAIN_ATMOS`, `SPACE_DUST_DRONE`, `DARK_ALLEY_AIR`, `DISTANT_SIGNAL_BED`,
`SUNRISE_NOISE_WASH`

#### SUPERSAW (cross-genre)
`SUPERSAW_ANTHEM`, `SUPERSAW_CHORD_STACK`, `SUPERSAW_WIDE_LEAD`, `SUPERSAW_SLOW_BLOOM`,
`SUPERSAW_DETUNE_RUSH`

#### OVERDRIVE (cross-genre)
`OVERDRIVE_BASS_GROWL`, `OVERDRIVE_LEAD_SCREAM`, `OVERDRIVE_PULSE_STAB`,
`OVERDRIVE_ARP_GRIND`, `OVERDRIVE_PAD_RASP`

---

## 4. SoundFont Instruments (SF2)

- **Format:** SF2 (SoundFont 2).
- **Delivery model:** one SF2 file per instrument; registered in `soundfonts/index.json`.
- **User soundfonts:** drop into `soundfonts/user/`; auto-discovered at startup.
- **Lazy loading:** soundfonts are loaded on first use (see section 8 below).

### Bundled Ethnic / Acoustic Instruments

| Instrument | Origin / Tradition |
|-----------|-------------------|
| Kora | West Africa (21-string bridge harp-lute) |
| Sitar | North India (long-neck plucked lute) |
| Tabla | North India (paired hand drums) |
| Oud | Middle East / North Africa (fretless lute) |
| Erhu | China (two-string bowed spike fiddle) |
| Koto | Japan (13-string zither) |
| Shakuhachi | Japan (end-blown bamboo flute) |
| Charango | Andes -- Bolivia/Peru (small 10-string lute) |
| Tin Whistle | Celtic / Ireland (six-hole flageolet) |
| Balafon | West Africa (wooden keyed percussion) |
| Duduk | Armenia / Caucasus (double-reed woodwind) |
| Didgeridoo | Australia -- Aboriginal (circular-breathing aerophone) |

### Bundled Synth-Based Instruments

| Instrument | Description |
|-----------|-------------|
| Synthpad | Slow-attack pad; lush chordal background texture |
| Supersaw | Detuned sawtooth stack; classic trance/eurobeat lead |
| Hoover | Aggressive rising-pitch synth stab |
| Pulse Bass | Square/pulse-wave bass with filter sweep |
| Glass Bell | FM-derived glassy bell tone |
| Choir Aah (Synth) | Vocoder/formant-based synthetic choir |
| Analog Strings | Detuned string ensemble emulation |

> Synth-based SF2 soundfonts are modelled rather than sampled; tonal accuracy depends on the
> SF2 engine's synthesis capabilities.

---

## 5. Audio Tracks and Mixer

- **Track count:** unlimited audio tracks.
- **Mixer:** per-track volume, panning, and effects; master volume control.
- **BPM:** configurable beats per minute; default tuning A = 440 Hz.
- **Effects bus:** reverb, delay, equalisation (shared and per-track).
- **Visual feedback:** real-time level meters.

### BASIC sound commands

```basic
SOUND        ' initiate sound
NOTE  pitch, duration
REST  duration
BPM   value
TUNING value
TRACK PLAY id
```

---

## 6. F4 Music Utility (Tracker)

The F4 utility is a text-based, accessible tracker. It runs in text/accessible mode and requires
no mouse.

### Sections

**Instrument panel:** select a soundfont from the library, or build a synth sound from oscillators
with ADSR/filter/FX. Multiple instruments can be defined per track.

**Tracker grid:** tabular layout (classic tracker style). Each channel column lists:
- Note, selected instrument, duration, optional rest.
- Channel volume (mixer) and global BPM.

Additional features: chords, portamento, fade in/out, microtones (optional).
External WAV/MP3 files can be included alongside synthesised tracks.

**Export options:**
- BASIC `TRACK` commands (for embedding in a BASIC listing and playing with `TRACK PLAY id`).
- WAV audio file.

---

## 7. Graphical DAW (Companion App)

A separate companion application (not a resident utility) providing a more visual DAW experience,
inspired by LMMS but much lighter in scope:

- Timeline-based arrangement view.
- Instrument and effects rack.
- Piano roll for note entry.
- Accessibility-minded: keyboard-navigable, TTS-compatible labels.
- Export: WAV and MP3.

---

## 8. Lazy Loading Strategy

- Audio assets (samples, soundfonts, presets) are loaded on demand when first played, not at
  startup.
- A short pre-buffer period is allowed before playback begins to avoid audible glitches.
- Loaded assets are cached; cache can be flushed manually or automatically under memory pressure.
- Frequently-used assets (UI sounds, common SFX) can be marked for eager loading at startup via
  a manifest file.
- Synthwave presets are loaded lazily per-preset; only the preset metadata index loads at startup.
- Piper voice models are loaded lazily with an LRU policy and a configurable memory budget
  (`TTS_CACHE_MB`).
