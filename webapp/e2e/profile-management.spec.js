import { test } from "@playwright/test";
import { GuitarReferencePage } from "./page-objects/guitar-reference.page.js";

test.beforeEach(async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.open();
});

test("profile tab button states and save/delete messages", async ({ page }) => {
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

test("defaults button resets in-memory options to defaults", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectKeyByName("C#");
  await app.keyScaleSelector.expectSelectedKeyName("C#");

  await app.optionTabs.openProfile();
  await app.profileTab.clickDefaults();
  await app.profileTab.expectDefaultsMessageVisible();

  await app.keyScaleSelector.expectSelectedKeyName("E");
});

test("save profile persists state across reload and delete profile clears persisted state", async ({ page }) => {
  const app = new GuitarReferencePage(page);
  await app.keyScaleSelector.selectKeyByName("G#");
  await app.optionTabs.openProfile();
  await app.profileTab.clickSaveProfile();
  await app.profileTab.expectSavedMessageVisible();

  await app.reload();
  await app.keyScaleSelector.expectSelectedKeyName("G#");

  await app.optionTabs.openProfile();
  await app.profileTab.clickDeleteProfile();
  await app.profileTab.expectDeletedMessageVisible();

  await app.reload();
  await app.keyScaleSelector.expectSelectedKeyName("E");
});

