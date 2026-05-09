import { expect, test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("built-in scale: copy enabled, rename/delete disabled", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Ionian mode ( mode 1 )");
  await app.keyScaleSelector.expectCopyEnabled();
  await app.keyScaleSelector.expectRenameDisabled();
  await app.keyScaleSelector.expectDeleteDisabled();
});

test("editing a built-in scale switches to custom and custom can be deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Ionian mode ( mode 1 )");
  await app.scaleNotes.toggleFirstScaleNote();
  await app.keyScaleSelector.expectSelectedScaleName("custom");
  await app.keyScaleSelector.expectDeleteEnabled();
  await app.keyScaleSelector.clickDeleteScale();
  await expect(page.locator("#scale-select option", { hasText: "custom" })).toHaveCount(0);
});

test("edited built-in custom scale can be renamed and deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectScaleByName("Major Dorian mode ( mode 2 )");
  await app.scaleNotes.toggleFirstScaleNote();
  await app.keyScaleSelector.expectSelectedScaleName("custom");
  await app.renameCurrentScale("custom renamed");
  await app.keyScaleSelector.expectSelectedScaleName("custom renamed");
  await app.keyScaleSelector.expectDeleteEnabled();
  await app.keyScaleSelector.clickDeleteScale();
  await expect(page.locator("#scale-select option", { hasText: "custom renamed" })).toHaveCount(0);
});

test("copy built-in scale preserves notes and copy can be deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const sourceScale = "Major Phrygian mode ( mode 3 )";
  await app.keyScaleSelector.selectScaleByName(sourceScale);
  const sourceNotes = await app.scaleNotes.getActiveNotes();
  await app.keyScaleSelector.clickCopyScale();

  const copyName = await app.keyScaleSelector.getSelectedScaleName();
  expect(copyName).toMatch(/\(copy/);
  const copyNotes = await app.scaleNotes.getActiveNotes();
  expect(copyNotes).toEqual(sourceNotes);

  await app.keyScaleSelector.selectScaleByName(sourceScale);
  expect(await app.scaleNotes.getActiveNotes()).toEqual(sourceNotes);
  await app.keyScaleSelector.selectScaleByName(copyName);
  expect(await app.scaleNotes.getActiveNotes()).toEqual(sourceNotes);

  await app.keyScaleSelector.clickDeleteScale();
  await expect(page.locator("#scale-select option", { hasText: copyName })).toHaveCount(0);
});

