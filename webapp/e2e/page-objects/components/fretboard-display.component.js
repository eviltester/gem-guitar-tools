import { expect } from "@playwright/test";

export class FretboardDisplay {
  constructor(page) {
    this.page = page;
  }

  async expectSymbolNotesVisible() {
    await expect(this.page.locator(".fret span", { hasText: "o" }).first()).toBeVisible();
  }

  async expectTextNotesVisible() {
    await expect(this.page.locator(".fret .note-single, .fret .note-dual").first()).toBeVisible();
  }

  async expectFretNumberVisible(numberText) {
    await expect(this.page.locator(".fret-num-row .fret-num-cell", { hasText: numberText }).first()).toBeVisible();
  }

  async expectNutDividerVisible() {
    await expect(this.page.locator(".fret.nut-divider").first()).toBeVisible();
  }

  async expectNutDividerEnabled(enabled) {
    const dividers = this.page.locator(".fret.nut-divider");
    if (enabled) {
      await expect(dividers.first()).toBeVisible();
    } else {
      await expect(dividers).toHaveCount(0);
    }
  }

  async expectFretColumns(maxFrets) {
    await expect(this.page.locator(".string-row").nth(1).locator(".fret")).toHaveCount(maxFrets);
  }

  async expectTopFretLabels(expectedLabels) {
    const cells = this.page.locator(".fret-num-row").first().locator(".fret-num-cell");
    const texts = (await cells.allTextContents()).map((t) => t.trim());
    expect(texts).toEqual(expectedLabels.map((x) => (x === null ? "" : String(x))));
  }

  async expectAnyEmphasis(expected) {
    const count = await this.page.locator(".fret .emphasis").count();
    if (expected) expect(count).toBeGreaterThan(0);
    else expect(count).toBe(0);
  }

  async expectOpenStringPitchClasses(expectedPitchClasses) {
    const rows = this.page.locator(".string-row");
    for (let i = 0; i < expectedPitchClasses.length; i++) {
      const openFret = rows.nth(i + 1).locator(".fret").first();
      await expect(openFret).toHaveAttribute("data-fret", "0");
      await expect(openFret).toHaveAttribute("data-pitch", String(expectedPitchClasses[i]));
    }
  }

  async clickFirstNonScaleFret() {
    await this.page.locator(".fret.off").first().click();
  }

  async clickFirstScaleFret() {
    await this.page.locator(".fret.in").first().click();
  }

  async clickFirstRootFret() {
    await this.page.locator(".fret.root").first().click();
  }

  async expectAnyRootVisible(visible) {
    const roots = this.page.locator(".fret.root");
    if (visible) {
      await expect(roots.first()).toBeVisible();
    } else {
      await expect(roots).toHaveCount(0);
    }
  }

  async expectScaleFretCountDelta(beforeCount, delta) {
    const after = await this.page.locator(".fret.in, .fret.root").count();
    expect(after).toBe(beforeCount + delta);
  }

  async getScaleFretCount() {
    return this.page.locator(".fret.in, .fret.root").count();
  }
}
