import { test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("select Drop B updates tuning strings and fretboard rendering", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openTuning();
  await app.tuningTab.expectVisible();

  await app.tuningTab.selectTuningByName("Drop B");
  await app.tuningTab.expectStringDefinitions(
    [2, 11, 7, 2, 9, 4],
    [3, 3, 2, 2, 2, 1]
  );

  await app.fretboardDisplay.expectOpenStringPitchClasses([2, 11, 7, 2, 9, 4]);
});

test("select Lute updates tuning strings and fretboard rendering", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openTuning();
  await app.tuningTab.expectVisible();

  await app.tuningTab.selectTuningByName("Lute");
  await app.tuningTab.expectStringDefinitions(
    [5, 0, 7, 0, 4, 7],
    [3, 3, 2, 2, 2, 1]
  );

  await app.fretboardDisplay.expectOpenStringPitchClasses([5, 0, 7, 0, 4, 7]);
});

