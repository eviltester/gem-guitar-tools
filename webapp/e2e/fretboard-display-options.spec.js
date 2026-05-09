import { test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

function normalizeEmphasis(value) {
  return value === "no-emphasis" ? "no-emphasis" : value;
}

function toSharpFlatValue(value) {
  const v = normalizeEmphasis(value);
  return v === "no-emphasis" ? "none" : v;
}

function toFretNumberingValue(value) {
  if (value === "5-7-12") return "5_7_12";
  if (value === "3-5-7-12") return "3_5_7_12";
  if (value === "1-3-5-7-12") return "1_3_5_7_12";
  return value;
}

function shouldShowFretNumber(fret, mode) {
  if (mode === "none") return false;
  if (mode === "all") return true;
  if (mode === "5_7_12") return fret > 0 && (fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  if (mode === "3_5_7_12") return fret > 0 && (fret % 12 === 3 || fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  if (mode === "1_3_5_7_12") return fret > 0 && (fret % 12 === 1 || fret % 12 === 3 || fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  return false;
}

function expectedTopLabels(numberOfFrets, fretNumbersMode, openStringMode) {
  const maxFrets = Number(numberOfFrets) + 1;
  const mode = toFretNumberingValue(fretNumbersMode);
  const showOpen = openStringMode === "fret" || openStringMode === "fret+nut";
  const labels = [];
  for (let fret = 0; fret < maxFrets; fret++) {
    const show = fret === 0 ? showOpen : shouldShowFretNumber(fret, mode);
    labels.push(show ? fret : null);
  }
  return labels;
}

const rows = [
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "none", "5", "fret"],
  ["symbol", "sharp", "emphasis", "emphasis", "all", "7", "nut"],
  ["symbol", "flat", "no-emphasis", "emphasis", "5-7-12", "12", "fret+nut"],
  ["note-names", "no-emphasis", "emphasis", "no-emphasis", "5-7-12", "24", "none"],
  ["note-names", "sharp", "no-emphasis", "no-emphasis", "3-5-7-12", "17", "nut"],
  ["note-names", "flat", "emphasis", "no-emphasis", "1-3-5-7-12", "5", "fret+nut"],
  ["note-names", "no-emphasis", "no-emphasis", "emphasis", "all", "7", "fret"],
  ["symbol", "no-emphasis", "emphasis", "emphasis", "3-5-7-12", "17", "none"],
  ["symbol", "no-emphasis", "no-emphasis", "emphasis", "1-3-5-7-12", "24", "nut"],
  ["note-names", "sharp", "emphasis", "emphasis", "none", "12", "fret"],
  ["symbol", "sharp", "no-emphasis", "no-emphasis", "5-7-12", "5", "none"],
  ["symbol", "sharp", "no-emphasis", "no-emphasis", "1-3-5-7-12", "7", "fret+nut"],
  ["symbol", "flat", "no-emphasis", "no-emphasis", "none", "7", "none"],
  ["symbol", "flat", "no-emphasis", "no-emphasis", "all", "12", "nut"],
  ["symbol", "flat", "no-emphasis", "no-emphasis", "3-5-7-12", "24", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "3-5-7-12", "12", "fret+nut"],
  ["symbol", "sharp", "no-emphasis", "no-emphasis", "none", "24", "fret+nut"],
  ["symbol", "flat", "no-emphasis", "no-emphasis", "none", "17", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "emphasis", "all", "5", "nut"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "all", "24", "fret+nut"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "all", "17", "fret+nut"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "5-7-12", "7", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "5-7-12", "17", "nut"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "3-5-7-12", "5", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "3-5-7-12", "7", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "1-3-5-7-12", "12", "none"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "1-3-5-7-12", "17", "fret"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "none", "5", "nut"],
  ["symbol", "no-emphasis", "no-emphasis", "no-emphasis", "all", "5", "none"]
];

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("fretboard display options matrix renders correctly", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openDisplay();

  for (const [showNotesAs, sharpFlat, note, root, fretNumbers, numberOfFrets, openString] of rows) {
    const config = {
      showNotesAs,
      sharpFlat: toSharpFlatValue(sharpFlat),
      note: normalizeEmphasis(note) === "emphasis" ? "emphasis" : "none",
      root: normalizeEmphasis(root) === "emphasis" ? "emphasis" : "none",
      fretNumbers: toFretNumberingValue(fretNumbers),
      numberOfFrets,
      openString
    };

    await app.displayOptionTab.applyDisplayConfig(config);

    const maxFrets = Number(numberOfFrets) + 1;
    await app.fretboardDisplay.expectFretColumns(maxFrets);
    await app.fretboardDisplay.expectTopFretLabels(
      expectedTopLabels(numberOfFrets, fretNumbers, openString)
    );
    await app.fretboardDisplay.expectNutDividerEnabled(openString === "nut" || openString === "fret+nut");

    if (showNotesAs === "note-names") {
      await app.fretboardDisplay.expectTextNotesVisible();
    } else {
      await app.fretboardDisplay.expectSymbolNotesVisible();
    }

    const expectEmphasis = showNotesAs === "note-names"
      && (config.sharpFlat !== "none" || config.note === "emphasis" || config.root === "emphasis");
    await app.fretboardDisplay.expectAnyEmphasis(expectEmphasis);
  }
});
