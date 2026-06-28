# Changelog

## 2.1 — 2026-06-28

Data-layer groundwork for a set of RTHextion-inspired features. **None of this is wired into the UI yet** — no menu items, no rendering hooks, no project file format. See "RTHextion comparison and roadmap" in README.md for what is and isn't done.

### Added
- `PointerListModel` (`pointerlistmodel.h/cpp`) — bidirectional offset↔pointer lookup backed by `std::map`/`std::multimap`, configurable pointer width (2/3/4 bytes packed into the high bits of the stored target), snapshot/restore methods for future project persistence. Not called from `mainwindow.cpp`.
- `SectionListModel` (`sectionlistmodel.h/cpp`) — flat list of `Section` (start/end offset, color, parent index for hierarchy), with offset→color lookup. Not called from `mainwindow.cpp`.
- `PointerEntry`, `Section`, `DocTableEntry` structs added to `structs.h`.
- `pointerrender.h` — standalone helper functions (`isByteInPointer`, `getPointerAtOffset`, `getPointerRenderColor`, `getPointerTargetData`) intended for a future `paint()` integration. Currently `#include`d nowhere.
- `models.cpp` — change-tracking globals (`g_changedPositions`, `g_changedRangeStart/End`) and their accessor functions, plus storage for the multi-table and virtual-formatting globals. No call sites yet.
- New `modeldata.h` header holding `extern` declarations for all of the above, kept separate from `globals.h` (see Fixed).

### Fixed
- **Build was broken**: the new model files didn't compile or link.
  - `structs.h`/`globals.h` use the project's own `string`/`vector<T>` classes (defined in `bstring.h`/`bvect.h`), not `std::string`/`std::vector` — the new headers included `structs.h`/`globals.h` directly without including `bstring.h`/`bvect.h`/`constants.h` first, so `string`/`vector`/constants like `STANDARDHEX` were undefined in those translation units.
  - `globals.h` is a single-inclusion *definitions* file meant to be `#include`d only by `mainwindow.cpp`; `sectionlistmodel.cpp` was also including it, which redefined every legacy global and caused `LNK2005` duplicate-symbol errors. Moved the new model `extern` declarations out of `globals.h` into `modeldata.h`, which the model `.cpp` files now include instead of `globals.h`.
  - `g_changedPositions`/`g_changedRangeStart`/`g_changedRangeEnd` were declared `extern` but never defined anywhere, causing `LNK2019` unresolved externals. Defined them in `models.cpp`.
- `build.bat` updated to compile `models.cpp`, `pointerlistmodel.cpp`, `sectionlistmodel.cpp` (this was already done locally, just verified working).
- `pointerrender.h` had two bugs that would have surfaced the moment it was actually included: it referenced `FileBuffer` as if it were a global, but it's a **private member of class `Translhextion`** (`mainwindow.h`), so a free function can't see it; and `getPointerTargetData` was declared to return `std::string` while its body built and returned the project's own custom `string` class (`bstring.h`), which has no implicit conversion to `std::string`. Fixed by having the function take `FileVector<unsigned char>& fileBuffer, __int64 fileSize` as parameters (to be passed by the calling `Translhextion` member function) and return `string` instead of `std::string`. Verified by temporarily wiring the header into `mainwindow.h` and building — compiles clean — then reverted the wiring since menu/rendering integration is still unstarted (see roadmap in README.md).

## 2.0 — 2026-06-16

### Added
- **RAM memory limit** option in Options → Preferences. Sets the maximum file size (in MB) that can be loaded into memory. Enter 0 to disable the limit. Value is saved to the registry and restored on next launch.
- **DPI awareness** declared at startup so the UI no longer appears blurry on high-DPI displays.
- **build.bat** at project root to rebuild from source with a single command using VS2022 Build Tools x86.

### Fixed
- 32-bit integer overflow in the memory limit check: files were being incorrectly rejected even when the limit was set high (e.g., 8192 MB overflowed a 32-bit `long` to 0, causing every file to appear "too large"). Fixed by using `__int64` arithmetic.
- Four occurrences of the typo "sucessfully" corrected to "successfully".
- All user-facing messages are now English only.

### Improved
- Script dump write speed: `write_script_file` now batches output into 64 KB chunks instead of calling the stream operator once per string. Reduces syscall overhead significantly on large ROMs.
- Compiled output is now organized under `bin/`: `Translhextion.exe`, `Translhextion.chm`, and `Januschan JOME Regular.ttf` all land there after each build, keeping the source tree clean.