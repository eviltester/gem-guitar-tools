import { test, expect } from "@playwright/test";

test("renders and shows text print preview", async ({ page }) => {
  await page.goto("/");
  await expect(page.getByRole("heading", { name: "Guitar Reference" })).toBeVisible();
  await page.locator("#tab-textprint").click();
  await expect(page.locator("#text-preview")).toContainText("Scale:");
});

test("copy adds a scale after creating custom", async ({ page }) => {
  await page.goto("/");
  // Built-ins are protected; first toggle a note to create/select custom.
  await page.locator(".note-toggle").first().click();
  await page.getByRole("button", { name: "Copy" }).click();
  await expect(page.locator("#scale-select")).toContainText("(copy)");
});
