import { expect, test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

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

function centerPad(value, width = 5) {
  const text = String(value ?? "");
  if (!text) return " ".repeat(width);
  if (text.length >= width) return text.slice(0, width);
  const total = width - text.length;
  const left = Math.floor(total / 2);
  const right = total - left;
  return `${" ".repeat(left)}${text}${" ".repeat(right)}`;
}

function expectedNumberLine(numberOfFrets, fretNumbersMode, openStringMode) {
  const maxFrets = Number(numberOfFrets) + 1;
  const mode = toFretNumberingValue(fretNumbersMode);
  const showOpen = openStringMode === "fret" || openStringMode === "fret+nut";
  let line = "   ";
  for (let fret = 0; fret < maxFrets; fret++) {
    const show = fret === 0 ? showOpen : shouldShowFretNumber(fret, mode);
    line += centerPad(show ? fret : "");
    if ((openStringMode === "nut" || openStringMode === "fret+nut") && fret === 0) line += "|";
  }
  return line;
}

const rows = [
  ["symbol", "none", "5", "fret"],
  ["symbol", "all", "7", "nut"],
  ["symbol", "5-7-12", "12", "fret+nut"],
  ["symbol", "3-5-7-12", "24", "none"],
  ["symbol", "1-3-5-7-12", "17", "fret"],
  ["note-names", "none", "7", "fret+nut"],
  ["note-names", "all", "5", "none"],
  ["note-names", "5-7-12", "24", "fret"],
  ["note-names", "3-5-7-12", "12", "nut"],
  ["note-names", "1-3-5-7-12", "5", "nut"],
  ["note-names", "none", "17", "nut"],
  ["symbol", "none", "12", "none"],
  ["symbol", "none", "24", "nut"],
  ["symbol", "all", "12", "fret"],
  ["symbol", "all", "24", "fret+nut"],
  ["symbol", "all", "17", "fret+nut"],
  ["symbol", "5-7-12", "5", "nut"],
  ["symbol", "5-7-12", "7", "none"],
  ["symbol", "5-7-12", "17", "none"],
  ["symbol", "3-5-7-12", "5", "fret+nut"],
  ["symbol", "3-5-7-12", "7", "fret"],
  ["symbol", "3-5-7-12", "17", "fret"],
  ["symbol", "1-3-5-7-12", "7", "fret+nut"],
  ["symbol", "1-3-5-7-12", "12", "none"],
  ["symbol", "1-3-5-7-12", "24", "fret"]
];

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
  await app.optionTabs.openTextPrint();
  await app.textPrintTab.openOptionsDetails();
  await app.textPrintTab.expectOptionsDetailsOpen();
});

test("text print options matrix renders expected output", async ({ page }) => {
  const app = new GuitarReferencePage(page);

  for (const [showNotesAs, fretNumbers, numberOfFrets, openString] of rows) {
    const config = {
      showNotesAs,
      fretNumbers: toFretNumberingValue(fretNumbers),
      numberOfFrets,
      openString
    };
    await app.textPrintTab.applyPrintConfig(config);
    const preview = await app.textPrintTab.getPreviewText();

    expect(preview).toContain("Scale:");
    expect(preview).toContain("S1 ");
    expect(preview).toContain("S6 ");
    if (fretNumbers !== "none") {
      expect(preview).toContain(expectedNumberLine(numberOfFrets, fretNumbers, openString));
    }
    if (showNotesAs === "symbol") expect(preview).toContain(". root, o scale");
    else expect(preview).not.toContain(". root, o scale");
  }
});

test("copy to clipboard shows confirmation label", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.textPrintTab.mockClipboardWriteSuccess();
  await app.textPrintTab.clickCopyToClipboard();
  await app.textPrintTab.expectCopiedNoticeVisible();
});

test("download text triggers file download", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const download = await app.textPrintTab.downloadText();
  expect(download.suggestedFilename()).toBe("scale.txt");
});
