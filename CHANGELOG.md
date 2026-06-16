# Changelog

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
