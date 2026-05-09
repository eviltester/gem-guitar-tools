import { expect } from "@playwright/test";

export class ScaleNotes {
  constructor(page) {
    this.page = page;
  }

  async clickFirstScaleNoteToggle() {
    await this.page.locator(".note-toggle").first().click();
  }

  async expectScaleNameAndKeyText(scaleName, keyName) {
    const summary = this.page
      .locator("section.panel")
      .filter({ has: this.page.getByRole("heading", { name: "Scale Notes" }) })
      .locator("p");
    await expect(summary).toHaveText(`${scaleName} in ${keyName}`);
  }

  async expectActiveNotesEqual(expectedNoteLabels) {
    const active = this.page.locator(".notes .note-toggle.on");
    const actualLabels = (await active.allTextContents()).map((s) => s.trim());
    expect(actualLabels).toEqual(expectedNoteLabels);
  }

  async toggleFirstScaleNote() {
    await this.page.locator(".note-toggle").first().click();
  }

  async getActiveNotes() {
    const active = this.page.locator(".notes .note-toggle.on");
    return (await active.allTextContents()).map((s) => s.trim());
  }
}

