# RetroWave-4M - Specifiche Tecniche Complete

## 1. Panoramica del Progetto

RetroWave-4M è un home computer moderno con anima retrò, progettato per il Raspberry Pi 400. L'概念 è evolvere l'architettura classica degli home computer Commodore (C16/C64) come se lo sviluppo non si fosse mai interrotto, adattandosi ai tempi moderni ma mantenendo lo spirito originale.

### 1.1 Filosofia

- **Nessun sistema operativo**: ambiente bare-metal, monotasking
- **Accessibilità estrema**: utilizzabile senza monitor, con sintesi vocale e tastiera Braille
- **Low-cost e sostenibile**: 4-6W di consumo, tethering USB, reti mesh
- **Aperto al mondo**: supporto multilingua, Unicode completo, strumenti etnici

### 1.2 Specifiche Hardware

| Componente | Specifica |
|------------|-----------|
| CPU | Broadcom BCM2711 (Raspberry Pi 400) |
| RAM | 4 GB (non espandibile) |
| Storage | SD card (boot) + USB |
| Display | HDMI Full HD (1920x1080) |
| Audio | Jack 3.5mm + USB |
| Networking | Ethernet, WiFi, Bluetooth |
| GPIO | 40 pin header |

---

## 2. Risoluzioni Video

### 2.1 Modalità Video

| Nome | Risoluzione | Scale to HD | Utilizzo |
|------|-------------|-------------|----------|
| SCREEN 0 | 640×360 | 3× | Giochi leggeri |
| SCREEN 1 | 960×540 | 2× | Media complessità |
| SCREEN 2 | 1920×1080 | 1× | Full HD nativo |

### 2.2 Modalità Testo

- **Griglia**: 48×27 caratteri
- **Dimensione carattere**: 40×40 pixel
- **Risoluzione finale**: 1920×1080 (Full HD)
- **Codifica**: Unicode a 2 byte (UTF-16)
- **Buffer**: 1296 caratteri (48×27)

---

## 3. Layer Grafici

I 5 layer vengono renderizzati dal fondo verso l'alto:

### Layer Priorities

```
0. BACKGROUND - Sfondo (statico o animato)
1. 3D         - Rendering tridimensionale
2. SPRITES    - Sprites hardware/software
3. 2D         - Primitive grafiche 2D
4. TEXT       - Buffer caratteri (modalità testo)
```

### 3.1 Background Layer

5 modalità:
1. **SOLID**: Colore singolo
2. **GRADIENT**: Gradiente (verticale, orizzontale, diagonale, radiale)
3. **IMAGE**: Immagine fissa centrata
4. **SCROLLING**: Immagine grande scorrevole (wrap infinito)
5. **TILEABLE**: Tiling infinito con scrolling

**Parallax**: sensibile ai movimenti dei layer superiori
- `parallax_factor_x/y`: 0.0 (fisso) → 1.0 (segue completamente)
- `parallax_smoothing`: interpolazione per transizioni morbide

### 3.2 3D Layer

**Modalità rendering**:
- **WIREFRAME**: Solo spigoli (colore + spessore configurabile)
- **FLAT**: Facce colorate con illuminazione
- **WIREFLAT**: Combinazione wireframe + flat

**Sistema coordinate**:
- Origine al centro (0, 0, 0)
- Asse X: positivo a destra
- Asse Y: positivo in alto
- Asse Z: positivo verso l'osservatore

**Camera**:
- Posizione: x, y, z
- Look-at: x, y, z
- FOV configurabile

**Primitive**:
- Mesh (vertices + indices)
- GRID: superficie definita da matrice A×B di altezze
  - Wireframe: colore + spessore spigoli
  - Flat: matrice colori facce (A-1)×(B-1)
  - Wireflat: entrambi

**Parent/Pivot**: sistema gerarchico di trasformazioni

### 3.3 Sprite Layer

