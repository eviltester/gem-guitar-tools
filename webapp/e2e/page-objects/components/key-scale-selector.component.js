import { expect } from "@playwright/test";

export class KeyScaleSelector {
  constructor(page) {
    this.page = page;
  }

  async selectScaleByName(scaleName) {
    await this.page.locator("#scale-select").selectOption({ label: scaleName });
  }

  async selectKeyByName(keyName) {
    await this.page.locator("#key-select").selectOption({ label: keyName });
  }

  async expectSelectedKeyName(keyName) {
    await expect(this.page.locator("#key-select option:checked")).toHaveText(keyName);
  }

  async expectScaleDropdownContains(text) {
    await expect(this.page.locator("#scale-select")).toContainText(text);
  }

  async clickCopyScale() {
    await this.page.getByRole("button", { name: "Copy" }).click();
  }

  async expectCopyEnabled() {
    await expect(this.page.locator("#new-scale")).toBeEnabled();
  }

  async expectRenameDisabled() {
    await expect(this.page.locator("#rename-scale")).toBeDisabled();
  }

  async expectDeleteDisabled() {
    await expect(this.page.locator("#delete-scale")).toBeDisabled();
  }

  async expectDeleteEnabled() {
    await expect(this.page.locator("#delete-scale")).toBeEnabled();
  }

  async clickRenameScale() {
    await this.page.locator("#rename-scale").click();
  }

  async clickDeleteScale() {
    await this.page.locator("#delete-scale").click();
  }

  async expectSelectedScaleName(name) {
    await expect(this.page.locator("#scale-select option:checked")).toHaveText(name);
  }

  async getSelectedScaleName() {
    return (await this.page.locator("#scale-select option:checked").textContent())?.trim() || "";
  }
}
