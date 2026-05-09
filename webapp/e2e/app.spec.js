import { test, expect } from "@playwright/test";

test("renders and previews text", async ({ page }) => {
  await page.goto("/");
  await expect(page.getByRole("heading", { name: "Guitar Reference" })).toBeVisible();
  await page.getByRole("button", { name: "Preview Text" }).click();
  await expect(page.locator("#text-preview")).toContainText("Scale:");
});

test("store as adds a scale", async ({ page }) => {
  await page.goto("/");
  page.once("dialog", (d) => d.accept("E2E Scale"));
  await page.getByRole("button", { name: "Store As" }).click();
  await expect(page.locator("#scale-select")).toContainText("E2E Scale");
});
