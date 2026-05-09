import { expect } from "@playwright/test";

export class DisplayOptionTab {
  constructor(page) {
    this.page = page;
  }

  async selectShowNotesAsSymbol() {
    await this.page.locator("input[name='fret-show-mode'][value='symbol']").check();
  }

  async selectShowNotesAsText() {
    await this.page.locator("input[name='fret-show-mode'][value='text']").check();
  }

  async selectFretNumbersAll() {
    await this.page.locator("input[name='fret-numbering'][value='all']").check();
  }

  async enableOpenStringNut() {
    await this.page.locator("#open-string-nut").check();
  }

  async expectShowNotesAsTextSelected() {
    await expect(this.page.locator("input[name='fret-show-mode'][value='text']")).toBeChecked();
  }

  async expectFretNumbersAllSelected() {
    await expect(this.page.locator("input[name='fret-numbering'][value='all']")).toBeChecked();
  }

  async expectOpenStringNutSelected() {
    await expect(this.page.locator("#open-string-nut")).toBeChecked();
  }

  async selectSharpFlatEmphasis(mode) {
    await this.page.locator(`input[name='sharp-flat-emphasis'][value='${mode}']`).check();
  }

  async selectNoteEmphasis(mode) {
    await this.page.locator(`input[name='note-emphasis'][value='${mode}']`).check();
  }

  async selectRootEmphasis(mode) {
    await this.page.locator(`input[name='root-emphasis'][value='${mode}']`).check();
  }

  async selectFretNumbering(mode) {
    await this.page.locator(`input[name='fret-numbering'][value='${mode}']`).check();
  }

  async selectFretCount(value) {
    if (["5", "7", "12", "24"].includes(String(value))) {
      await this.page.locator(`input[name='fret-count-mode'][value='${value}']`).check();
      return;
    }
    await this.page.locator("input[name='fret-count-mode'][value='custom']").check();
    await this.page.locator("#custom-frets").fill(String(value));
    await this.page.locator("#custom-frets").blur();
  }

  async setOpenStringOptions(mode) {
    const fret = this.page.locator("#open-string-fret-number");
    const nut = this.page.locator("#open-string-nut");
    const wantFret = mode === "fret" || mode === "fret+nut";
    const wantNut = mode === "nut" || mode === "fret+nut";
    if ((await fret.isChecked()) !== wantFret) await fret.click();
    if ((await nut.isChecked()) !== wantNut) await nut.click();
  }

  async applyDisplayConfig(config) {
    if (config.showNotesAs === "note-names") await this.selectShowNotesAsText();
    else await this.selectShowNotesAsSymbol();
    await this.selectSharpFlatEmphasis(config.sharpFlat);
    await this.selectNoteEmphasis(config.note);
    await this.selectRootEmphasis(config.root);
    await this.selectFretNumbering(config.fretNumbers);
    await this.selectFretCount(config.numberOfFrets);
    await this.setOpenStringOptions(config.openString);
  }
}

