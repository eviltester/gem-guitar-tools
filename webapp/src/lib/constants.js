export const NOTE_NAMES = ["A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"];
export const NOTE_ALIASES = { "A#": "Bb", "C#": "Db", "D#": "Eb", "F#": "Gb", "G#": "Ab" };

export const DEFAULT_SCALE_BANK = [
  { name: "pentatonic blues scale", notes: [1,0,1,0,0,1,0,1,0,0,1,0], builtIn: true },
  { name: "Double Harmonic Oriental (mode 5)", notes: [1,1,0,0,1,1,0,1,1,0,0,1], builtIn: true },
  { name: "Harmonic Minor Spanish Gypsy Scale (mode 5)", notes: [1,0,1,1,0,1,0,1,1,0,0,1], builtIn: true },
  { name: "Pentatonic Scale - Indian", notes: [1,0,1,0,0,1,0,1,0,0,0,1], builtIn: true }
];

export const DEFAULT_TUNING = {
  name: "Standard E Tuning",
  notes: [7, 2, 10, 5, 0, 7],
  octaves: [3, 3, 2, 2, 2, 1],
  builtIn: true
};

export const FRET_MARKERS = [3, 5, 7, 9, 12, 15, 17, 19, 21, 24];
