const HEADER = "!AJR!\n";

function writeInt32LE(value, ctx) {
  ctx.view.setInt32(ctx.offset, value, true);
  ctx.offset += 4;
}

function readInt32LE(view, offset) { return view.getInt32(offset, true); }

export function writeScl(scales) {
  const chunks = [new TextEncoder().encode(HEADER)];
  for (const scale of scales) {
    chunks.push(new TextEncoder().encode(`${scale.name}\n`));
    const arr = new Uint8Array(12 * 4);
    const ref = { view: new DataView(arr.buffer), offset: 0 };
    for (const n of scale.notes) writeInt32LE(n ? 1 : 0, ref);
    chunks.push(arr);
  }
  const total = chunks.reduce((n, c) => n + c.byteLength, 0);
  const out = new Uint8Array(total);
  let o = 0;
  for (const c of chunks) { out.set(c, o); o += c.byteLength; }
  return out.buffer;
}

export function writeTun(tunings) {
  const chunks = [new TextEncoder().encode(HEADER)];
  for (const tuning of tunings) {
    chunks.push(new TextEncoder().encode(`${tuning.name}\n`));
    const arr = new Uint8Array(12 * 4);
    const ref = { view: new DataView(arr.buffer), offset: 0 };
    for (const n of tuning.notes) writeInt32LE(n, ref);
    for (const o of tuning.octaves) writeInt32LE(o, ref);
    chunks.push(arr);
  }
  const total = chunks.reduce((n, c) => n + c.byteLength, 0);
  const out = new Uint8Array(total);
  let o = 0;
  for (const c of chunks) { out.set(c, o); o += c.byteLength; }
  return out.buffer;
}

function parseRecords(buffer, valueCount) {
  const bytes = new Uint8Array(buffer);
  const text = new TextDecoder().decode(bytes.slice(0, 6));
  if (text !== HEADER) throw new Error("Not a valid legacy file header");
  const records = [];
  let p = 6;
  const view = new DataView(buffer);

  while (p < bytes.length) {
    let end = p;
    while (end < bytes.length && bytes[end] !== 10) end++;
    if (end === p) break;
    const name = new TextDecoder().decode(bytes.slice(p, end));
    p = end + 1;
    const values = [];
    for (let i = 0; i < valueCount; i++) {
      values.push(readInt32LE(view, p));
      p += 4;
    }
    records.push({ name, values });
  }

  return records;
}

export function parseScl(buffer) {
  return parseRecords(buffer, 12).map((r) => ({ name: r.name, notes: r.values.map((v) => (v ? 1 : 0)) }));
}

export function parseTun(buffer) {
  return parseRecords(buffer, 12).map((r) => ({ name: r.name, notes: r.values.slice(0, 6), octaves: r.values.slice(6, 12) }));
}
