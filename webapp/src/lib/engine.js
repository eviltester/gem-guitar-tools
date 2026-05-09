function mod12(n) { return ((n % 12) + 12) % 12; }

export function transposeScale(scale, key) {
  const out = new Array(12).fill(0);
  for (let i = 0; i < 12; i++) {
    if (scale.notes[i]) out[mod12(i + (key - 7))] = 1;
  }
  return out;
}

export function computeFretboard(state) {
  const tuning = state.tunings[state.currentTuningIndex];
  const activeScale = state.scales[state.currentScaleIndex];
  const currentNotes = transposeScale(activeScale, state.key);
  const relativeRaw = activeScale.notes;
  const rows = [];

  for (let s = 0; s < 6; s++) {
    const row = [];
    for (let fret = 0; fret < state.display.maxFrets; fret++) {
      const pitchClass = mod12(tuning.notes[s] + fret);
      const octave = tuning.octaves[s] + Math.floor((tuning.notes[s] + fret) / 12);
      const isRoot = pitchClass === state.key;
      const inScale = !!currentNotes[pitchClass];
      const relativeIndex = mod12(pitchClass - state.key + 7);
      const isRelative = !!relativeRaw[relativeIndex] && state.display.showRelative;
      row.push({ string: s + 1, fret, pitchClass, octave, isRoot, inScale, isRelative });
    }
    rows.push(row);
  }

  return rows;
}

export function toggleScaleNoteByPitch(state, pitchClass) {
  const scale = state.scales[state.currentScaleIndex];
  const idx = mod12(pitchClass - state.key + 7);
  scale.notes[idx] = scale.notes[idx] ? 0 : 1;
}

export function noteLabel(pitchClass, names, aliases) {
  const primary = names[pitchClass];
  return aliases[primary] ? `${primary}/${aliases[primary]}` : primary;
}

export function scaleNoteNames(scale, key, names) {
  const notes = transposeScale(scale, key);
  return notes.map((v, i) => (v ? names[i] : null)).filter(Boolean);
}
