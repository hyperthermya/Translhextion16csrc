# Translhextion 2.0 — Resurrected by Hyperthermya

A hex editor built for ROM hacking, originally written by Brian Bennewitz (1999–2001) and later packaged with HTML help by Kitsune Sniper. This fork picks up where the original source left off: compiled cleanly under a modern toolchain, with a handful of targeted fixes and performance improvements applied to the critical paths.

## What this is

Translhextion is not a general-purpose hex editor. It was designed specifically for people translating games — you load a binary, point it at a Thingy-style table file, and it handles the messy parts: script dumping, DTE/MTE encoding, relative search, bookmark management, and direct in-place text entry via the IME toolbar. The original 1.6b source was the last version the author published before the project went dark.

## What changed in this fork

The original source compiled under Visual C++ 6.0 but needed some attention to run correctly on modern hardware.

**High-DPI fix** — the window was rendering blurry on any display above 96 DPI. Fixed by calling `SetProcessDPIAware` at startup via a runtime `GetProcAddress` lookup, so the binary stays compatible with Windows XP/Vista/7 without needing the manifest approach.

**Fast hex decoding** — `hex.h` now exposes `hex_nibble`, `char_hex_fast`, and `is_hex2`, which decode hex pairs without allocating temporary `string` objects. The original `char_hex(string)` call was fine for one-off lookups but showed up in profiling during large script dumps and replacements because it was called millions of times per operation. `table.h` was updated to use `dualbyteindex_fast` in the two-byte lookup path, which is the hottest inner loop during table-translated operations.

**Memory limit setting** — added a configurable file-load memory cap under Preferences (Options → Preferences, or Ctrl+I). The default is 512 MB. Set it to 0 to disable the check entirely. The setting persists to the registry alongside all other preferences. This is mainly useful if you're working on a machine with limited RAM and want the editor to refuse oversized files instead of hanging while it tries to allocate them.

**Version bump** — `constants.h` was updated to version `2.0` with the `Resurrected by Hyperthermya` sub-release tag, which shows up in the About box.

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
