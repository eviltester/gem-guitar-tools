import { expect } from "@playwright/test";

export class ProfileTab {
  constructor(page) {
    this.page = page;
  }

  async expectDeleteDisabled() {
    await expect(this.page.locator("#delete-profile")).toBeDisabled();
  }

  async expectDeleteEnabled() {
    await expect(this.page.locator("#delete-profile")).toBeEnabled();
  }

  async clickSaveProfile() {
    await this.page.locator("#save-profile").click();
  }

  async clickDeleteProfile() {
    await this.page.locator("#delete-profile").click();
  }

  async clickDefaults() {
    await this.page.locator("#reset-defaults").click();
  }

  async expectSavedMessageVisible() {
    await expect(this.page.getByText("saved to local storage")).toBeVisible();
  }

  async expectDeletedMessageVisible() {
    await expect(this.page.getByText("profile deleted from local storage")).toBeVisible();
  }

  async expectDefaultsMessageVisible() {
    await expect(this.page.getByText("defaults loaded in memory")).toBeVisible();
  }
}