- Dimensioni configurabili
- Animazione (frame count + delay)
- Flip orizzontale/verticale
- Priorità (0=back, 255=front)
- **Collisioni**: bounding box e pixel-perfect

### 3.4 2D Layer

**Primitive supportate**:
- POINT, LINE, RECT, CIRCLE
- FILLED_RECT, FILLED_CIRCLE
- POLYGON

**Stato grafico**:
- `fill_color`: colore riempimento
- `stroke_color`: colore contorno
- `stroke_width`: spessore linea (1-32 pixel)
- `anti_alias`: on/off

**Clip**: regione di clipping automatica

### 3.5 Text Layer

**Buffer caratteri**:
```c
struct {
    uint16_t code_point;  // Unicode
    uint8_t  fg_color;    // Colore foreground
    uint8_t  bg_color;   // Colore background
    uint8_t  attributes;  // Bold, Italic, Underline, Blink, Inverse, Cursor
}
```

**Attributi**:
- RW_CHAR_ATTR_BOLD
- RW_CHAR_ATTR_ITALIC
- RW_CHAR_ATTR_UNDERLINE
- RW_CHAR_ATTR_BLINK
- RW_CHAR_ATTR_INVERSE
- RW_CHAR_ATTR_CURSOR

**Funzionalità**:
- Cursore (lampeggiante)
- Auto-scroll
- Margini configurabili
- Cut/Copy/Paste/Undo/Redo

---

## 4. Sistema Audio

### 4.1 Sezioni Audio

1. **STRUMENTI**: Soundfont interni (preset)
2. **SINTESI**: Oscillatori, filtri, envelope
3. **SAMPLES**: Audio campionato
4. **VOCE**: Text-to-Speech (TTS)

### 4.2 Input

- **Microfono**: registrazione e processing
- **MIDI**: tastiera MIDI via USB

### 4.3 Strumenti Campionati vs Sintetizzati

**Campionati** (file .sf2):
- Pianoforte (grand, upright)
- Batteria
- 4 strumenti etnici:
  - SITAR (India)
  - ERHU (Cina)
  - OUD (Medio Oriente)
  - KORA (Africa Occidentale)

**Sintetizzati**:
- Synth pad
- Bass
- Lead
- Arpeggio
- Effetti

### 4.4 Sistema Track

```basic
' Creazione track
TRACK 1, "melodia"
TRACK 2, "basso"

' Note sulla track
NOTE 1, "C4", 1000  ' track, nota, durata

' Controllo playback
PLAYTRACK        ' suona tutte le track
PAUSETRACK 1     ' pausa track specifica
STOPTRACK        ' ferma tutto
```

### 4.5 Audio Parallelo

- Track: suonate in background
- Suoni singoli: eventi (esplosioni, click)
- Sample: effetti sonori
- Tutto simultaneo

### 4.6 TTS (Text-to-Speech)

- **Voci**: Femminile (default), Maschile
- **Coda**: messi in coda automaticamente
- **Lingua**: configurabile (Italiano, Inglese, ecc.)

### 4.7 Formati

- **Import**: MP3, WAV
- **Export**: WAV
- **Internal**: PCM 16-bit, 44.1kHz stereo

### 4.8 Intanazione

- A4 = 440 Hz (default)
- A4 = 432 Hz (opzionale)
- Configurabile per altre intonazioni

---

## 5. Networking

### 5.1 Priorità Protocolli

| Priorità | Protocollo | Note |
|----------|-----------|------|
| 1 | MESH | Encrypted by default |
| 2 | EMAIL | Client completo |
| 3 | FTP | File transfer |
| 4 | IRC | Chat testuale |
| 5 | HTTP | Browser text-only (Lynx-like) |
| 6 | SSH | (opzionale, forse evitare) |

### 5.2 Mesh Network

- **Encryption**: ON di default
- **Architettura**: peer-to-peer
- **Costo**: nessun abbonamento

### 5.3 Browser Text-Only

