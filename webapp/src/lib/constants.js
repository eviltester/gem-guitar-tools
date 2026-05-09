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

function builtInScaleFromNotes(name, notes) {
  return { name, notes: [...notes], builtIn: true };
}

function dedupeByName(items) {
  const seen = new Set();
  return items.filter((item) => {
    const key = item.name.toLowerCase();
    if (seen.has(key)) return false;
    seen.add(key);
    return true;
  });
}

function scoreNameQuality(name) {
  let score = 0;
  const lower = name.toLowerCase();
  if (!lower.includes("default")) score += 6;
  if (!lower.includes("mode")) score += 4;
  if (!lower.includes("scale")) score += 2;
  if (!lower.includes(" - ")) score += 2;
  if (!/[()]/.test(name)) score += 2;
  if (!/[#/]/.test(name)) score += 1;
  if (/^[A-Z][A-Za-z0-9#+/\s-]*$/.test(name)) score += 2;
  if (/^(major|minor|harmonic|melodic|dorian|phrygian|lydian|mixolydian|aeolian|locrian|pentatonic|blues|chromatic|whole tone|diminished|augmented|enigmatic|persian|arabic|iwato|kumoi|hirajoshi|pelog|prometheus|neapolitan)/i.test(name)) score += 3;
  score -= Math.floor(name.length / 24);
  return score;
}

function dedupeBySignature(items, signatureFn) {
  const bestBySignature = new Map();
  for (const item of items) {
    const signature = signatureFn(item);
    const current = bestBySignature.get(signature);
    if (!current) {
      bestBySignature.set(signature, item);
      continue;
    }
    const currentScore = scoreNameQuality(current.name);
    const itemScore = scoreNameQuality(item.name);
    if (itemScore > currentScore || (itemScore === currentScore && item.name.length < current.name.length)) {
      bestBySignature.set(signature, item);
    }
  }
  return [...bestBySignature.values()];
}

const BASE_SCALES = [
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

const LEGACY_SCALE_ALIASES = [
  builtInScale("Major Ionian mode ( mode 1 )", [0, 2, 4, 5, 7, 9, 11]),
  builtInScale("Major Dorian mode ( mode 2 )", [0, 2, 3, 5, 7, 9, 10]),
  builtInScale("Major Phrygian mode ( mode 3 )", [0, 1, 3, 5, 7, 8, 10]),
  builtInScale("Major Lydian mode ( mode 4 )", [0, 2, 4, 6, 7, 9, 11]),
  builtInScale("Major Mixolydian mode ( mode 5 )", [0, 2, 4, 5, 7, 9, 10]),
  builtInScale("Major Aeolian mode ( mode 6 )", [0, 2, 3, 5, 7, 8, 10]),
  builtInScale("Major Locrian mode ( mode 7 )", [0, 1, 3, 5, 6, 8, 10]),
  builtInScale("Melodic Minor (mode 1)", [0, 2, 3, 5, 7, 9, 11]),
  builtInScale("Melodic Minor Phrygian #6 (Java) (mode 2)", [0, 1, 3, 5, 7, 9, 10]),
  builtInScale("Melodic Minor Lydian augmented (mode 3)", [0, 2, 4, 6, 8, 9, 11]),
  builtInScale("Melodic Minor Overtone (mode 4)", [0, 2, 4, 6, 7, 9, 10]),
  builtInScale("Melodic Minor Hindustan (mode 5)", [0, 2, 4, 5, 7, 8, 10]),
  builtInScale("Melodic Minor Locrian #2 (mode 6)", [0, 2, 3, 5, 6, 8, 10]),
  builtInScale("Melodic Minor Superlocrian (mode 7)", [0, 1, 3, 4, 6, 8, 10]),
  builtInScale("Harmonic Minor (mode 1)", [0, 2, 3, 5, 7, 8, 11]),
  builtInScale("Harmonic Minor (mode 2)", [0, 1, 3, 5, 6, 9, 10]),
  builtInScale("Harmonic Minor (mode 3)", [0, 2, 4, 5, 8, 9, 11]),
  builtInScale("Melodic Minor - Romanian (mode 4)", [0, 2, 3, 6, 7, 9, 10]),
  builtInScale("Harmonic Minor - Spanish Gypsy Scale (mode 5)", [0, 1, 4, 5, 7, 8, 10]),
  builtInScale("Harmonic Minor (mode 6)", [0, 3, 4, 6, 7, 9, 11]),
  builtInScale("Harmonic Minor (mode 7)", [0, 1, 3, 4, 6, 8, 9]),
  builtInScale("Double Harmonic Byzantine (mode 1)", [0, 1, 4, 5, 7, 8, 11]),
  builtInScale("Double Harmonic (mode 2)", [0, 3, 4, 6, 7, 10, 11]),
  builtInScale("Double Harmonic (mode 3)", [0, 1, 4, 5, 8, 9, 11]),
  builtInScale("Double Harmonic - Hungarian (mode 4)", [0, 2, 3, 6, 7, 8, 11]),
  builtInScale("Double Harmonic - Oriental (mode 5)", [0, 1, 4, 5, 6, 8, 10]),
  builtInScale("Double Harmonic (mode 6)", [0, 3, 4, 5, 7, 9, 10]),
  builtInScale("Double Harmonic (mode 7)", [0, 1, 2, 4, 6, 8, 9]),
  builtInScale("Diatonic Pentatonic Major (mode 1)", [0, 2, 4, 7, 9]),
  builtInScale("Diatonic Pentatonic - Egyptian (mode 2)", [0, 2, 5, 7, 10]),
  builtInScale("Diatonic Pentatonic (mode 3)", [0, 3, 5, 8, 10]),
  builtInScale("Diatonic Pentatonic - Ritusen Japan (mode 4)", [0, 2, 5, 7, 9]),
  builtInScale("Diatonic Pentatonic Minor (mode 5)", [0, 3, 5, 7, 10]),
  builtInScale("Pentatonic Scale - Scriabin", [0, 1, 4, 7, 9]),
  builtInScale("Pentatonic - Pelog (Java)", [0, 1, 3, 7, 8]),
  builtInScale("Pentatonic Scale - Kumoi (Japan)", [0, 2, 3, 7, 9]),
  builtInScale("Pentatonic Scale - Hirajoshi (Japan)", [0, 2, 3, 7, 8]),
  builtInScale("Pentatonic Scale - Iwato (Japan)", [0, 1, 5, 6, 10]),
  builtInScale("Pentatonic Scale - Indian", [0, 1, 5, 7, 10]),
  builtInScale("Hexatonic Scale - Blues Scale", [0, 3, 5, 6, 7, 10]),
  builtInScale("Hexatonic Scale - Prometheus", [0, 2, 4, 6, 9, 10]),
  builtInScale("Hexatonic Scale - Prometheus Neapolitan", [0, 1, 4, 6, 9, 10]),
  builtInScale("Symmetrical Scale - Whole Tone", [0, 2, 4, 6, 8, 10]),
  builtInScale("Symmetrical Scale - Diminished", [0, 1, 3, 4, 6, 7, 9, 10]),
  builtInScale("Symmetrical Scale - Augmented", [0, 3, 4, 7, 8, 11]),
  builtInScale("Symmetrical Scale - Chromatic", [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11]),
  builtInScale("Hungarian Major Scale", [0, 3, 4, 6, 7, 9, 10]),
  builtInScale("Enigmatic Scale", [0, 1, 4, 6, 8, 10, 11]),
  builtInScale("Eight-tone Spanish Scale", [0, 1, 3, 4, 5, 6, 8, 10]),
  builtInScale("Neapolitan Scale", [0, 1, 3, 5, 7, 9, 11]),
  builtInScale("Lydian Minor Scale", [0, 2, 4, 6, 7, 8, 10]),
  builtInScale("Major Locrian Scale - Arabian", [0, 2, 4, 5, 6, 8, 10]),
  builtInScale("Neapolitan Minor Scale", [0, 1, 3, 5, 7, 8, 11]),
  builtInScale("Todi - Indian Raga Scale", [0, 1, 3, 6, 7, 8, 11]),
  builtInScale("Marve - Indian Raga Scale", [0, 1, 4, 6, 7, 9, 11]),
  builtInScale("Persian Scale", [0, 1, 4, 5, 6, 8, 11]),
  builtInScale("Pentatonic Blues Scale", [0, 3, 5, 6, 7, 10]),
  builtInScaleFromNotes("DEFAULT SCALE - pentatonic blues scale", buildScaleNotes([0, 3, 5, 6, 7, 10])),
  builtInScaleFromNotes("DEFAULT SCALE - pentatonic blues scale num 2", buildScaleNotes([0, 3, 5, 6, 7, 10]))
];

const SCALE_SIGNATURE = (scale) => scale.notes.join(",");
export const DEFAULT_SCALE_BANK = dedupeByName(
  dedupeBySignature([...BASE_SCALES, ...LEGACY_SCALE_ALIASES], SCALE_SIGNATURE)
);

function builtInTuning(name, notes, octaves = [3, 3, 2, 2, 2, 1]) {
  return { name, notes, octaves, builtIn: true };
}

const BASE_TUNINGS = [
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

const LEGACY_TUNING_ALIASES = [
  builtInTuning("A9", [0, 7, 0, 2, 4, 7]),
  builtInTuning("Standard", [7, 2, 10, 5, 0, 7]),
  builtInTuning("open A", [7, 0, 7, 0, 4, 7]),
  builtInTuning("A major 7th", [7, 0, 8, 4, 0, 7]),
  builtInTuning("C 6th", [3, 0, 3, 10, 3, 7]),
  builtInTuning("C Sus 4", [3, 10, 3, 10, 3, 8]),
  builtInTuning("Dmaj7", [5, 0, 5, 9, 4, 5]),
  builtInTuning("D modal", [5, 0, 5, 10, 0, 5]),
  builtInTuning("Lute", [5, 0, 7, 0, 4, 7]),
  builtInTuning("Esus2", [7, 2, 7, 9, 2, 7]),
  builtInTuning("Emsus7", [7, 2, 5, 10, 2, 7]),
  builtInTuning("E no 5", [7, 11, 4, 8, 11, 4]),
  builtInTuning("Open F", [3, 8, 3, 8, 0, 3]),
  builtInTuning("Wahine Slack", [5, 10, 5, 10, 2, 5]),
  builtInTuning("Open F/C", [3, 8, 3, 8, 0, 3]),
  builtInTuning("Open Gb/Db", [4, 9, 4, 9, 1, 4]),
  builtInTuning("Gb/Db add 4", [4, 9, 4, 9, 11, 4]),
  builtInTuning("Gmaj Pentatonic", [5, 10, 5, 10, 2, 5]),
  builtInTuning("Cmaj Pentatonic", [3, 10, 3, 10, 3, 7]),
  builtInTuning("Open G/D", [5, 10, 5, 10, 2, 5]),
  builtInTuning("G6/D", [5, 10, 5, 10, 2, 4]),
  builtInTuning("Gmaj7/Double Slack", [5, 10, 5, 10, 1, 5]),
  builtInTuning("Gsus4", [5, 10, 5, 10, 0, 5])
];

const TUNING_SIGNATURE = (tuning) => `${tuning.notes.join(",")}|${(tuning.octaves || []).join(",")}`;
export const BUILT_IN_TUNINGS = dedupeByName(
  dedupeBySignature([...BASE_TUNINGS, ...LEGACY_TUNING_ALIASES], TUNING_SIGNATURE)
);

export const FRET_MARKERS = [3, 5, 7, 9, 12, 15, 17, 19, 21, 24];
