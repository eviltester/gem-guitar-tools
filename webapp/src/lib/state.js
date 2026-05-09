import { NOTE_NAMES, NOTE_ALIASES, DEFAULT_SCALE_BANK, BUILT_IN_TUNINGS } from "./constants.js";

/** @typedef {{name:string, notes:number[]}} Scale */
/** @typedef {{name:string, notes:number[], octaves:number[]}} Tuning */
/** @typedef {{showRoot:boolean,showRelative:boolean,showNumbers:boolean,showScaleName:boolean,showKeyName:boolean,noteShape:"circle"|"square"|"text",firstFret:number,fretIncrement:number,maxFrets:number}} DisplayOptions */
/** @typedef {{channel:number,velocity:number,noteLengthMs:number,midiEnabled:boolean,midiNoteOff:boolean}} MidiConfig */
/** @typedef {{header1:string,header2:string,trailer1:string,trailer2:string,startFret:number,noFrets:number,tab:number}} TextPrintConfig */

export function blankNotes() { return new Array(12).fill(0); }

export function defaultState() {
  return {
    noteNames: NOTE_NAMES,
    noteAliases: NOTE_ALIASES,
    key: 7,
    root: 7,
    scales: structuredClone(DEFAULT_SCALE_BANK),
    currentScaleIndex: 0,
    tunings: structuredClone(BUILT_IN_TUNINGS),
    currentTuningIndex: 0,
    display: {
      showRoot: true,
      showRelative: false,
      showNumbers: true,
      showScaleName: true,
      showKeyName: true,
      noteShape: "circle",
      sharpFlatEmphasis: "none",
      noteEmphasis: "none",
      rootEmphasis: "none",
      fretNumbering: "none",
      fretCountMode: "24",
      customFrets: 24,
      openStringFretNumber: false,
      openStringNut: false,
      firstFret: 5,
      fretIncrement: 5,
      maxFrets: 24
    },
    midi: {
      channel: 0,
      velocity: 100,
      noteLengthMs: 250,
      midiEnabled: true,
      midiNoteOff: true
    },
    tuningPlaySeconds: 15,
    textPrint: {
      header1: "Scale: &name in &key",
      header2: "Notes: &notes",
      trailer1: "&symbols",
      trailer2: "",
      startFret: 0,
      noFrets: 12,
      tab: 4,
      showAs: "symbol",
      fretNumbers: "none",
      fretCountMode: "24",
      customFrets: 24,
      openStringFretNumber: false,
      openStringNut: false
    }
  };
}