- Modalità Lynx (solo testo)
- Accessibile a non vedenti
- Limita surface attack

### 5.4 Sicurezza

- Minima attack surface
- Nessun browser grafico
- Mesh encrypted
- Email con supporto PGP

---

## 6. Accessibilità

### 6.1 Non Vedenti

- **TTS**: sempre attivo (configurabile)
- **Monitor**: non necessario
- **Tastiera Braille**: supportata
- **Shortcut vocali**:
  - F1: Manuale (multilingua, letto automaticamente)
  - Fn: Coordinate cursore (letto vocalmente)
  - Fn: Riga corrente (letta vocalmente)

### 6.2 Shortcut Text Mode

| Tasto | Funzione |
|-------|----------|
| Ctrl+X | Cut |
| Ctrl+C | Copy |
| Ctrl+V | Paste |
| Ctrl+Z | Undo |
| Ctrl+Y | Redo |
| F1 | Help |
| F2 | Salva file |
| F3 | Carica file |

### 6.3 Headless Mode

- Utilizzo senza monitor
- Tutto accessibile via TTS
- Input/output seriale o SSH limitato

---

## 7. Linguaggio BASIC

### 7.1 Strutture Dati

```basic
' Array
DIM array(100)

' Matrici
DIM matrix(10, 10)
MATRIX determinant(matrix)

' Liste (senza oggetti)
LISTA create
LISTA add lista, valore
LISTA remove lista, index

' Alberi
TREE create
TREE insert tree, nodo, valore

' Grafi
GRAPH create
GRAPH add_edge graph, nodo1, nodo2
```

### 7.2 Strutture di Controllo

```basic
' SELECT/CASE (analogo a Switch)
SELECT nome_variabile
    CASE valore1
        ' codice
    CASE valore2
        ' codice
    DEFAULT
        ' codice
END SELECT

' IF inline
risultato = IF(condizione, valore_se_true, valore_se_false)
```

### 7.3 Procedure

```basic
DEFPROC nome_procedura(param1, param2)
    ' codice
ENDPROC

' Chiamata
PROC nome_procedura(arg1, arg2)
```

### 7.4 Comandi Grafici

```basic
' Risoluzione
SCREEN 0  ' 640x360
SCREEN 1  ' 960x540
SCREEN 2  ' 1920x1080

' Colori
COLOR 15, 0        ' foreground, background
PEN 3              ' spessore linea

' Primitive
PSET 100, 100, 7
LINE 0, 0, 200, 200, 5
RECT 10, 10, 100, 50, 3
CIRCLE 100, 100, 50, 2

' Poligono regolare
POLYGON centro_x, centro_y, lati, raggio, colore

' GRID
GRID cols, rows, width, height, color, thickness

' Immagini
IMAGE "file.png", x, y
```

### 7.5 Comandi 3D

```basic
' Camera
CAMERA x, y, z
LOOKAT x, y, z

' Rendering
3DMODE WIRE       ' wireframe
3DMODE FLAT       ' flat shading
3DMODE WIREFLAT   ' combinato

' Mesh
MESH load, "model.rwm"
MESH wirecolor, r, g, b, alpha
MESH wiridthickness, n
MESH facecolor r, g, b

' Parent/Pivot
PARENT mesh_child, mesh_parent
PIVOT mesh, x, y, z

' Collisioni
IF COLLISION(sprite1, sprite2) THEN ...
IF COLLISION_3D(mesh1, mesh2) THEN ...
```

### 7.6 Comandi Audio

```basic
' Strumenti
INSTRUMENT "piano"     ' carica strumento
NOTE "C4", 1000        ' suona nota (ms)

' Sintesi
SYNTH osc, tipo, frequenza
SYNTH filter, tipo, cutoff
SYNTH env, attack, decay, sustain, release

' Track
TRACK 1, "nome"
NOTE 1, "C4", 500
PLAYTRACK
PAUSETRACK
STOPTRACK

' Samples
SAMPLE load, "drum.wav"
SAMPLE play

' TTS
SAY "Ciao mondo"
SAYLANG "en", "Hello world"
VOICE gender, "female"  ' o "male"

' Export
EXPORT audio, "canzone.wav"
```

