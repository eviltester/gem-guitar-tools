import "./style.css";
import "tippy.js/dist/tippy.css";
import tippy from "tippy.js";
import { defaultState } from "./lib/state.js";
import { computeFretboard, toggleScaleNoteByPitch, noteLabel, scaleNoteNames } from "./lib/engine.js";
import { saveState, loadState, downloadText, hasSavedState, deleteState } from "./lib/storage.js";
import { playManagedNote, stopCurrentNote } from "./lib/audio.js";

function buildPersistedProfile(appState) {
  return {
    key: appState.key,
    root: appState.root,
    currentScaleIndex: appState.currentScaleIndex,
    currentTuningIndex: appState.currentTuningIndex,
    scales: appState.scales.filter((s) => !s.builtIn).map((s) => ({
      name: s.name,
      notes: [...s.notes],
      builtIn: false
    })),
    tunings: appState.tunings.filter((t) => !t.builtIn).map((t) => ({
      name: t.name,
      notes: [...t.notes],
      octaves: [...t.octaves],
      builtIn: false
    })),
    display: { ...appState.display },
    midi: { ...appState.midi },
    textPrint: { ...appState.textPrint },
    tuningPlaySeconds: appState.tuningPlaySeconds
  };
}

function hydrateState() {
  const defaults = defaultState();
  const persisted = loadState();
  if (!persisted) return defaults;

  const customScales = Array.isArray(persisted.scales)
    ? persisted.scales.filter((s) => !s.builtIn).map((s) => ({ ...s, builtIn: false }))
    : [];
  const customTunings = Array.isArray(persisted.tunings)
    ? persisted.tunings.filter((t) => !t.builtIn).map((t) => ({ ...t, builtIn: false }))
    : [];

  const mergedScales = [...defaults.scales, ...customScales];
  const mergedTunings = [...defaults.tunings, ...customTunings];

  return {
    ...defaults,
    ...persisted,
    scales: mergedScales,
    tunings: mergedTunings,
    currentScaleIndex: Math.min(Math.max(0, Number(persisted.currentScaleIndex) || 0), mergedScales.length - 1),
    currentTuningIndex: Math.min(Math.max(0, Number(persisted.currentTuningIndex) || 0), mergedTunings.length - 1)
  };
}

const state = hydrateState();
const app = document.querySelector("#app");
let activeStringIndex = null;
let displayOptionsOpen = false;
let textPrintOptionsOpen = false;
let saveNoticeUntil = 0;
let lowerTab = "display";
let copyNoticeUntil = 0;
let profileNoticeUntil = 0;
let profileNoticeText = "";

if (!state.tuningPlaySeconds || state.tuningPlaySeconds < 1) {
  state.tuningPlaySeconds = 15;
}
state.display.noteShape = state.display.noteShape === "text" ? "text" : "circle";
state.display.sharpFlatEmphasis = ["none", "sharp", "flat"].includes(state.display.sharpFlatEmphasis) ? state.display.sharpFlatEmphasis : "none";
state.display.noteEmphasis = ["none", "emphasis"].includes(state.display.noteEmphasis) ? state.display.noteEmphasis : "none";
state.display.rootEmphasis = ["none", "emphasis"].includes(state.display.rootEmphasis) ? state.display.rootEmphasis : "none";
state.display.fretNumbering = ["none", "all", "5_7_12", "3_5_7_12", "1_3_5_7_12"].includes(state.display.fretNumbering) ? state.display.fretNumbering : "none";
state.display.fretCountMode = ["5", "7", "12", "24", "custom"].includes(state.display.fretCountMode) ? state.display.fretCountMode : "24";
state.display.customFrets = Math.min(40, Math.max(1, Number(state.display.customFrets) || 24));
state.display.openStringFretNumber = !!state.display.openStringFretNumber;
state.display.openStringNut = !!state.display.openStringNut;
state.textPrint.showAs = state.textPrint.showAs === "text" ? "text" : "symbol";
state.textPrint.fretNumbers = ["none", "all", "5_7_12", "3_5_7_12", "1_3_5_7_12"].includes(state.textPrint.fretNumbers) ? state.textPrint.fretNumbers : "none";
state.textPrint.fretCountMode = ["5", "7", "12", "24", "custom"].includes(state.textPrint.fretCountMode) ? state.textPrint.fretCountMode : "24";
state.textPrint.customFrets = Math.min(40, Math.max(1, Number(state.textPrint.customFrets) || 24));
state.textPrint.openStringFretNumber = !!state.textPrint.openStringFretNumber;
state.textPrint.openStringNut = !!state.textPrint.openStringNut;

