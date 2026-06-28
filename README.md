# Translhextion 2.0 — Resurrected by Hyperthermya

A hex editor built for ROM hacking, originally written by Brian Bennewitz (1999–2001) and later packaged with HTML help by Kitsune Sniper. This fork picks up where the original source left off: compiled cleanly under a modern toolchain, with additional improvements aligned with the RTHextion evolution.

## What this is

Translhextion is not a general-purpose hex editor. It was designed specifically for people translating games — you load a binary, point it at a Thingy-style table file, and it handles the messy parts: script dumping, DTE/MTE encoding, relative search, bookmark management, and direct in-place text entry via the IME toolbar. The original 1.6b source was the last version the author published before the project went dark.

## What changed in this fork

The original source compiled under Visual C++ 6.0 but needed some attention to run correctly on modern hardware.

**High-DPI fix** — the window was rendering blurry on any display above 96 DPI. Fixed by calling `SetProcessDPIAware` at startup via a runtime `GetProcAddress` lookup, so the binary stays compatible with Windows XP/Vista/7 without needing the manifest approach.

**Fast hex decoding** — `hex.h` now exposes `hex_nibble`, `char_hex_fast`, and `is_hex2`, which decode hex pairs without allocating temporary `string` objects. The original `char_hex(string)` call was fine for one-off lookups but showed up in profiling during large script dumps and replacements because it was called millions of times per operation. `table.h` was updated to use `dualbyteindex_fast` in the two-byte lookup path, which is the hottest inner loop during table-translated operations.

**Memory limit setting** — added a configurable file-load memory cap under Preferences (Options → Preferences, or Ctrl+I). The default is 512 MB. Set it to 0 to disable the check entirely. The setting persists to the registry alongside all other preferences. This is mainly useful if you're working on a machine with limited RAM and want the editor to refuse oversized files instead of hanging while it tries to allocate them.

**Pointer/section data models (backend only, not wired up yet)** — `PointerListModel` and `SectionListModel` (`pointerlistmodel.h/cpp`, `sectionlistmodel.h/cpp`) add bidirectional pointer lookup with configurable width and hierarchical, colored sections, modeled after RTHextion's equivalents. There is no menu item, dialog, or rendering hook that uses them yet — see "RTHextion comparison and roadmap" below for what's left.

**Change tracking scaffolding** — `models.cpp` adds position-tracking globals for a future "Changes" list; nothing populates them yet.

## RTHextion comparison and roadmap

