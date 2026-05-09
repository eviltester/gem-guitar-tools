import { expect, test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
  await app.optionTabs.openTuning();
  await app.tuningTab.expectVisible();
});

test("built-in tuning: copy enabled, rename/delete disabled", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.tuningTab.selectTuningByName("Drop B");
  await app.tuningTab.expectCopyEnabled();
  await app.tuningTab.expectRenameDisabled();
  await app.tuningTab.expectDeleteDisabled();
});

test("editing a built-in tuning switches to custom and custom can be deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.tuningTab.selectTuningByName("Drop B");
  await app.tuningTab.setStringNote(0, 3);
  await app.tuningTab.expectSelectedTuningName("custom");
  await app.tuningTab.expectDeleteEnabled();
  await app.tuningTab.deleteCurrentTuning();
  await expect(page.locator("#tuning-select option", { hasText: "custom" })).toHaveCount(0);
});

test("edited built-in custom tuning can be renamed and deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.tuningTab.selectTuningByName("Lute");
  await app.tuningTab.setStringNote(0, 6);
  await app.tuningTab.expectSelectedTuningName("custom");
  await app.tuningTab.renameCurrentTuning("custom renamed");
  await app.tuningTab.expectSelectedTuningName("custom renamed");
  await app.tuningTab.expectDeleteEnabled();
  await app.tuningTab.deleteCurrentTuning();
  await expect(page.locator("#tuning-select option", { hasText: "custom renamed" })).toHaveCount(0);
});

test("copy built-in tuning preserves string definitions and copy can be deleted", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  const sourceTuning = "Drop B";
  await app.tuningTab.selectTuningByName(sourceTuning);
  const sourceDefs = await app.tuningTab.getCurrentStringDefinitions();

  await app.tuningTab.copyCurrentTuning();
  const copyName = await app.tuningTab.getSelectedTuningName();
  expect(copyName).toMatch(/\(copy/);
  expect(await app.tuningTab.getCurrentStringDefinitions()).toEqual(sourceDefs);

  await app.tuningTab.selectTuningByName(sourceTuning);
  expect(await app.tuningTab.getCurrentStringDefinitions()).toEqual(sourceDefs);
  await app.tuningTab.selectTuningByName(copyName);
  expect(await app.tuningTab.getCurrentStringDefinitions()).toEqual(sourceDefs);

  await app.tuningTab.deleteCurrentTuning();
  await expect(page.locator("#tuning-select option", { hasText: copyName })).toHaveCount(0);
});

