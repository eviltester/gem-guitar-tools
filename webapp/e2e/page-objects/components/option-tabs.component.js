export class OptionTabs {
  constructor(page) {
    this.page = page;
  }

  async openDisplay() {
    await this.page.locator("#tab-display").click();
  }

  async openTuning() {
    await this.page.locator("#tab-tuning").click();
  }

  async openTextPrint() {
    await this.page.locator("#tab-textprint").click();
  }

  async openProfile() {
    await this.page.locator("#tab-profile").click();
  }
}

