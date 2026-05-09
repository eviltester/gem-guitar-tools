import { expect } from "@playwright/test";
import { KeyScaleSelector } from "./components/key-scale-selector.component.js";
import { ScaleNotes } from "./components/scale-notes.component.js";
import { FretboardDisplay } from "./components/fretboard-display.component.js";
import { OptionTabs } from "./components/option-tabs.component.js";
import { DisplayOptionTab } from "./components/display-option-tab.component.js";
import { TuningTab } from "./components/tuning-tab.component.js";
import { TextPrintTab } from "./components/text-print-tab.component.js";
import { ProfileTab } from "./components/profile-tab.component.js";

export class GuitarReferencePage {
  constructor(page) {
    this.page = page;
    this.keyScaleSelector = new KeyScaleSelector(page);
    this.scaleNotes = new ScaleNotes(page);
    this.fretboardDisplay = new FretboardDisplay(page);
    this.optionTabs = new OptionTabs(page);
    this.displayOptionTab = new DisplayOptionTab(page);
    this.tuningTab = new TuningTab(page);
    this.textPrintTab = new TextPrintTab(page);
    this.profileTab = new ProfileTab(page);
  }

  async open() {
    await this.page.goto("/");
    await this.page.evaluate(() => localStorage.clear());
    await this.page.reload();
    await expect(this.page.getByRole("heading", { name: "Guitar Reference" })).toBeVisible();
  }

  async openWithoutReset() {
    await this.page.goto("/");
    await expect(this.page.getByRole("heading", { name: "Guitar Reference" })).toBeVisible();
  }

  async reload() {
    await this.page.reload();
    await expect(this.page.getByRole("heading", { name: "Guitar Reference" })).toBeVisible();
  }

  async renameCurrentScale(newName) {
    this.page.once("dialog", (d) => d.accept(newName));
    await this.keyScaleSelector.clickRenameScale();
  }
}
