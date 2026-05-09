export const NOTE_NAMES = ["A", "A#", "B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"];
export const NOTE_ALIASES = { "A#": "Bb", "C#": "Db", "D#": "Eb", "F#": "Gb", "G#": "Ab" };

const ROOT_E_INDEX = 7; // NOTE_NAMES index for E
function buildScaleNotes(intervals) {
  const notes = new Array(12).fill(0);
  for (const semis of intervals) {
    notes[(ROOT_E_INDEX + semis) % 12] = 1;
  }
  return notes;
}
function builtInScale(name, intervals) {
  return { name, notes: buildScaleNotes(intervals), builtIn: true };
}

export const DEFAULT_SCALE_BANK = [
  builtInScale("Major", [0, 2, 4, 5, 7, 9, 11]),
  builtInScale("Natural Minor", [0, 2, 3, 5, 7, 8, 10]),
  builtInScale("Harmonic Minor", [0, 2, 3, 5, 7, 8, 11]),
  builtInScale("Melodic Minor", [0, 2, 3, 5, 7, 9, 11]),
  builtInScale("Major Pentatonic", [0, 2, 4, 7, 9]),
  builtInScale("Minor Pentatonic", [0, 3, 5, 7, 10]),
  builtInScale("Major Blues", [0, 2, 3, 4, 7, 9]),
  builtInScale("Minor Blues", [0, 3, 5, 6, 7, 10]),
  builtInScale("Ionian", [0, 2, 4, 5, 7, 9, 11]),
  builtInScale("Dorian", [0, 2, 3, 5, 7, 9, 10]),
  builtInScale("Phrygian", [0, 1, 3, 5, 7, 8, 10]),
  builtInScale("Lydian", [0, 2, 4, 6, 7, 9, 11]),
  builtInScale("Mixolydian", [0, 2, 4, 5, 7, 9, 10]),
  builtInScale("Aeolian", [0, 2, 3, 5, 7, 8, 10]),
  builtInScale("Locrian", [0, 1, 3, 5, 6, 8, 10]),
  builtInScale("Phrygian Dominant", [0, 1, 4, 5, 7, 8, 10]),
  builtInScale("Lydian Dominant", [0, 2, 4, 6, 7, 9, 10]),
  builtInScale("Altered (Super Locrian)", [0, 1, 3, 4, 6, 8, 10]),
  builtInScale("Locrian #2", [0, 2, 3, 5, 6, 8, 10]),
  builtInScale("Melodic Minor b6", [0, 2, 3, 5, 7, 8, 11]),
  builtInScale("Harmonic Major", [0, 2, 4, 5, 7, 8, 11]),
  builtInScale("Double Harmonic", [0, 1, 4, 5, 7, 8, 11]),
  builtInScale("Hungarian Minor", [0, 2, 3, 6, 7, 8, 11]),
  builtInScale("Neapolitan Minor", [0, 1, 3, 5, 7, 8, 11]),
  builtInScale("Neapolitan Major", [0, 1, 3, 5, 7, 9, 11]),
  builtInScale("Persian", [0, 1, 4, 5, 6, 8, 11]),
  builtInScale("Arabic", [0, 2, 4, 5, 6, 8, 10]),
  builtInScale("Hirajoshi", [0, 2, 3, 7, 8]),
  builtInScale("In Sen", [0, 1, 5, 7, 10]),
  builtInScale("Iwato", [0, 1, 5, 6, 10]),
  builtInScale("Kumoi", [0, 2, 3, 7, 9]),
  builtInScale("Pelog", [0, 1, 3, 7, 8]),
  builtInScale("Prometheus", [0, 2, 4, 6, 9, 10]),
  builtInScale("Enigmatic", [0, 1, 4, 6, 8, 10, 11]),
  builtInScale("Whole Tone", [0, 2, 4, 6, 8, 10]),
  builtInScale("Diminished (Whole-Half)", [0, 2, 3, 5, 6, 8, 9, 11]),
  builtInScale("Diminished (Half-Whole)", [0, 1, 3, 4, 6, 7, 9, 10]),
  builtInScale("Augmented", [0, 3, 4, 7, 8, 11]),
  builtInScale("Major Bebop", [0, 2, 4, 5, 7, 8, 9, 11]),
  builtInScale("Dominant Bebop", [0, 2, 4, 5, 7, 9, 10, 11]),
  builtInScale("Minor Bebop", [0, 2, 3, 5, 7, 8, 10, 11]),
  builtInScale("Dorian Bebop", [0, 2, 3, 4, 5, 7, 9, 10]),
  builtInScale("Mixolydian b6", [0, 2, 4, 5, 7, 8, 10]),
  builtInScale("Major Locrian", [0, 2, 4, 5, 6, 8, 10]),
  builtInScale("Lydian Augmented", [0, 2, 4, 6, 8, 9, 11]),
  builtInScale("Ukrainian Dorian", [0, 2, 3, 6, 7, 9, 10]),
  builtInScale("Spanish 8 Tone", [0, 1, 3, 4, 5, 6, 8, 10]),
  builtInScale("Major Hexatonic", [0, 2, 4, 5, 7, 9]),
  builtInScale("Minor Hexatonic", [0, 2, 3, 5, 7, 10]),
  builtInScale("Chromatic", [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11])
];

function builtInTuning(name, notes, octaves = [3, 3, 2, 2, 2, 1]) {
  return { name, notes, octaves, builtIn: true };
}

export const BUILT_IN_TUNINGS = [
  builtInTuning("Standard E Tuning", [7, 2, 10, 5, 0, 7]),
  builtInTuning("Half-step Down", [6, 1, 9, 4, 11, 6]),
  builtInTuning("D Standard", [5, 0, 8, 3, 10, 5]),
  builtInTuning("Drop D", [5, 2, 10, 5, 0, 7]),
  builtInTuning("Drop C#", [4, 1, 9, 4, 11, 6]),
  builtInTuning("Drop C", [3, 0, 8, 3, 10, 5]),
  builtInTuning("Drop B", [2, 11, 7, 2, 9, 4]),
  builtInTuning("Open G", [5, 10, 5, 10, 2, 5]),
  builtInTuning("Open D", [5, 0, 5, 9, 0, 5]),
  builtInTuning("Open E", [7, 2, 7, 11, 2, 7]),
  builtInTuning("Open C", [3, 10, 3, 10, 3, 7]),
  builtInTuning("Open A", [7, 0, 7, 0, 4, 7]),
  builtInTuning("DADGAD", [5, 0, 5, 10, 0, 5]),
  builtInTuning("CGCGCE", [3, 10, 3, 10, 3, 7]),
  builtInTuning("C6 Variant", [3, 0, 3, 10, 3, 7]),
  builtInTuning("Nashville High-Strung", [7, 2, 10, 5, 0, 7], [4, 4, 3, 3, 2, 1]),
  builtInTuning("Double Drop D", [5, 2, 10, 5, 0, 5]),
  builtInTuning("Modal D (DADFAD)", [5, 0, 5, 9, 0, 5]),
  builtInTuning("New Standard (Fripp)", [3, 10, 5, 0, 7, 10]),
  builtInTuning("All Fourths", [7, 2, 9, 4, 11, 6])
];

export const FRET_MARKERS = [3, 5, 7, 9, 12, 15, 17, 19, 21, 24];
