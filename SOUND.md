# Sound System Specification

## Overview
This document outlines the comprehensive sound system specification for the RetroWave-4M project. It includes details on text-to-speech, sampled sound, synthesizers, soundfont instruments, audio tracks, mixers, and 3D sound capabilities.

## 1. Text-to-Speech (TTS)
- **Engine**: Utilizes advanced TTS engine for realistic voice synthesis.
- **Languages Supported**: English, Spanish, French, and more.
- **Custom Voices**: Ability to create and integrate custom voices.

### Offline implementation notes (normative)

Target: Raspberry Pi 400 (offline-first home computer).

#### Engines
- **Piper (primary):** preferred when a matching Piper voice/model is installed (higher-quality voices).
- **eSpeak NG (fallback):** used when Piper is not available for the requested language (broad coverage).

#### Utterance queue semantics
- Each `TTS` call MUST create one *utterance* and enqueue it in a **FIFO queue**.
- The system MUST synthesize/play **only one utterance at a time** (**no overlap**).
- New utterances MUST NOT implicitly interrupt the currently playing utterance (**no implicit suppression**).

#### Control commands
- `SAYSTOP` MUST stop playback immediately and clear the pending utterance queue.
- `SAYFLUSH` MUST NOT interrupt the currently playing utterance; it clears the pending queue so that no further speech occurs after the current utterance completes.
- `SAYSTATUS()` returns: `0` idle, `1` speaking, `2` paused (only if pause/resume exists).
- `SAYQUEUE()` returns the number of queued utterances **excluding** the one currently playing.

#### Lazy loading and memory (recommended)
- Piper voices SHOULD be loaded lazily (not at boot).
- Cached Piper voices SHOULD use an LRU policy with a configurable memory budget (e.g. `TTS_CACHE_MB`).
- Under memory pressure (budget exceeded), least-recently-used voices SHOULD be unloaded.

## 2. Sampled Sound
- **Library**: Includes a wide range of pre-recorded sounds.
- **Formats Supported**: WAV, MP3, OGG.
- **Functionality**: Supports playback, looping, and layering of sounds.

## 3. Synthesizers
- **Types**: Virtual Analog, FM, Granular.
- **Specifications**: Adjustable parameters for oscillators, filters, and envelopes.
- **Presets**: Ships with a variety of presets for easy use.

### Synthwave Preset Bank
Inspired by the sonic palette of 1980s synthesizers (Roland Juno, Oberheim, DX7-style FM pads, etc.), the Synthwave preset bank includes:
- **Lead Synth** – Bright, cutting leads for melodic lines.
- **Pad** – Lush, sustained pads for atmosphere.
- **Arpeggio Bass** – Sequenced bass patterns in classic synthwave style.
- **Pluck** – Short, percussive plucked tones.
- **Choir/Vox** – Vocal and choir-style synthesis.
- **Drums/808** – Drum machine and 808-style percussive sounds.
- **Atmospheres** – Ambient soundscapes and textural effects.

Presets are named and browsable. Users may save modified presets as custom entries.

## 3A. Synth Preset Library (Bundled)

Preset identifiers are stable, machine-readable strings used by code and UI components. UI display names may be localized for different languages, but identifiers always remain constant. Presets are grouped by category to make browsing easier for users and developers.

### Preset Identifier Groups

#### LEAD_SYNTH
- `NEON_VIPER_LEAD`
- `LASER_FIFTH_LEAD`
- `CHROME_PULSE_LEAD`
- `ARCADE_SOLO_LEAD`
- `NIGHT_RIDER_PORTA`

#### PAD
- `VHS_GLOW_PAD`
- `MIDNIGHT_HORIZON_PAD`
- `LUSH_ANALOG_PAD`
- `GLASS_DAWN_PAD`
- `SLOW_DRIFT_PAD`

#### ARPEGGIO_BASS
- `GRID_RUNNER_ARP_BASS`
- `STEP_SEQ_MONO_BASS`
- `METRO_PULSE_ARP`
- `NEON_CHASE_BASS`
- `CLOCKWORK_ARP_BASS`

#### PLUCK
- `PIXEL_PLUCK`
- `CRYSTAL_PICK_PLUCK`
- `SHORT_GATE_PLUCK`
- `FM_KEY_PLUCK`
- `BUBBLY_PLUCK`

#### CHOIR_VOX
- `VOX_AAH_STACK`
- `FORMANT_CHOIR_PAD`
- `VAPOR_VOX`
- `TALKBOX_LEAD`
- `NEON_CHOIR_HIT`

#### DRUMS_808
- `EIGHT_O_EIGHT_PUNCH_KIT`
- `LINN_NIGHT_KIT`
- `ELECTRO_TOMS`
- `GATED_SNARE_80S`
- `TAPE_HATS`

#### ATMOSPHERES
- `CITY_RAIN_ATMOS`
- `SPACE_DUST_DRONE`
- `DARK_ALLEY_AIR`
- `DISTANT_SIGNAL_BED`
- `SUNRISE_NOISE_WASH`

