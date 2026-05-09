import { test } from "@playwright/test";
import { NOTE_NAMES, DEFAULT_SCALE_BANK } from "../src/lib/constants.js";
import { transposeScale } from "../src/lib/engine.js";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

function expectedScaleNotesFor(scaleName, keyName) {
  const scale = DEFAULT_SCALE_BANK.find((s) => s.name === scaleName);
  if (!scale) throw new Error(`Scale not found in DEFAULT_SCALE_BANK: ${scaleName}`);
  const keyIndex = NOTE_NAMES.indexOf(keyName);
  if (keyIndex < 0) throw new Error(`Key not found in NOTE_NAMES: ${keyName}`);
  const transposed = transposeScale(scale, keyIndex);
  return transposed.map((v, i) => (v ? NOTE_NAMES[i] : null)).filter(Boolean);
}

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("selecting a scale updates scale-name text and active scale notes", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const scaleName = "Major Ionian mode ( mode 1 )";
  const keyName = "E";

  await app.keyScaleSelector.selectScaleByName(scaleName);
  await app.scaleNotes.expectScaleNameAndKeyText(scaleName, keyName);
  await app.scaleNotes.expectActiveNotesEqual(expectedScaleNotesFor(scaleName, keyName));
});

test("selecting key + scale updates scale-name text and active scale notes", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const scaleName = "Pentatonic Blues Scale";
  const keyName = "C#";

  await app.keyScaleSelector.selectScaleByName(scaleName);
  await app.keyScaleSelector.selectKeyByName(keyName);
  await app.scaleNotes.expectScaleNameAndKeyText(scaleName, keyName);
  await app.scaleNotes.expectActiveNotesEqual(expectedScaleNotesFor(scaleName, keyName));
});

test("data-driven key/scale combinations show expected names and notes", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const combos = [
    { key: "A", scale: "Major Ionian mode ( mode 1 )" },
    { key: "F#", scale: "Major Dorian mode ( mode 2 )" },
    { key: "C", scale: "Symmetrical Scale - Whole Tone" },
    { key: "D#", scale: "Pentatonic Scale - Kumoi (Japan)" },
    { key: "G", scale: "Pentatonic Blues Scale" }
  ];

  for (const combo of combos) {
    await app.keyScaleSelector.selectScaleByName(combo.scale);
    await app.keyScaleSelector.selectKeyByName(combo.key);
    await app.scaleNotes.expectScaleNameAndKeyText(combo.scale, combo.key);
    await app.scaleNotes.expectActiveNotesEqual(expectedScaleNotesFor(combo.scale, combo.key));
  }
});

test("clicking key root removes it from scale notes and root is no longer rendered on fretboard", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const scaleName = "Major Ionian mode ( mode 1 )";
  const keyName = "E";

  await app.keyScaleSelector.selectScaleByName(scaleName);
  await app.keyScaleSelector.selectKeyByName(keyName);
  await app.fretboardDisplay.expectAnyRootVisible(true);

  const beforeNotes = await app.scaleNotes.getActiveNotes();
  if (!beforeNotes.includes(keyName)) throw new Error(`Expected ${keyName} in active notes before root toggle`);

  await app.fretboardDisplay.clickFirstRootFret();

  const afterNotes = await app.scaleNotes.getActiveNotes();
  if (afterNotes.includes(keyName)) throw new Error(`Expected ${keyName} to be removed from active notes after root toggle`);
  await app.fretboardDisplay.expectAnyRootVisible(false);
});