[RTHextion](https://github.com/road-t/RTHextion) is a from-scratch Qt6 rewrite by Ilya Annikov — not a fork of this codebase. It shares Translhextion's purpose (ROM translation hex editing) but rebuilds everything on top of Qt: cross-platform (Windows/macOS/Linux), modular `src/{dialogs,dockwidgets,document,hexeditor,utils}` tree, `QUndoStack`-based undo/redo, multi-tab sessions, dockable Sections/Pointers/Tables/Changes widgets, a `.rthp` project file bundling multiple translation tables + pointers + original-data snapshots, ROM-type auto-detection for ~20 retro platforms, encoding auto-detection, virtual formatting, semi-automatic table generation, IPS patch import/export, and a 7-language UI with light/dark theming.

(For reference, [hayate891/translhextion](https://github.com/hayate891/translhextion) was checked too — it's a minimal 3-commit fork of the original VC6 source that fixes signed/unsigned bugs in `FileVector<T>` and `mainwindow.cpp` by changing `size_t` indices to `long`. This codebase's `filevect.h` already uses `long` throughout, so that fix doesn't apply here — it was presumably already part of the 1.6c base this fork started from.)

Porting RTHextion wholesale isn't realistic without rewriting this codebase on Qt — that's a different, much larger project. What's actually portable to this Win32/VC6 architecture, roughly in order of effort:

**Already started, needs UI wiring (cheapest wins):**
- Pointer model: hook `pointerrender.h`'s helpers into `paint()` for hex-view highlighting, add a Win32 list view (dialog or toolbar panel) to list/add/remove pointers, and call `PointerListModel::addPointer`/`dropPointer` from the existing find/replace and script-insert code paths so pointers auto-update when bytes shift.
- Section model: same pattern — a list/tree view panel, and a `colorAtOffset()` call from `paint()`'s background-fill logic.
- Multi-table support (`DocTableEntry`): wire to the existing single-table loader in `table.h`; needs a simple table-switcher (combo box or list) instead of RTHextion's dock widget.
- Change tracking: call `markPositionChanged()` from the existing byte-write paths (`SetAt`/`InsertAt` callers in `mainwindow.cpp`) and add a simple "Changes" list dialog.

**Self-contained, no UI dependency, moderate effort:**
- ROM-type/byte-order auto-detection: a static extension→type table plus header-sniffing for ambiguous `.bin`/`.rom`, similar to RTHextion's `romdetect.h` — straightforward to port as plain C++ data + a function, no Qt needed.
- IPS patch generation/import: self-contained binary diff format: feasible as a standalone function pair over `FileVector`.
- Original-data preservation: keep a second read-only copy of the loaded bytes (or just re-read from disk) to diff against current edits for a "compare to original" view.
- Lightweight project file: serialize the pointer/section snapshot methods (`snapshotPointers`/`restorePointers`, already stubbed) plus the active table path into a small text/INI-style file, extending the existing registry-based preference persistence rather than inventing a binary format.

**Large, likely not worth it without a bigger rewrite:**
- True multi-tab workspace with persisted session and dockable panels — Win32 has no built-in equivalent to Qt's `QDockWidget`/`QTabWidget`; this would mean building custom MDI-style window management from scratch.
- Generic command-pattern undo/redo (RTHextion's `CharCommand`/`QUndoStack`) — the current code has no undo concept at all; retrofitting one means touching every edit call site across `mainwindow.cpp`. Worth doing eventually, but it's the biggest single item here.
- Encoding auto-detection with the breadth of RTHextion's `encodingdetect.h` (CJK/Shift-JIS/EUC-JP/Windows-1251/UTF-8 with iconv fallback) — approximable with `IsTextUnicode`/`MultiByteToWideChar` heuristics, but won't match Qt's `QStringDecoder`-based detection without significant work.
- Cross-platform support — out of scope by design; this fork is Win32-only.

## Building

The project file is `Translhextion.dsp`, targeting Visual C++ 6 or later. It should also compile under Visual Studio 2005–2022 with the project converted to a newer format — the code uses Win32 API directly and has no third-party dependencies beyond the runtime.

Things the original readme recommended but are not done here yet:
- Switching from the custom `bstring`/`bvect` to STL `std::string`/`std::vector` — would improve speed further but requires source-level changes throughout
- `Translhextion::print_line` is still the biggest rendering bottleneck for large files; no changes there yet

If you compile and something explodes, the most likely culprits are the global-scope `vector<string>` definitions in `globals.h` — under some compiler optimization settings these can cause initialization order problems.

## Features (original)

- Hex and ASCII editing with configurable bytes-per-line and font
- Thingy table support: load `.tbl` files, view file contents translated through the table, dump/reinsert scripts
- DTE, MTE, and multi-byte value encoding/optimization
- Script dump bookmarks, bound to table file entries
- Direct IME text entry — type text directly into the file encoded on-the-fly
- Relative search and scan (useful for finding text encoded with unknown tables)
- Value scan relative (for non-western alphabets)
- Bookmarks (up to 9, saved to the table file)
- Partial file open
- SNES HiROM/LoROM offset display
- Japanese Shift-JIS and EUC-JIS support with custom Hiragana/Katakana font
- Bit manipulation dialog
- Template application
- Compare from current offset
- File properties, read-only mode, revert

## Memory limit option

Found under **Options → Preferences** (Ctrl+I), near the bottom of the dialog.

The field accepts a size in megabytes. Before loading any file, Translhextion checks the file size against this limit and refuses to proceed if it would be exceeded, showing the file size and the current limit in the error message. Set the field to `0` to turn off the check.

The value is saved to `HKCU\Software\Translhextion\2.0\` in the registry under `iMemoryLimitMB`.

## License

X11 License. Copyright (c) 1999, 2000, 2001 Brian Bennewitz. See `DOC\LICENSE.TXT` for the full text.

Distribution of the compiled program must include the font (`FONT\Januschan JOME Regular.ttf`), the help file (`HELP\`), and the original license. These requirements were set by Brian Bennewitz and documented by Kitsune Sniper.