function stripDefaultScalePrefix(name) {
  return (name || "").replace(/^DEFAULT SCALE\s*-\s*/i, "").trim();
}

function normalizeScaleNames() {
  state.scales = state.scales.map((s) => ({ ...s, name: stripDefaultScalePrefix(s.name) }));
}

function stripDefaultTuningPrefix(name) {
  return (name || "").replace(/^DEFAULT:\s*/i, "").trim();
}

function normalizeTuningNames() {
  state.tunings = state.tunings.map((t) => ({ ...t, name: stripDefaultTuningPrefix(t.name) }));
  if (!state.tunings.some((t) => t.builtIn) && state.tunings.length > 0) {
    state.tunings[0].builtIn = true;
  }
}

function sortScalesKeepCurrent() {
  normalizeScaleNames();
  const currentName = state.scales[state.currentScaleIndex]?.name;
  state.scales.sort((a, b) => a.name.trim().localeCompare(b.name.trim()));
  state.currentScaleIndex = Math.max(
    0,
    state.scales.findIndex((s) => s.name === currentName)
  );
}

function makeUniqueCopyName(baseName, scales) {
  const names = new Set(scales.map((s) => s.name));
  const plainCopy = `${baseName} (copy)`;
  if (!names.has(plainCopy)) return plainCopy;
  let i = 1;
  while (names.has(`${baseName} (copy ${i})`)) i++;
  return `${baseName} (copy ${i})`;
}

function ensureEditableCurrentScale() {
  const current = state.scales[state.currentScaleIndex];
  if (!current?.builtIn) return;
  let customIndex = state.scales.findIndex((s) => !s.builtIn && s.name.toLowerCase() === "custom");
  if (customIndex === -1) {
    const customName = makeUniqueName("custom", state.scales);
    state.scales.push({ name: customName, notes: [...current.notes], builtIn: false });
    state.currentScaleIndex = state.scales.length - 1;
    sortScalesKeepCurrent();
    return;
  }
  state.scales[customIndex].notes = [...current.notes];
  state.currentScaleIndex = customIndex;
}

function makeUniqueName(baseName, items) {
  const names = new Set(items.map((x) => x.name));
  if (!names.has(baseName)) return baseName;
  let i = 1;
  while (names.has(`${baseName} ${i}`)) i++;
  return `${baseName} ${i}`;
}

function makeUniqueTuningCopyName(baseName, tunings) {
  const names = new Set(tunings.map((t) => t.name));
  const plainCopy = `${baseName} (copy)`;
  if (!names.has(plainCopy)) return plainCopy;
  let i = 1;
  while (names.has(`${baseName} (copy ${i})`)) i++;
  return `${baseName} (copy ${i})`;
}

function ensureEditableCurrentTuning() {
  const current = state.tunings[state.currentTuningIndex];
  if (!current?.builtIn) return;
  const customName = makeUniqueName("custom", state.tunings);
  const cloned = {
    name: customName,
    notes: [...current.notes],
    octaves: [...current.octaves],
    builtIn: false
  };
  state.tunings.push(cloned);
  state.currentTuningIndex = state.tunings.length - 1;
}

function optionsForNotes() {
  return state.noteNames.map((n, i) => `<option value="${i}" ${i===state.key?"selected":""}>${n}</option>`).join("");
}

