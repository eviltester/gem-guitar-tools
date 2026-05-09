import { expect } from "@playwright/test";

export class TuningTab {
  constructor(page) {
    this.page = page;
  }

  async expectVisible() {
    await expect(this.page.locator("#tuning-select")).toBeVisible();
  }

  async selectTuningByName(tuningName) {
    await this.page.locator("#tuning-select").selectOption({ label: tuningName });
  }

  async expectStringDefinitions(expectedNotes, expectedOctaves) {
    const noteSelects = this.page.locator(".tune-note");
    const octInputs = this.page.locator(".tune-oct");
    for (let i = 0; i < expectedNotes.length; i++) {
      await expect(noteSelects.nth(i)).toHaveValue(String(expectedNotes[i]));
      await expect(octInputs.nth(i)).toHaveValue(String(expectedOctaves[i]));
    }
  }

  async expectCopyEnabled() {
    await expect(this.page.locator("#copy-tuning")).toBeEnabled();
  }

  async expectRenameDisabled() {
    await expect(this.page.locator("#rename-tuning")).toBeDisabled();
  }

  async expectDeleteDisabled() {
    await expect(this.page.locator("#delete-tuning")).toBeDisabled();
  }

  async expectRenameEnabled() {
    await expect(this.page.locator("#rename-tuning")).toBeEnabled();
  }

  async expectDeleteEnabled() {
    await expect(this.page.locator("#delete-tuning")).toBeEnabled();
  }

  async copyCurrentTuning() {
    await this.page.locator("#copy-tuning").click();
  }

  async renameCurrentTuning(newName) {
    this.page.once("dialog", (d) => d.accept(newName));
    await this.page.locator("#rename-tuning").click();
  }

  async deleteCurrentTuning() {
    await this.page.locator("#delete-tuning").click();
  }

  async setStringNote(indexZeroBased, noteValue) {
    await this.page.locator(".tune-note").nth(indexZeroBased).selectOption(String(noteValue));
  }

  async getCurrentStringDefinitions() {
    const notes = [];
    const octaves = [];
    const noteSelects = this.page.locator(".tune-note");
    const octInputs = this.page.locator(".tune-oct");
    const count = await noteSelects.count();
    for (let i = 0; i < count; i++) {
      notes.push(Number(await noteSelects.nth(i).inputValue()));
      octaves.push(Number(await octInputs.nth(i).inputValue()));
    }
    return { notes, octaves };
  }

  async expectSelectedTuningName(name) {
    await expect(this.page.locator("#tuning-select option:checked")).toHaveText(name);
  }

  async getSelectedTuningName() {
    return (await this.page.locator("#tuning-select option:checked").textContent())?.trim() || "";
  }
}

