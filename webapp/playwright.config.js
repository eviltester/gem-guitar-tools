import { defineConfig } from "@playwright/test";

export default defineConfig({
  testDir: "./e2e",
  use: { baseURL: "http://127.0.0.1:43173" },
  webServer: {
    command: "npm run build && npm run preview -- --host 127.0.0.1 --port 43173",
    port: 43173,
    reuseExistingServer: false
  }
});
