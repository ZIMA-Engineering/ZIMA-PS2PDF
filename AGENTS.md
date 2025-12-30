# Repository Guidelines

## Project Structure & Module Organization
- Core code lives in `src/`, with Qt widgets (`MainWindow`, `SettingsDialog`, `ErrorDialog`, `AboutDialog`) split into `.cpp`, `.h`, and `.ui` files.
- `ZIMA-PS2PDF.pro` and the generated `Makefile` define the qmake build; `ZIMA-PS2PDF.qrc` lists bundled resources (icons in `gfx/`).
- Translations sit in `locale/`; place runtime `.qm` files next to the executable or inside `locale/`.
- `shell.nix` and `nix-build` provide a Nix-based environment; `README` covers legacy source tarball install notes.

## Build, Test, and Development Commands
- Always verify changes with a build (run `qmake` if needed, then `make`) before concluding work.
- `qmake` then `make` — configure and build the Qt application (Qt 4.x expected; adjust QMAKE variables if using Qt 5+).
- `make clean` — drop build artifacts before a fresh build.
- `./ZIMA-PS2PDF` — run the built GUI; ensure `ps2pdf` (Ghostscript) is on PATH.
- `nix-build` — optional: produce a Nix build using `shell.nix` if you prefer reproducible toolchains.

## Coding Style & Naming Conventions
- Follow existing Qt/C++ patterns: tabs for indentation, braces on new lines for class/namespace, method definitions aligned with the qmake-generated style.
- Classes use `PascalCase` (`MainWindow`), methods and slots use `camelCase` (`addFiles`, `openSettings`); member variables are plain names without prefixes.
- Use `tr()` for user-facing strings to keep translations working; keep UI text synchronized with `.ui` forms.
- Prefer Qt containers (`QStringList`, `QList`) and signals/slots connections matching current usage (`connect(..., SIGNAL(...), this, SLOT(...))`).

## Testing Guidelines
- No automated test suite exists; perform manual checks after changes:
  - Convert multiple `.ps`/`.plt` files and verify PDFs appear in the chosen target directory.
  - Toggle “delete source files” and target directory options to confirm persistence across sessions.
  - Validate translations load by placing `.qm` files in `locale/` and switching system locale.
- Keep logs visible: error output from `ps2pdf` should surface via `ErrorDialog`.

## Commit & Pull Request Guidelines
- Match existing history: short, imperative summaries (e.g., “Show ps2pdf output on error”, “Search for translation files in locale/”).
- Reference related issues when available and describe the user-visible impact (UI changes, build tweaks, translation updates).
- For UI changes, include before/after notes or screenshots; for build changes, note target Qt version or platform.
- Ensure PRs mention manual test steps performed and any dependencies (Qt version, Ghostscript/ps2pdf location).