function shouldShowFretNumber(fret, mode) {
  if (mode === "none") return false;
  if (mode === "all") return true;
  if (mode === "5_7_12") return fret > 0 && (fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  if (mode === "3_5_7_12") return fret > 0 && (fret % 12 === 3 || fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  if (mode === "1_3_5_7_12") return fret > 0 && (fret % 12 === 1 || fret % 12 === 3 || fret % 12 === 5 || fret % 12 === 7 || fret % 12 === 0);
  return false;
}

function fretNumbersRow(maxFrets, mode, showOpenStringFretNumber, showNut) {
  const cells = [];
  for (let fret = 0; fret < maxFrets; fret++) {
    const showLabel = fret === 0 ? showOpenStringFretNumber : shouldShowFretNumber(fret, mode);
    const label = showLabel ? fret : "";
    const nutClass = showNut && fret === 0 ? " nut-divider" : "";
    cells.push(`<button class="fret fret-num-cell${nutClass}" tabindex="-1" aria-hidden="true">${label}</button>`);
  }
  return `<div class="string-row fret-num-row"><span class="string-label"></span>${cells.join("")}</div>`;
}

function resolveVisibleFrets(display) {
  if (display.fretCountMode === "custom") {
    const fretsExcludingOpen = Math.min(40, Math.max(1, Number(display.customFrets) || 24));
    return fretsExcludingOpen + 1;
  }
  const fromMode = Number(display.fretCountMode);
  if ([5, 7, 12, 24].includes(fromMode)) return fromMode + 1;
  return 25;
}

function render() {
  const prevDisplay = app.querySelector("#display-options");
  const prevPrint = app.querySelector("#text-print-options");
  if (prevDisplay) displayOptionsOpen = prevDisplay.open;
  if (prevPrint) textPrintOptionsOpen = prevPrint.open;

  state.display.maxFrets = resolveVisibleFrets(state.display);
  normalizeTuningNames();
  const scale = state.scales[state.currentScaleIndex];
  const tuning = state.tunings[state.currentTuningIndex];
  const board = computeFretboard(state);

  app.innerHTML = `
  <main>
    <h1>Guitar Reference</h1>
    <div class="toolbar">
      <label>Key <select id="key-select">${optionsForNotes()}</select></label>
      <label>Scale <select id="scale-select">${state.scales.map((s,i)=>`<option value="${i}" ${i===state.currentScaleIndex?"selected":""}>${s.name}</option>`).join("")}</select></label>
      <button id="new-scale" ${scale?.builtIn ? "disabled" : ""}>Copy</button>
      <button id="rename-scale" ${scale?.builtIn ? "disabled" : ""}>Rename</button>
      <button id="delete-scale" ${scale?.builtIn ? "disabled" : ""}>Delete</button>
    </div>

    <section class="panel">
      <h2>Scale Notes</h2>
      <div class="notes">${state.noteNames.map((n,i)=>`<button class="note-toggle ${scaleNoteNames(scale,state.key,state.noteNames).includes(n)?"on":""}" data-note="${i}">${n}</button>`).join("")}</div>
      <p>${scale.name} in ${state.noteNames[state.key]}</p>
    </section>

    <section class="panel">
      <h2>Fretboard</h2>
      <div class="fretboard">
        ${fretNumbersRow(state.display.maxFrets, state.display.fretNumbering, state.display.openStringFretNumber, state.display.openStringNut)}
        ${board.map((row,ri)=>`<div class="string-row"><span class="string-label">S${ri+1}</span>${row.map((cell)=>{
          const cls = cell.isRoot ? "root" : cell.inScale ? "in" : "off";
          const rel = cell.isRelative ? " rel" : "";
          const nutClass = state.display.openStringNut && cell.fret === 0 ? " nut-divider" : "";
          const text = state.display.noteShape === "text"
            ? (cell.isRoot || cell.inScale ? noteLabelMarkup(cell, state) : "")
            : symbolMarkup(cell, state);
          const tooltip = fretTooltipContent(cell, state).replaceAll("\"", "&quot;");
          return `<button class="fret ${cls}${rel}${nutClass}" data-pitch="${cell.pitchClass}" data-string="${cell.string}" data-fret="${cell.fret}" data-tooltip="${tooltip}">${text}</button>`;
        }).join("")}</div>`).join("")}
        ${fretNumbersRow(state.display.maxFrets, state.display.fretNumbering, state.display.openStringFretNumber, state.display.openStringNut)}
      </div>
    </section>

    <section class="panel tabbed-panel">
      <div class="tabs" role="tablist" aria-label="Lower panels">
        <button class="tab-btn ${lowerTab === "display" ? "active" : ""}" id="tab-display" role="tab" aria-selected="${lowerTab === "display"}">Display Options</button>
        <button class="tab-btn ${lowerTab === "tuning" ? "active" : ""}" id="tab-tuning" role="tab" aria-selected="${lowerTab === "tuning"}">Tuning</button>
        <button class="tab-btn ${lowerTab === "textprint" ? "active" : ""}" id="tab-textprint" role="tab" aria-selected="${lowerTab === "textprint"}">Text Print</button>
        <button class="tab-btn ${lowerTab === "profile" ? "active" : ""}" id="tab-profile" role="tab" aria-selected="${lowerTab === "profile"}">Profile</button>
      </div>

      ${lowerTab === "display" ? `
      <div class="tab-content">
        <div class="option-row">
          <span class="option-label">show notes as:</span>
          <label><input type="radio" name="fret-show-mode" value="symbol" ${state.display.noteShape === "text" ? "" : "checked"} /> symbol</label>
          <label><input type="radio" name="fret-show-mode" value="text" ${state.display.noteShape === "text" ? "checked" : ""} /> note names</label>
        </div>
        <div class="option-row">
          <span class="option-label">sharp/flat:</span>
          <label><input type="radio" name="sharp-flat-emphasis" value="none" ${state.display.sharpFlatEmphasis === "none" ? "checked" : ""} /> no emphasis</label>
          <label><input type="radio" name="sharp-flat-emphasis" value="sharp" ${state.display.sharpFlatEmphasis === "sharp" ? "checked" : ""} /> sharp</label>
          <label><input type="radio" name="sharp-flat-emphasis" value="flat" ${state.display.sharpFlatEmphasis === "flat" ? "checked" : ""} /> flat</label>
        </div>
        <div class="option-row">
          <span class="option-label">note:</span>
          <label><input type="radio" name="note-emphasis" value="none" ${state.display.noteEmphasis === "none" ? "checked" : ""} /> no emphasis</label>
          <label><input type="radio" name="note-emphasis" value="emphasis" ${state.display.noteEmphasis === "emphasis" ? "checked" : ""} /> emphasis</label>
        </div>
        <div class="option-row">
          <span class="option-label">root:</span>
          <label><input type="radio" name="root-emphasis" value="none" ${state.display.rootEmphasis === "none" ? "checked" : ""} /> no emphasis</label>
          <label><input type="radio" name="root-emphasis" value="emphasis" ${state.display.rootEmphasis === "emphasis" ? "checked" : ""} /> emphasis</label>
        </div>
        <div class="option-row">
          <span class="option-label">fret numbers:</span>
          <label><input type="radio" name="fret-numbering" value="none" ${state.display.fretNumbering === "none" ? "checked" : ""} /> none</label>
          <label><input type="radio" name="fret-numbering" value="all" ${state.display.fretNumbering === "all" ? "checked" : ""} /> all</label>
          <label><input type="radio" name="fret-numbering" value="5_7_12" ${state.display.fretNumbering === "5_7_12" ? "checked" : ""} /> 5/7/12</label>
          <label><input type="radio" name="fret-numbering" value="3_5_7_12" ${state.display.fretNumbering === "3_5_7_12" ? "checked" : ""} /> 3/5/7/12</label>
          <label><input type="radio" name="fret-numbering" value="1_3_5_7_12" ${state.display.fretNumbering === "1_3_5_7_12" ? "checked" : ""} /> 1/3/5/7/12</label>
        </div>
        <div class="option-row">
          <span class="option-label">number of frets:</span>
          <label><input type="radio" name="fret-count-mode" value="5" ${state.display.fretCountMode === "5" ? "checked" : ""} /> 5</label>
          <label><input type="radio" name="fret-count-mode" value="7" ${state.display.fretCountMode === "7" ? "checked" : ""} /> 7</label>
          <label><input type="radio" name="fret-count-mode" value="12" ${state.display.fretCountMode === "12" ? "checked" : ""} /> 12</label>
          <label><input type="radio" name="fret-count-mode" value="24" ${state.display.fretCountMode === "24" ? "checked" : ""} /> 24</label>
          <label><input type="radio" name="fret-count-mode" value="custom" ${state.display.fretCountMode === "custom" ? "checked" : ""} /> custom</label>
          <input id="custom-frets" type="number" min="1" max="40" step="1" value="${state.display.customFrets}" ${state.display.fretCountMode === "custom" ? "" : "disabled"} />
        </div>
        <div class="option-row">
          <span class="option-label">open string:</span>
          <label><input type="checkbox" id="open-string-fret-number" ${state.display.openStringFretNumber ? "checked" : ""} /> fret#</label>
          <label><input type="checkbox" id="open-string-nut" ${state.display.openStringNut ? "checked" : ""} /> nut</label>
        </div>
      </div>
      ` : lowerTab === "tuning" ? `
      <div class="tab-content tuning">
      <div>
        <label>Tuning Preset <select id="tuning-select">${state.tunings.map((t,i)=>`<option value="${i}" ${i===state.currentTuningIndex?"selected":""}>${t.name}</option>`).join("")}</select></label>
        <button id="copy-tuning">Copy</button>
        <button id="rename-tuning" ${tuning?.builtIn ? "disabled" : ""}>Rename</button>
        <button id="delete-tuning" ${tuning?.builtIn ? "disabled" : ""}>Delete</button>
      </div>
      <div>
        <label>Duration (s) <input id="play-seconds" type="number" min="1" step="1" value="${state.tuningPlaySeconds}" /></label>
      </div>
      ${tuning.notes.map((note, i) => `<div>String ${i+1}: <select class="tune-note" data-index="${i}">${state.noteNames.map((n,ni)=>`<option value="${ni}" ${ni===note?"selected":""}>${n}</option>`).join("")}</select>
      Oct <input class="tune-oct" data-index="${i}" type="number" min="-3" max="5" value="${tuning.octaves[i]}" />
      <button class="play-string" data-index="${i}">${activeStringIndex === i ? "Stop" : "Play"}</button></div>`).join("")}
      </div>
      ` : lowerTab === "textprint" ? `
      <div class="tab-content">
      <details class="fret-options" id="text-print-options" ${textPrintOptionsOpen ? "open" : ""}>
        <summary>Text Print Options</summary>
        <div class="option-row">
          <span class="option-label">show notes as:</span>
          <label><input type="radio" name="print-show-mode" value="symbol" ${state.textPrint.showAs === "symbol" ? "checked" : ""} /> symbol</label>
          <label><input type="radio" name="print-show-mode" value="text" ${state.textPrint.showAs === "text" ? "checked" : ""} /> note names</label>
        </div>
        <div class="option-row">
          <span class="option-label">fret numbers:</span>
          <label><input type="radio" name="print-fret-numbering" value="none" ${state.textPrint.fretNumbers === "none" ? "checked" : ""} /> none</label>
          <label><input type="radio" name="print-fret-numbering" value="all" ${state.textPrint.fretNumbers === "all" ? "checked" : ""} /> all</label>
          <label><input type="radio" name="print-fret-numbering" value="5_7_12" ${state.textPrint.fretNumbers === "5_7_12" ? "checked" : ""} /> 5/7/12</label>
          <label><input type="radio" name="print-fret-numbering" value="3_5_7_12" ${state.textPrint.fretNumbers === "3_5_7_12" ? "checked" : ""} /> 3/5/7/12</label>
          <label><input type="radio" name="print-fret-numbering" value="1_3_5_7_12" ${state.textPrint.fretNumbers === "1_3_5_7_12" ? "checked" : ""} /> 1/3/5/7/12</label>
        </div>
        <div class="option-row">
          <span class="option-label">number of frets:</span>
          <label><input type="radio" name="print-fret-count-mode" value="5" ${state.textPrint.fretCountMode === "5" ? "checked" : ""} /> 5</label>
          <label><input type="radio" name="print-fret-count-mode" value="7" ${state.textPrint.fretCountMode === "7" ? "checked" : ""} /> 7</label>
          <label><input type="radio" name="print-fret-count-mode" value="12" ${state.textPrint.fretCountMode === "12" ? "checked" : ""} /> 12</label>
          <label><input type="radio" name="print-fret-count-mode" value="24" ${state.textPrint.fretCountMode === "24" ? "checked" : ""} /> 24</label>
          <label><input type="radio" name="print-fret-count-mode" value="custom" ${state.textPrint.fretCountMode === "custom" ? "checked" : ""} /> custom</label>
          <input id="print-custom-frets" type="number" min="1" max="40" step="1" value="${state.textPrint.customFrets}" ${state.textPrint.fretCountMode === "custom" ? "" : "disabled"} />
        </div>
        <div class="option-row">
          <span class="option-label">open string:</span>
          <label><input type="checkbox" id="print-open-string-fret-number" ${state.textPrint.openStringFretNumber ? "checked" : ""} /> fret#</label>
          <label><input type="checkbox" id="print-open-string-nut" ${state.textPrint.openStringNut ? "checked" : ""} /> nut</label>
        </div>
      </details>
      <button id="copy-text">Copy to Clipboard</button>
      <button id="download-text">Download Text</button>
      <span class="save-notice ${Date.now() < copyNoticeUntil ? "visible" : ""}">copied to clipboard</span>
      <pre id="text-preview">${textPrintOutput()}</pre>
      </div>
      ` : `
      <div class="tab-content">
        <p>Save Profile stores your current custom scales/tunings and all app options to browser local storage for this device/browser profile.</p>
        <button id="save-profile">Save Profile</button>
        <p>Delete Profile removes the saved profile from browser local storage. This is only enabled when a saved profile exists.</p>
        <button id="delete-profile" ${hasSavedState() ? "" : "disabled"}>Delete Profile</button>
        <p>Defaults resets the app to built-in default values for scales, tunings, and options. To persist this reset, click Save Profile afterwards.</p>
        <button id="reset-defaults">Defaults</button>
        <span class="save-notice ${Date.now() < profileNoticeUntil ? "visible" : ""}">${profileNoticeText}</span>
      </div>
      `}
    </section>
  </main>`;

  wire();
  initFretTooltips();
}

function symbolMarkup(cell, appState) {
  if (cell.isRoot) {
    return `<span>.</span>`;
  }
  if (cell.inScale) {
    return `<span>o</span>`;
  }
  return "";
}

function noteLabelMarkup(cell, appState) {
  const label = noteLabel(cell.pitchClass, appState.noteNames, appState.noteAliases);

  if (!label.includes("/")) {
    const singleClass = cell.isRoot
      ? (appState.display.rootEmphasis === "emphasis" ? " emphasis" : "")
      : (appState.display.noteEmphasis === "emphasis" ? " emphasis" : "");
    return `<span class="note-single${singleClass}">${label}</span>`;
  }

  const dualClass = cell.isRoot && appState.display.rootEmphasis === "emphasis" ? " emphasis" : "";
  const [sharp, flat] = label.split("/");
  const sharpClass = appState.display.sharpFlatEmphasis === "sharp" ? " emphasis" : "";
  const flatClass = appState.display.sharpFlatEmphasis === "flat" ? " emphasis" : "";
  return `<span class="note-dual${dualClass}"><span class="sharp${sharpClass}">${sharp}</span><span class="flat${flatClass}">${flat}</span></span>`;
}

function fretTooltipContent(cell, appState) {
  const status = cell.isRoot ? "Root note" : (cell.inScale ? "In scale" : "Not in scale");
  const note = noteLabel(cell.pitchClass, appState.noteNames, appState.noteAliases);
  return `<div class='tip'><div><strong>${note}</strong></div><div>${status}</div><div>String: ${cell.string}</div><div>Fret: ${cell.fret}</div></div>`;
}

function initFretTooltips() {
  tippy(".fret[data-tooltip]", {
    allowHTML: true,
    interactive: false,
    delay: [120, 40],
    maxWidth: 280,
    content(reference) {
      return reference.getAttribute("data-tooltip") || "";
    }
  });
}

function textPrintOutput() {
  const scale = state.scales[state.currentScaleIndex];
  const key = state.noteNames[state.key];
  const notes = scaleNoteNames(scale, state.key, state.noteNames).join(" ");
  const start = 0;
  const end = resolveVisibleFrets(state.textPrint);
  const boardState = {
    ...state,
    display: {
      ...state.display,
      maxFrets: end
    }
  };
  const board = computeFretboard(boardState);
  const width = 5;
  const centerPad = (v) => {
    const text = String(v ?? "");
    if (!text) return " ".repeat(width);
    if (text.length >= width) return text.slice(0, width);
    const total = width - text.length;
    const left = Math.floor(total / 2);
    const right = total - left;
    return `${" ".repeat(left)}${text}${" ".repeat(right)}`;
  };
  const rows = [];

  const numberLine = () => {
    if (state.textPrint.fretNumbers === "none") return null;
    let line = "   ";
    for (let fret = start; fret < end; fret++) {
      const showLabel = fret === 0 ? state.textPrint.openStringFretNumber : shouldShowFretNumber(fret, state.textPrint.fretNumbers);
      line += centerPad(showLabel ? fret : "");
      if (state.textPrint.openStringNut && fret === 0) line += "|";
    }
    return line;
  };

  const top = numberLine();
  if (top) {
    rows.push(top);
    rows.push("");
  }

  for (let s = 0; s < 6; s++) {
    let line = `S${s + 1} `;
    for (let fret = start; fret < end; fret++) {
      const n = board[s][fret];
      if (!n.inScale && !n.isRoot) {
        line += centerPad("-");
      } else if (state.textPrint.showAs === "text") {
        line += centerPad(noteLabel(n.pitchClass, state.noteNames, state.noteAliases));
      } else {
        line += centerPad(n.isRoot ? "." : "o");
      }
      if (state.textPrint.openStringNut && fret === 0) line += "|";
    }
    rows.push(line);
  }

  const bottom = numberLine();
  if (bottom) {
    rows.push("");
    rows.push(bottom);
  }

  const symbolsLegend = state.textPrint.showAs === "symbol" ? ". root, o scale" : "";
  return `${state.textPrint.header1.replace("&name", scale.name).replace("&key", key)}\n${state.textPrint.header2.replace("&notes", notes)}\n\n${rows.join("\n")}\n\n${state.textPrint.trailer1.replace("&symbols", symbolsLegend)}`;
}

function wire() {
  app.querySelector("#tab-display").onclick = () => { lowerTab = "display"; render(); };
  app.querySelector("#tab-tuning").onclick = () => { lowerTab = "tuning"; render(); };
  app.querySelector("#tab-textprint").onclick = () => { lowerTab = "textprint"; render(); };
  app.querySelector("#tab-profile").onclick = () => { lowerTab = "profile"; render(); };

  app.querySelector("#key-select").onchange = (e) => { state.key = Number(e.target.value); render(); };
  app.querySelector("#scale-select").onchange = (e) => { state.currentScaleIndex = Number(e.target.value); render(); };
  app.querySelector("#new-scale").onclick = () => {
    const source = state.scales[state.currentScaleIndex];
    if (source?.builtIn) return;
    const name = makeUniqueCopyName(source.name, state.scales);
    state.scales.push({ name, notes: [...source.notes], builtIn: false });
    state.currentScaleIndex = state.scales.length - 1;
    sortScalesKeepCurrent();
    render();
  };
  app.querySelector("#rename-scale").onclick = () => {
    if (state.scales[state.currentScaleIndex]?.builtIn) return;
    const name = prompt("Rename scale", state.scales[state.currentScaleIndex].name);
    if (!name) return;
    state.scales[state.currentScaleIndex].name = name;
    sortScalesKeepCurrent();
    render();
  };
  app.querySelector("#delete-scale").onclick = () => {
    if (state.scales[state.currentScaleIndex]?.builtIn) return;
    if (state.scales.length < 2) return;
    state.scales.splice(state.currentScaleIndex, 1);
    state.currentScaleIndex = Math.max(0, state.currentScaleIndex - 1);
    render();
  };
  app.querySelectorAll(".note-toggle").forEach((b)=>b.onclick = (e)=>{
    ensureEditableCurrentScale();
    toggleScaleNoteByPitch(state, Number(e.target.dataset.note));
    render();
  });
  app.querySelectorAll(".fret").forEach((b)=>b.onclick = (e)=>{
    ensureEditableCurrentScale();
    toggleScaleNoteByPitch(state, Number(e.currentTarget.dataset.pitch));
    render();
  });
  if (lowerTab === "display") {
    app.querySelectorAll("input[name='fret-show-mode']").forEach((el) => {
      el.onchange = (e) => {
        state.display.noteShape = e.target.value === "text" ? "text" : "circle";
        render();
      };
    });
    app.querySelectorAll("input[name='sharp-flat-emphasis']").forEach((el) => {
      el.onchange = (e) => {
        state.display.sharpFlatEmphasis = e.target.value;
        render();
      };
    });
    app.querySelectorAll("input[name='note-emphasis']").forEach((el) => {
      el.onchange = (e) => {
        state.display.noteEmphasis = e.target.value;
        render();
      };
    });
    app.querySelectorAll("input[name='root-emphasis']").forEach((el) => {
      el.onchange = (e) => {
        state.display.rootEmphasis = e.target.value;
        render();
      };
    });
    app.querySelectorAll("input[name='fret-numbering']").forEach((el) => {
      el.onchange = (e) => {
        state.display.fretNumbering = e.target.value;
        render();
      };
    });
    app.querySelectorAll("input[name='fret-count-mode']").forEach((el) => {
      el.onchange = (e) => {
        state.display.fretCountMode = e.target.value;
        render();
      };
    });
    app.querySelector("#custom-frets").onchange = (e) => {
      const n = Math.min(40, Math.max(1, Number(e.target.value) || 24));
      state.display.customFrets = n;
      state.display.fretCountMode = "custom";
      render();
    };
    app.querySelector("#open-string-fret-number").onchange = (e) => {
      state.display.openStringFretNumber = e.target.checked;
      render();
    };
    app.querySelector("#open-string-nut").onchange = (e) => {
      state.display.openStringNut = e.target.checked;
      render();
    };
  }
  if (lowerTab === "textprint") {
    app.querySelectorAll("input[name='print-show-mode']").forEach((el) => {
      el.onchange = (e) => {
        state.textPrint.showAs = e.target.value === "text" ? "text" : "symbol";
        render();
      };
    });
    app.querySelectorAll("input[name='print-fret-numbering']").forEach((el) => {
      el.onchange = (e) => {
        state.textPrint.fretNumbers = e.target.value;
        render();
      };
    });
    app.querySelectorAll("input[name='print-fret-count-mode']").forEach((el) => {
      el.onchange = (e) => {
        state.textPrint.fretCountMode = e.target.value;
        render();
      };
    });
    app.querySelector("#print-custom-frets").onchange = (e) => {
      const n = Math.min(40, Math.max(1, Number(e.target.value) || 24));
      state.textPrint.customFrets = n;
      state.textPrint.fretCountMode = "custom";
      render();
    };
    app.querySelector("#print-open-string-fret-number").onchange = (e) => {
      state.textPrint.openStringFretNumber = e.target.checked;
      render();
    };
    app.querySelector("#print-open-string-nut").onchange = (e) => {
      state.textPrint.openStringNut = e.target.checked;
      render();
    };
  }

  if (lowerTab === "tuning") {
    app.querySelector("#tuning-select").onchange = (e) => { state.currentTuningIndex = Number(e.target.value); render(); };
    app.querySelectorAll(".tune-note").forEach((el)=>el.onchange = (e)=>{
      ensureEditableCurrentTuning();
      state.tunings[state.currentTuningIndex].notes[Number(e.target.dataset.index)] = Number(e.target.value);
      render();
    });
    app.querySelectorAll(".tune-oct").forEach((el)=>el.onchange = (e)=>{
      ensureEditableCurrentTuning();
      state.tunings[state.currentTuningIndex].octaves[Number(e.target.dataset.index)] = Number(e.target.value);
      render();
    });
    app.querySelector("#copy-tuning").onclick = () => {
      const source = state.tunings[state.currentTuningIndex];
      const name = makeUniqueTuningCopyName(source.name, state.tunings);
      state.tunings.push({ name, notes:[...source.notes], octaves:[...source.octaves], builtIn: false });
      state.currentTuningIndex = state.tunings.length - 1;
      render();
    };
    app.querySelector("#rename-tuning").onclick = () => {
      const t = state.tunings[state.currentTuningIndex];
      if (t?.builtIn) return;
      const name = prompt("Rename tuning", t.name);
      if (!name) return;
      t.name = stripDefaultTuningPrefix(name);
      render();
    };
    app.querySelector("#delete-tuning").onclick = () => {
      if (state.tunings.length < 2) return;
      if (state.tunings[state.currentTuningIndex]?.builtIn) return;
      state.tunings.splice(state.currentTuningIndex, 1);
      state.currentTuningIndex = Math.max(0, state.currentTuningIndex - 1);
      render();
    };
    app.querySelectorAll(".play-string").forEach((el)=>el.onclick = async (e)=>{
      const i = Number(e.target.dataset.index);
      if (activeStringIndex === i) {
        stopCurrentNote();
        activeStringIndex = null;
        render();
        return;
      }

      if (activeStringIndex !== null) {
        stopCurrentNote();
      }

      activeStringIndex = i;
      render();
      const t = state.tunings[state.currentTuningIndex];
      const playingIndex = i;
      await playManagedNote({
        note: t.notes[i],
        octave: t.octaves[i],
        ...state.midi,
        lengthMs: Number(state.tuningPlaySeconds) * 1000,
        onEnded: () => {
          if (activeStringIndex === playingIndex) {
            activeStringIndex = null;
            render();
          }
        }
      });
    });
    app.querySelector("#play-seconds").onchange = (e) => {
      const seconds = Number(e.target.value);
      state.tuningPlaySeconds = Number.isFinite(seconds) && seconds > 0 ? Math.floor(seconds) : 15;
      render();
    };
  }

  if (lowerTab === "textprint") {
    app.querySelector("#copy-text").onclick = async () => {
      const text = textPrintOutput();
      if (navigator.clipboard && navigator.clipboard.writeText) {
        await navigator.clipboard.writeText(text);
      } else {
        const ta = document.createElement("textarea");
        ta.value = text;
        document.body.appendChild(ta);
        ta.select();
        document.execCommand("copy");
        document.body.removeChild(ta);
      }
      copyNoticeUntil = Date.now() + 5000;
      render();
      window.setTimeout(() => {
        if (Date.now() >= copyNoticeUntil) render();
      }, 5000);
    };
    app.querySelector("#download-text").onclick = () => downloadText("scale.txt", textPrintOutput());
  }

  if (lowerTab === "profile") {
    app.querySelector("#save-profile").onclick = () => {
      saveState(buildPersistedProfile(state));
      profileNoticeText = "saved to local storage";
      profileNoticeUntil = Date.now() + 5000;
      render();
      window.setTimeout(() => {
        if (Date.now() >= profileNoticeUntil) render();
      }, 5000);
    };
    app.querySelector("#delete-profile").onclick = () => {
      if (!hasSavedState()) return;
      deleteState();
      profileNoticeText = "profile deleted from local storage";
      profileNoticeUntil = Date.now() + 5000;
      render();
      window.setTimeout(() => {
        if (Date.now() >= profileNoticeUntil) render();
      }, 5000);
    };
    app.querySelector("#reset-defaults").onclick = () => {
      const fresh = defaultState();
      Object.keys(state).forEach((k) => delete state[k]);
      Object.assign(state, fresh);
      profileNoticeText = "defaults loaded in memory";
      profileNoticeUntil = Date.now() + 5000;
      render();
      window.setTimeout(() => {
        if (Date.now() >= profileNoticeUntil) render();
      }, 5000);
    };
  }
}

sortScalesKeepCurrent();
render();