### 7.7 Matematica

```basic
' Trigonometria precalcolata
x = SIN(angolo)    ' angolo in gradi (0.01° precisione)
y = COS(angolo)
z = TAN(angolo)

' Matrici
DIM m(3, 3)
MATRIX fill m, 1, 0, 0, 0, 1, 0, 0, 0, 1
d = MATRIX det(m)
```

---

## 8. Utilities e Sistema

### 8.1 GPIO

```basic
' Configurazione
GPIO mode 17, OUTPUT
GPIO mode 27, INPUT

' I/O
GPIO write 17, 1
val = GPIO read 27

' PWM
GPIO pwm 18, 50  ' 50% duty cycle
```

### 8.2 File

| Formato | Supporto |
|---------|----------|
| BMP | Leggi/Mostra |
| JPG | Leggi/Mostra |
| PNG | Leggi/Mostra |
| WAV | Leggi/Scrivi |
| MP3 | Leggi |
| SF2 | Caricamento lazy |

### 8.3 Screenshots

```basic
SCREENSHOT             ' screenshot modo grafico
SCREENSHOT "nome.png"  ' con nome file

' Modo testo
TEXTMODE screenshot
```

### 8.4 Video Grab

```basic
VIDEO start
VIDEO stop
VIDEO save "video.mp4"

' Con audio
VIDEO startmic
VIDEO stopmic
```

---

## 9. Funzionalità da Implementare

### 9.1 Editor

- [ ] Sprite Editor
- [ ] Poliedri Editor
- [ ] Tracker (text mode, accessibile)

### 9.2 Comandi Missing

- [ ] Screenshot (testo e grafico)
- [ ] Header row TTS
- [ ] IMAGE SHOW a coordinate specifiche
- [ ] Collisioni sprites (bounding box)
- [ ] Collisioni 3D

### 9.3 TTS

- [ ] Coda automatica (no sovrapposizioni)
- [ ] Lingua mista in linea (es: italiano + cinese)

### 9.4 Audio

- [ ] Lazy loading soundfont
- [ ] Modifica singoli sample
- [ ] Sample info (campioni, qualità, canali)

### 9.5 Altro

- [ ] F1 → Manuale multilingua
- [ ] Selezione caratteri Unicode non-europei
- [ ] Supporto stampanti

---

## 10. Stack di Rete

```
 priorità bassa
      │
  ┌───┴───┐
  │  SSH  │ ← (opzionale)
  ├───┬───┤
  │HTTP │ ← browser text-only
  ├───┬───┤
  │ IRC │ ← chat
  ├───┬───┤
  │ FTP │ ← file transfer
  ├───┬───┤
  │EMAIL│ ← client
  ├───┬───┤
  │MESH│ ← encrypted P2P
  └───┴───┘
```

---

## 11. Glossario

| Termine | Definizione |
|---------|-------------|
| **Bare-metal** | Senza sistema operativo |
| **Monotasking** | Un solo programma alla volta |
| **TTS** | Text-to-Speech |
| **SF2** | Soundfont 2.0 |
| **WIREFLAT** | Modalità 3D wireframe + flat |
| **GRID** | Superficie mesh A×B |
| **Mesh** | Rete peer-to-peer |
| **Lazy load** | Caricamento su richiesta |

---

## 12. Riferimenti

- Colori C64 compatibili (16 colori base)
- Palette estese (16 colori moderni)
- Unicode UTF-16 per caratteri
- PCM 16-bit 44.1kHz per audio
- TCP/IP per networking (ECCETTO mesh)

---

*Documento generato per RetroWave-4M*
*Ultimo aggiornamento: 2026-03-03*
