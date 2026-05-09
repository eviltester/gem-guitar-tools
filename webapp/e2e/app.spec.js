import { test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
  test.info().annotations.push({ type: "app", description: "ready" });
});

test("display options update fretboard rendering and retain on tab switch", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openDisplay();
  await app.displayOptionTab.selectShowNotesAsSymbol();
  await app.fretboardDisplay.expectSymbolNotesVisible();

  await app.displayOptionTab.selectShowNotesAsText();
  await app.fretboardDisplay.expectTextNotesVisible();

  await app.displayOptionTab.selectFretNumbersAll();
  await app.fretboardDisplay.expectFretNumberVisible("1");

  await app.displayOptionTab.enableOpenStringNut();
  await app.fretboardDisplay.expectNutDividerVisible();

  await app.optionTabs.openTuning();
  await app.tuningTab.expectVisible();
  await app.optionTabs.openDisplay();

  await app.displayOptionTab.expectShowNotesAsTextSelected();
  await app.displayOptionTab.expectFretNumbersAllSelected();
  await app.displayOptionTab.expectOpenStringNutSelected();
  await app.fretboardDisplay.expectTextNotesVisible();
});

test("text print options reflect immediately in preview and details stay open", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openTextPrint();
  await app.textPrintTab.expectPreviewContains("Scale:");

  await app.textPrintTab.openOptionsDetails();
  await app.textPrintTab.expectOptionsDetailsOpen();

  await app.textPrintTab.selectShowNotesAsText();
  await app.textPrintTab.expectPreviewNotContains(". root, o scale");

  await app.textPrintTab.selectFretNumbersAll();
  await app.textPrintTab.enableOpenStringFretNumber();
  await app.textPrintTab.expectPreviewContains("  0  ");
  await app.textPrintTab.expectOptionsDetailsOpen();

  await app.textPrintTab.selectFretCount5();
  await app.textPrintTab.expectPreviewContains("S1 ");
  await app.textPrintTab.expectPreviewContains("S6 ");
});

test("profile tab actions show feedback and delete enablement changes", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.optionTabs.openProfile();
  await app.profileTab.expectDeleteDisabled();

  await app.profileTab.clickSaveProfile();
  await app.profileTab.expectSavedMessageVisible();
  await app.profileTab.expectDeleteEnabled();

  await app.profileTab.clickDeleteProfile();
  await app.profileTab.expectDeletedMessageVisible();
  await app.profileTab.expectDeleteDisabled();
});

test("custom scale copy flow still works", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.scaleNotes.clickFirstScaleNoteToggle();
  await app.keyScaleSelector.clickCopyScale();
  await app.keyScaleSelector.expectScaleDropdownContains("(copy)");
});
