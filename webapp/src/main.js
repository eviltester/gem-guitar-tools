import "./style.css";
import { defaultState } from "./lib/state.js";
import { computeFretboard, toggleScaleNoteByPitch, noteLabel, scaleNoteNames } from "./lib/engine.js";
import { saveState, loadState, downloadText } from "./lib/storage.js";
import { playManagedNote, stopCurrentNote } from "./lib/audio.js";

const state = Object.assign(defaultState(), loadState() || {});
const app = document.querySelector("#app");
let activeStringIndex = null;
let displayOptionsOpen = false;
let textPrintOptionsOpen = false;
let textPreviewActive = false;

if (!state.tuningPlaySeconds || state.tuningPlaySeconds < 1) {
  state.tuningPlaySeconds = 15;
}
state.display.noteShape = state.display.noteShape === "text" ? "text" : "circle";
state.display.sharpFlatEmphasis = ["none", "sharp", "flat"].includes(state.display.sharpFlatEmphasis) ? state.display.sharpFlatEmphasis : "none";
state.display.noteEmphasis = ["none", "emphasis"].includes(state.display.noteEmphasis) ? state.display.noteEmphasis : "none";
state.display.rootEmphasis = ["none", "emphasis"].includes(state.display.rootEmphasis) ? state.display.rootEmphasis : "none";
state.display.fretNumbering = ["none", "all", "5_7_12", "3_5_7_12", "1_3_5_7_12"].includes(state.display.fretNumbering) ? state.display.fretNumbering : "none";
state.textPrint.showAs = state.textPrint.showAs === "text" ? "text" : "symbol";
state.textPrint.fretNumbers = ["none", "all", "5_7_12", "3_5_7_12", "1_3_5_7_12"].includes(state.textPrint.fretNumbers) ? state.textPrint.fretNumbers : "none";

function stripDefaultScalePrefix(name) {
  return (name || "").replace(/^DEFAULT SCALE\s*-\s*/i, "").trim();
}

function normalizeScaleNames() {
  state.scales = state.scales.map((s) => ({ ...s, name: stripDefaultScalePrefix(s.name) }));
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

function fretNumbersRow(maxFrets, mode) {
  const cells = [];
  for (let fret = 0; fret < maxFrets; fret++) {
    const label = shouldShowFretNumber(fret, mode) ? fret : "";
    cells.push(`<button class="fret fret-num-cell" tabindex="-1" aria-hidden="true">${label}</button>`);
  }
  return `<div class="string-row fret-num-row"><span class="string-label"></span>${cells.join("")}</div>`;
}

function render() {
  const prevDisplay = app.querySelector("#display-options");
  const prevPrint = app.querySelector("#text-print-options");
  if (prevDisplay) displayOptionsOpen = prevDisplay.open;
  if (prevPrint) textPrintOptionsOpen = prevPrint.open;

  const scale = state.scales[state.currentScaleIndex];
  const tuning = state.tunings[state.currentTuningIndex];
  const board = computeFretboard(state);

  app.innerHTML = `
  <main>
    <h1>Guitar Reference</h1>
    <div class="toolbar">
      <button id="reset-defaults">Defaults</button>
      <button id="save-profile">Save Profile</button>
      <label>Key <select id="key-select">${optionsForNotes()}</select></label>
      <label>Scale <select id="scale-select">${state.scales.map((s,i)=>`<option value="${i}" ${i===state.currentScaleIndex?"selected":""}>${s.name}</option>`).join("")}</select></label>
      <button id="new-scale">Store As</button>
      <button id="rename-scale">Rename</button>
      <button id="delete-scale">Delete</button>
    </div>

    <section class="panel">
      <h2>Scale Notes</h2>
      <div class="notes">${state.noteNames.map((n,i)=>`<button class="note-toggle ${scaleNoteNames(scale,state.key,state.noteNames).includes(n)?"on":""}" data-note="${i}">${n}</button>`).join("")}</div>
      <p>${scale.name} in ${state.noteNames[state.key]}</p>
    </section>

    <section class="panel">
      <h2>Fretboard</h2>
      <details class="fret-options" id="display-options" ${displayOptionsOpen ? "open" : ""}>
        <summary>Display Options</summary>
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
      </details>
      <div class="fretboard">
        ${fretNumbersRow(state.display.maxFrets, state.display.fretNumbering)}
        ${board.map((row,ri)=>`<div class="string-row"><span class="string-label">S${ri+1}</span>${row.map((cell)=>{
          const cls = cell.isRoot ? "root" : cell.inScale ? "in" : "off";
          const rel = cell.isRelative ? " rel" : "";
          const text = state.display.noteShape === "text"
            ? (cell.isRoot || cell.inScale ? noteLabelMarkup(cell, state) : "")
            : symbolMarkup(cell, state);
          return `<button class="fret ${cls}${rel}" data-pitch="${cell.pitchClass}" data-string="${cell.string}" data-fret="${cell.fret}">${text}</button>`;
        }).join("")}</div>`).join("")}
        ${fretNumbersRow(state.display.maxFrets, state.display.fretNumbering)}
      </div>
    </section>

    <section class="panel tuning">
      <h2>Tuning</h2>
      <div>
        <label>Tuning Preset <select id="tuning-select">${state.tunings.map((t,i)=>`<option value="${i}" ${i===state.currentTuningIndex?"selected":""}>${t.name}</option>`).join("")}</select></label>
        <button id="store-tuning">Store Tuning</button><button id="delete-tuning">Delete Tuning</button>
      </div>
      <div>
        <label>Duration (s) <input id="play-seconds" type="number" min="1" step="1" value="${state.tuningPlaySeconds}" /></label>
      </div>
      ${tuning.notes.map((note, i) => `<div>String ${i+1}: <select class="tune-note" data-index="${i}">${state.noteNames.map((n,ni)=>`<option value="${ni}" ${ni===note?"selected":""}>${n}</option>`).join("")}</select>
      Oct <input class="tune-oct" data-index="${i}" type="number" min="-3" max="5" value="${tuning.octaves[i]}" />
      <button class="play-string" data-index="${i}">${activeStringIndex === i ? "Stop" : "Play"}</button></div>`).join("")}
    </section>

    <section class="panel">
      <h2>Text Print</h2>
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
      </details>
      <button id="preview-text">Preview Text</button>
      <button id="download-text">Download Text</button>
      <pre id="text-preview"></pre>
    </section>
  </main>`;

  wire();
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

function textPrintOutput() {
  const scale = state.scales[state.currentScaleIndex];
  const key = state.noteNames[state.key];
  const notes = scaleNoteNames(scale, state.key, state.noteNames).join(" ");
  const board = computeFretboard(state);
  const start = Math.max(0, Number(state.textPrint.startFret) || 0);
  const count = Math.max(1, Number(state.textPrint.noFrets) || 12);
  const end = Math.min(state.display.maxFrets, start + count);
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
      line += centerPad(shouldShowFretNumber(fret, state.textPrint.fretNumbers) ? fret : "");
    }
    return line;
  };

  const top = numberLine();
  if (top) rows.push(top);

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
    }
    rows.push(line);
  }

  const bottom = numberLine();
  if (bottom) rows.push(bottom);

  const symbolsLegend = state.textPrint.showAs === "symbol" ? ". root, o scale" : "";
  return `${state.textPrint.header1.replace("&name", scale.name).replace("&key", key)}\n${state.textPrint.header2.replace("&notes", notes)}\n\n${rows.join("\n")}\n\n${state.textPrint.trailer1.replace("&symbols", symbolsLegend)}`;
}

