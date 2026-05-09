import { expect, test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("clicking a non-scale fret note adds it, updates scale notes, and switches name to custom", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Ionian mode ( mode 1 )");

  const beforeNotes = await app.scaleNotes.getActiveNotes();
  const beforeCount = await app.fretboardDisplay.getScaleFretCount();
  await app.fretboardDisplay.clickFirstNonScaleFret();

  await app.keyScaleSelector.expectSelectedScaleName("custom");
  const afterNotes = await app.scaleNotes.getActiveNotes();
  expect(afterNotes.length).toBe(beforeNotes.length + 1);
  const afterCount = await app.fretboardDisplay.getScaleFretCount();
  expect(afterCount).toBeGreaterThan(beforeCount);
});

test("clicking a scale fret note removes it, updates scale notes, and switches name to custom", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Dorian mode ( mode 2 )");

  const beforeNotes = await app.scaleNotes.getActiveNotes();
  const beforeCount = await app.fretboardDisplay.getScaleFretCount();
  await app.fretboardDisplay.clickFirstScaleFret();

  await app.keyScaleSelector.expectSelectedScaleName("custom");
  const afterNotes = await app.scaleNotes.getActiveNotes();
  expect(afterNotes.length).toBe(beforeNotes.length - 1);
  const afterCount = await app.fretboardDisplay.getScaleFretCount();
  expect(afterCount).toBeLessThan(beforeCount);
});

test("clicking a root note removes it from scale notes and root is no longer rendered", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Ionian mode ( mode 1 )");
  await app.fretboardDisplay.expectAnyRootVisible(true);

  const beforeNotes = await app.scaleNotes.getActiveNotes();
  expect(beforeNotes).toContain("E");

  await app.fretboardDisplay.clickFirstRootFret();

  await app.keyScaleSelector.expectSelectedScaleName("custom");
  const afterNotes = await app.scaleNotes.getActiveNotes();
  expect(afterNotes).not.toContain("E");
  await app.fretboardDisplay.expectAnyRootVisible(false);
});
