import { expect } from "@playwright/test";

export class TextPrintTab {
  constructor(page) {
    this.page = page;
  }

  async expectPreviewContains(text) {
    await expect(this.page.locator("#text-preview")).toContainText(text);
  }

  async expectPreviewNotContains(text) {
    await expect(this.page.locator("#text-preview")).not.toContainText(text);
  }

  async openOptionsDetails() {
    await this.page.locator("#text-print-options summary").click();
  }

  async expectOptionsDetailsOpen() {
    await expect(this.page.locator("#text-print-options")).toHaveAttribute("open", "");
  }

  async selectShowNotesAsText() {
    await this.page.locator("input[name='print-show-mode'][value='text']").check();
  }

  async selectShowNotesAsSymbol() {
    await this.page.locator("input[name='print-show-mode'][value='symbol']").check();
  }

  async selectFretNumbersAll() {
    await this.page.locator("input[name='print-fret-numbering'][value='all']").check();
  }

  async selectFretNumbers(mode) {
    await this.page.locator(`input[name='print-fret-numbering'][value='${mode}']`).check();
  }

  async enableOpenStringFretNumber() {
    await this.page.locator("#print-open-string-fret-number").check();
  }

  async disableOpenStringFretNumber() {
    const el = this.page.locator("#print-open-string-fret-number");
    if (await el.isChecked()) await el.click();
  }

  async enableOpenStringNut() {
    const el = this.page.locator("#print-open-string-nut");
    if (!(await el.isChecked())) await el.click();
  }

  async disableOpenStringNut() {
    const el = this.page.locator("#print-open-string-nut");
    if (await el.isChecked()) await el.click();
  }

  async selectFretCount5() {
    await this.page.locator("input[name='print-fret-count-mode'][value='5']").check();
  }

  async selectFretCount(value) {
    if (["5", "7", "12", "24"].includes(String(value))) {
      await this.page.locator(`input[name='print-fret-count-mode'][value='${value}']`).check();
      return;
    }
    await this.page.locator("input[name='print-fret-count-mode'][value='custom']").check();
    await this.page.locator("#print-custom-frets").fill(String(value));
    await this.page.locator("#print-custom-frets").blur();
  }

  async setOpenStringOptions(mode) {
    const wantFret = mode === "fret" || mode === "fret+nut";
    const wantNut = mode === "nut" || mode === "fret+nut";
    if (wantFret) await this.enableOpenStringFretNumber();
    else await this.disableOpenStringFretNumber();
    if (wantNut) await this.enableOpenStringNut();
    else await this.disableOpenStringNut();
  }

  async applyPrintConfig(config) {
    if (config.showNotesAs === "note-names") await this.selectShowNotesAsText();
    else await this.selectShowNotesAsSymbol();
    await this.selectFretNumbers(config.fretNumbers);
    await this.selectFretCount(config.numberOfFrets);
    await this.setOpenStringOptions(config.openString);
  }

  async getPreviewText() {
    return (await this.page.locator("#text-preview").textContent()) || "";
  }

  async clickCopyToClipboard() {
    await this.page.locator("#copy-text").click();
  }

  async expectCopiedNoticeVisible() {
    await expect(this.page.locator("span.save-notice.visible", { hasText: "copied to clipboard" })).toBeVisible();
  }

  async mockClipboardWriteSuccess() {
    await this.page.evaluate(() => {
      Object.defineProperty(navigator, "clipboard", {
        configurable: true,
        value: { writeText: async () => {} }
      });
    });
  }

  async downloadText() {
    const downloadPromise = this.page.waitForEvent("download");
    await this.page.locator("#download-text").click();
    return downloadPromise;
  }
}