function refreshTextPreviewIfActive() {
  if (!textPreviewActive) return;
  const preview = app.querySelector("#text-preview");
  if (preview) preview.textContent = textPrintOutput();
}

function wire() {
  app.querySelector("#key-select").onchange = (e) => { state.key = Number(e.target.value); render(); };
  app.querySelector("#scale-select").onchange = (e) => { state.currentScaleIndex = Number(e.target.value); render(); };
  app.querySelector("#new-scale").onclick = () => {
    const name = prompt("Scale name", `${state.scales[state.currentScaleIndex].name} copy`);
    if (!name) return;
    state.scales.push({ name, notes: [...state.scales[state.currentScaleIndex].notes] });
    state.currentScaleIndex = state.scales.length - 1;
    sortScalesKeepCurrent();
    render();
  };
  app.querySelector("#rename-scale").onclick = () => {
    const name = prompt("Rename scale", state.scales[state.currentScaleIndex].name);
    if (!name) return;
    state.scales[state.currentScaleIndex].name = name;
    sortScalesKeepCurrent();
    render();
  };
  app.querySelector("#delete-scale").onclick = () => {
    if (state.scales.length < 2) return;
    state.scales.splice(state.currentScaleIndex, 1);
    state.currentScaleIndex = Math.max(0, state.currentScaleIndex - 1);
    render();
  };
  app.querySelectorAll(".note-toggle").forEach((b)=>b.onclick = (e)=>{ toggleScaleNoteByPitch(state, Number(e.target.dataset.note)); render(); });
  app.querySelectorAll(".fret").forEach((b)=>b.onclick = (e)=>{ toggleScaleNoteByPitch(state, Number(e.currentTarget.dataset.pitch)); render(); });
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
  app.querySelectorAll("input[name='print-show-mode']").forEach((el) => {
    el.onchange = (e) => {
      state.textPrint.showAs = e.target.value === "text" ? "text" : "symbol";
      render();
      refreshTextPreviewIfActive();
    };
  });
  app.querySelectorAll("input[name='print-fret-numbering']").forEach((el) => {
    el.onchange = (e) => {
      state.textPrint.fretNumbers = e.target.value;
      render();
      refreshTextPreviewIfActive();
    };
  });

  app.querySelector("#tuning-select").onchange = (e) => { state.currentTuningIndex = Number(e.target.value); render(); };
  app.querySelectorAll(".tune-note").forEach((el)=>el.onchange = (e)=>{ state.tunings[state.currentTuningIndex].notes[Number(e.target.dataset.index)] = Number(e.target.value); render(); });
  app.querySelectorAll(".tune-oct").forEach((el)=>el.onchange = (e)=>{ state.tunings[state.currentTuningIndex].octaves[Number(e.target.dataset.index)] = Number(e.target.value); render(); });
  app.querySelector("#store-tuning").onclick = () => {
    const name = prompt("Store tuning as", `${state.tunings[state.currentTuningIndex].name} copy`);
    if (!name) return;
    const t = state.tunings[state.currentTuningIndex];
    state.tunings.push({ name, notes:[...t.notes], octaves:[...t.octaves] });
    state.currentTuningIndex = state.tunings.length - 1;
    render();
  };
  app.querySelector("#delete-tuning").onclick = () => {
    if (state.tunings.length < 2) return;
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

  app.querySelector("#preview-text").onclick = () => {
    textPreviewActive = true;
    app.querySelector("#text-preview").textContent = textPrintOutput();
  };
  app.querySelector("#download-text").onclick = () => downloadText("scale.txt", textPrintOutput());

  app.querySelector("#save-profile").onclick = () => saveState(state);
  app.querySelector("#reset-defaults").onclick = () => {
    const fresh = defaultState();
    Object.keys(state).forEach((k) => delete state[k]);
    Object.assign(state, fresh);
    render();
  };
}

sortScalesKeepCurrent();
render();
