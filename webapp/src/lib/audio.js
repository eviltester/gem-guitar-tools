let audioContext;
let activePlayback = null;

function clearActive(playback) {
  if (activePlayback === playback) activePlayback = null;
}

export function stopCurrentNote() {
  if (activePlayback) activePlayback.stop();
}

export async function playManagedNote({
  note,
  octave,
  channel,
  velocity,
  lengthMs,
  midiEnabled,
  midiNoteOff,
  onEnded
}) {
  const midiNote = note + 24 - 3 + octave * 12;
  let finished = false;
  let timerId = null;

  const endPlayback = (playback, emitEnded = true) => {
    if (finished) return;
    finished = true;
    if (timerId) window.clearTimeout(timerId);
    clearActive(playback);
    if (emitEnded && onEnded) onEnded();
  };

  if (midiEnabled && navigator.requestMIDIAccess) {
    try {
      const access = await navigator.requestMIDIAccess();
      const out = access.outputs.values().next().value;
      if (out) {
        out.send([0x90 + channel, midiNote, velocity]);
        const playback = {
          stop: () => {
            if (midiNoteOff) out.send([0x80 + channel, midiNote, velocity]);
            else out.send([0x90 + channel, midiNote, 0]);
            endPlayback(playback);
          }
        };
        activePlayback = playback;
        timerId = window.setTimeout(() => playback.stop(), lengthMs);
        return playback;
      }
    } catch {
      // fallback below
    }
  }

  audioContext = audioContext || new AudioContext();
  const o = audioContext.createOscillator();
  const g = audioContext.createGain();
  const hz = 440 * 2 ** ((midiNote - 69) / 12);
  o.type = "triangle";
  o.frequency.value = hz;
  g.gain.value = Math.max(0.05, velocity / 127 * 0.25);
  o.connect(g);
  g.connect(audioContext.destination);
  o.start();

  const playback = {
    stop: () => {
      try { o.stop(); } catch { /* already stopped */ }
      endPlayback(playback);
    }
  };
  o.onended = () => endPlayback(playback);
  activePlayback = playback;
  timerId = window.setTimeout(() => playback.stop(), lengthMs);
  return playback;
}