### Cross-Genre Synth Features

The following identifiers represent **synthesis features/styles** rather than genre-specific preset banks. They may be applied across any bank or genre and are not limited to synthwave contexts.

#### SUPERSAW (supersaw oscillator stack / detune chorus style)
Multiple detuned sawtooth oscillators layered to produce a wide, rich sound common in trance, eurobeat, synthwave, and pop production:
- `SUPERSAW_ANTHEM`
- `SUPERSAW_CHORD_STACK`
- `SUPERSAW_WIDE_LEAD`
- `SUPERSAW_SLOW_BLOOM`
- `SUPERSAW_DETUNE_RUSH`

#### OVERDRIVE (drive/distortion stage)
A clipping or waveshaping stage applied to the signal path, adding harmonic saturation and grit. Usable across bass, lead, pad, and arp sounds in any genre:
- `OVERDRIVE_BASS_GROWL`
- `OVERDRIVE_LEAD_SCREAM`
- `OVERDRIVE_PULSE_STAB`
- `OVERDRIVE_ARP_GRIND`
- `OVERDRIVE_PAD_RASP`

## 4. Soundfont Instruments
- **Compatibility**: Supports SF2 soundfonts.
- **Integration**: Allows users to load and create custom instruments.
- **Features**: Includes basic editing tools for modifying soundfonts.
- **Delivery model**: One SF2 file per instrument, registered in `soundfonts/index.json` (aligned with §8 Lazy Loading). Custom user soundfonts can be dropped into the `soundfonts/user/` directory and are auto-discovered at startup.

### Bundled Instrument Library

#### Ethnic / Acoustic Instruments
A curated selection of ~12 widely recognised acoustic and ethnic instruments, covering a broad range of world music traditions:

| Instrument | Origin / Tradition | Notes |
|---|---|---|
| Kora | West Africa | 21-string bridge harp-lute |
| Sitar | North India | Long-neck plucked lute; includes sympathetic strings |
| Tabla | North India | Paired hand drums; multiple stroke articulations |
| Oud | Middle East / North Africa | Fretless short-neck lute |
| Erhu | China | Two-string bowed spike fiddle |
| Koto | Japan | 13-string zither |
| Shakuhachi | Japan | End-blown bamboo flute |
| Charango | Andes (Bolivia/Peru) | Small 10-string lute; bright, metallic tone |
| Tin Whistle | Celtic / Ireland | Six-hole flageolet; bright, piercing tone |
| Balafon | West Africa | Wooden keyed percussion with gourd resonators |
| Duduk | Armenia / Caucasus | Double-reed woodwind; rich, mournful timbre |
| Didgeridoo | Australia (Aboriginal) | Circular-breathing aerophone; deep drones |

#### Synth-Based Instruments
A set of synthesis-driven timbres delivered as SF2 soundfonts. These are inherently harder to source as authentic recordings and are instead modelled or synthesized:

| Instrument | Description |
|---|---|
| Synthpad | Slow-attack, sustain-heavy pad; lush chordal background texture |
| Supersaw | Detuned stack of sawtooth oscillators; classic trance/eurobeat lead |
| Hoover | Aggressive, rising-pitch synth stab inspired by Rave Hoover |
| Pulse Bass | Square/pulse-wave bass with filter sweep; punchy electronic bass |
| Glass Bell | FM-derived glassy bell tone; crystalline, high-register sound |
| Choir Aah (Synth) | Vocoder/formant-based synthetic choir |
| Analog Strings | Detuned string ensemble emulation reminiscent of Roland Strings |

> **Note:** Synth-based SF2 soundfonts are modelled rather than sampled; tonal accuracy depends on the SF2 engine's synthesis capabilities.

## 5. Audio Tracks
- **Track Count**: Supports unlimited audio tracks.
- **Control**: Provides tools for adjusting volume, panning, and effects for each track.
- **Recording**: Capable of recording live audio in real-time.

## 6. Mixer
- **Channels**: Multi-channel mixing capabilities.
- **Effects**: Built-in effects such as reverb, delay, and equalization.
- **Visuals**: Real-time visual feedback of audio levels and effects.

## 7. 3D Sound Capabilities
- **Surround Sound**: Supports 5.1 and 7.1 surround formats.
- **Positional Audio**: Enables 3D spatial audio integration for immersive experiences.
- **Compatibility**: Works with VR and AR systems for enhanced interaction.

## Conclusion
This sound system will provide robust capabilities to enhance the auditory experience in the RetroWave-4M project. Each section can be expanded with further technical details as development progresses.

## 8. Lazy Loading Strategy
- Audio assets (samples, soundfonts, presets) are loaded on demand when first played, not at startup.
- A short prebuffer period is allowed before playback begins to avoid audible glitches.
- Loaded assets are cached; cache can be flushed manually or automatically under memory pressure.
- Frequently-used assets (UI sounds, common SFX) can be marked for eager loading at startup via a manifest file.
- Synthwave presets are loaded lazily per-preset; only the preset metadata index is loaded at startup.
