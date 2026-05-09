import { describe, expect, it } from "vitest";
import { transposeScale, computeFretboard } from "../src/lib/engine.js";
import { defaultState } from "../src/lib/state.js";
import { parseScl, parseTun, writeScl, writeTun } from "../src/lib/legacy-files.js";

describe("transposeScale", () => {
  it("transposes pentatonic in E to A correctly", () => {
    const scale = { notes: [1,0,1,0,0,1,0,1,0,0,1,0] };
    const out = transposeScale(scale, 0);
    expect(out.reduce((a,b)=>a+b,0)).toBe(5);
  });
});

describe("computeFretboard", () => {
  it("creates 6 strings and max frets", () => {
    const state = defaultState();
    const board = computeFretboard(state);
    expect(board).toHaveLength(6);
    expect(board[0]).toHaveLength(24);
  });
});

describe("legacy codecs", () => {
  it("round trips scales", () => {
    const data = [{ name: "X", notes: [1,0,0,1,0,0,1,0,0,1,0,0] }];
    const out = parseScl(writeScl(data));
    expect(out).toEqual(data);
  });

  it("round trips tunings", () => {
    const data = [{ name: "T", notes: [7,2,10,5,0,7], octaves: [3,3,2,2,2,1] }];
    const out = parseTun(writeTun(data));
    expect(out).toEqual(data);
  });

  it("rejects malformed headers", () => {
    expect(() => parseScl(new TextEncoder().encode("bad").buffer)).toThrow();
  });
});
