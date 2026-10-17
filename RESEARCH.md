# Lensfun: project state, Windows build, and the C/C++ split

Research notes on the Lensfun repository at `D:\Dev\lensfun` (origin `squareSphere29/lensfun`, forked from `lensfun/lensfun`). Read-only investigation; no code changed.

## Overview

Lensfun is a C/C++ library plus XML lens database. The public API is C-compatible (`struct` with methods under `#ifdef __cplusplus`) and the implementation is C++. Internals mix GLib (GString, GPtrArray, g_strdup) with `std::vector`, plus custom template helpers in `libs/lensfun/lensfunprv.h`.

The canonical upstream is `lensfun/lensfun` on GitHub. The local clone's `origin` (`squareSphere29/lensfun`) is an inactive fork (0 stars, 0 forks); all real activity, issues, and PRs live under `lensfun/lensfun`.

## Project state

- Upstream is low-velocity but not dead. 88 open issues and 18 open PRs as of this check. Releases are infrequent: v0.3.3 is the last stable tag referenced by users; v0.3.4 broke the Windows build.
- Maintenance is concentrated in a small set of contributors (`seebk`, `CAHEK7`, `kmilos`, `letmaik`). Many PRs sit open for years (e.g. #2176 open since Mar 2024, #2138 since Jan 2024).
- The project moved from Berlios/SourceForge to GitHub; legacy archive trees (`berlios-archive/`, `sourceforge-archive/`) remain in the repo.

## Windows CMake status

The Windows build is the sharpest pain point:

- Issue #2676: v0.3.4 dropped `SET(PLATFORM_WINDOWS 1)` from the CMake side, so `lensfun.h` fails with "I don't know how to change symbol visibility for your compiler". The build-side fix exists in `master` but is not in a release. The header-side fix is still missing: `include/lensfun/lensfun.h.in` lines 84 and 92 test `PLATFORM_WINDOWS`, which is only defined via `config.h`. Consumers that include the installed header without `config.h` (MinGW) still fail. Replace those tests with `_WIN32`.
- Issue #2520: CMake 4 breaks the build (closed Oct 2025).
- Issue #1998: MinGW/MSYS2 GCC locale_t errors (closed Aug 2023).
- Issue #2001: MinGW cross-compile for Windows 64-bit broken (closed Jun 2024).
- Issue #1971: `cmake-windows` and `make-lenslist` CI workflows stopped working (closed Mar 2023).
- Issue #1938: pkg-config generation problems (closed Nov 2023).

CI today: `.github/workflows/cmake-windows.yml` builds with Visual Studio 2022 + vcpkg (`glib`, `libpng`). `.github/workflows/cmake-msys2.yml` builds under MSYS2 UCRT64. Both target `master`.

The local checkout has no `cmake` or `vcpkg` on PATH, so the build cannot be reproduced here without installing them.

## The C/C++ mishmash

The public header `include/lensfun/lensfun.h.in` defines plain C structs (`lfMount`, `lfCamera`, `lfLens`, `lfDatabase`) that grow C++ methods under `#ifdef __cplusplus`, while keeping C-style `lf_*_create`/`lf_*_destroy` free functions. It also exposes 25 `DEPRECATED` C-style struct members (`Mounts`, `CalibDistortion`, `CalibTCA`, `CalibVignetting`, `CalibCrop`, `CalibFov`, …) that C consumers read directly. Those members are live ABI, not just wrappers. Internals use both GLib and STL:

- `libs/lensfun/lensfunprv.h` declares custom template helpers (`absolute`, `square`, `clampd`) alongside GLib types and `std::vector`.
- `lfFuzzyStrCmp` uses `GPtrArray` for word lists.
- Vectors are null-terminated by reserving `size()+1` and writing `NULL` into the extra slot (`_lf_terminate_vec`), so the C API can return `const T* const*` without ownership transfer. This is the core coupling that makes refactoring hard.

Issue #2142 ("Try to separate the C and C++") is the canonical discussion. The maintainer (`seebk`) explains the null-terminated-vector trick exists to keep the C API backward compatible and to avoid ownership transfer. Contributors (`CAHEK7`, `payano`) want to modernize with `std::shared_ptr`/`std::unique_ptr` and clean separation, but note the cost: changing the C API ownership model.

Open modernization PRs:

- #2176 "replace custom helper templates with C++ STL" (open since Mar 2024, approved by CAHEK7). Replaces `clampd` etc. with `std::clamp`, `std::hypot`, cmath. Requires C++17 (already set).
- #2163 "Modernize lfMount" (open since Feb 2024).
- #2160 "Add C interface for EnableDistortionCorrection".
- #2138 "Use smart pointers for lfLens Calibrations" (open since Jan 2024).
- #2687 "Modernize calibration tool" (open since Feb 2026).
- #2169 "Add clang-tidy CI action".

These PRs have stalled, which matches the "semi-abandoned" observation.

## Assessment

The C/C++ coupling is real but intentional: the null-terminated vector trick is a documented compatibility shim, not an accident. Reworking it means either (a) keeping the shim and modernizing internals in place, or (b) changing the C API ownership model, which breaks downstream consumers (darktable, lensfunpy, rawtherapee). Given downstream usage and the maintainer's stated caution, option (a) is the pragmatic path.

The Windows build breakage is the more urgent, concrete defect: it blocks real users. The build-side fix exists in `master` but is not released; the header-side fix (`_WIN32` instead of `PLATFORM_WINDOWS`) is still missing. The gap is both code and release management.

## Feasibility of a full C++ migration

Short answer: yes, it is feasible, but the "thin wrapper" framing understates the coupling. The implementation is C++, but the C API is baked into struct layout and ABI through 25 deprecated members (`Mounts`, `CalibDistortion`, …) that C consumers read directly. There are roughly 70 `lf_*` free functions in the header, not 66.

Evidence:

- The implementation files (`libs/lensfun/*.cpp`) are C++. The C-specific code is ~70 `lf_*` free functions (e.g. `lf_mount_create`, `lf_lens_create`) plus the `#ifndef __cplusplus` branches and the 25 deprecated struct members in `include/lensfun/lensfun.h.in`.
- The null-terminated `std::vector` trick (`_lf_terminate_vec`, `lensfunprv.h:268-274`) exists solely to serve the C API's `const T* const*` return. Dropping the C API lets vectors be normal vectors. Note: `reserve(size+1)` does not change `size()`, so any mutating operation that forgets to re-terminate silently breaks C consumers.
- `FindCameras`/`FindLenses` in `database.cpp` allocate with `g_new` and return raw pointers with unspecified ownership. A migration must resolve this.
- GLib (`GString`, `GPtrArray`, `g_strdup`, `g_new`) is used in XML export and memory allocation, not just I/O; PR #2176 and the 0.3.95 release notes moved containers to STL, so finishing that is incremental but not trivial.
- The news page documents the direction: C++11, STL containers. The static `lfXX::Create()`/`Destroy()` methods were deprecated in 0.3.3/0.3.95 and are gone from current master (0.3.99); constructors/destructors are used now.

Cost and breakage:

- Removing the C API breaks every C consumer and every binding that uses ctypes/cffi: lensfunpy (Python), and any C program. C++ consumers (darktable, rawtherapee, digikam, ffmpeg, gimp-lensfun, hugin) can adapt, but each needs source changes and testing.
- lensfunpy would need a rewrite (pybind11/cppyy) against the C++ API.
- ABI: a new SONAME and parallel installability, coordination with distros (Debian, Fedora, Homebrew, vcpkg).
- The data pipeline is unaffected, so a fork can ship the C++ library and keep updating the database.

Recommended path for a fork that accepts divergence (only if you accept the cost):

1. Delete the `lf_*` wrappers and the `#ifndef __cplusplus` branches; bump SONAME. Note: the deprecated struct members (`Mounts`, `Calib*`) are live ABI and cannot be removed without breaking C consumers, so keep them or provide a shim.
2. Replace the null-terminated vectors with normal `std::vector` and iterators (or a small span-like view) throughout `database.cpp`, `lens.cpp`, `mount.cpp`. Add an invariant test that the trailing element is NULL after every mutating operation until then.
3. Resolve ownership of `g_new`-allocated find results in `database.cpp`.
4. Finish the GLib-to-STL migration (GString -> std::string, GPtrArray -> std::vector, g_new -> new) in `auxfun.cpp` and `lfFuzzyStrCmp`.
5. Provide an optional `lensfun-c` compatibility shim (thin C wrappers) for consumers that cannot migrate. Note: this re-exposes the C API, so the "C++-only" claim is partly nominal.
6. Rewrite lensfunpy against the C++ API with pybind11.

This is a multi-week effort. The implementation is already C++, but the ABI and ownership coupling is real, not just coordination.

## On a "permanent better fork"

A fork that only fixes code is not enough, because the value users get is the database. A credible fork must keep the data pipeline running: accept calibration PRs, run `check_database.py`, regenerate tarballs with `generate_db.py`, and host them at a stable URL that `lensfun-update-data` can reach. It must also keep the C API backward compatible, since darktable, lensfunpy, and rawtherapee link against it.

## Recommendation

1. Adopt the maintainer's incremental stance (issue #2142): keep the C API and the null-terminated-vector shim, but add an invariant test that the trailing element is NULL after every mutating operation in `lens.cpp`, `mount.cpp`, `database.cpp`. Modernize internals behind tests. Do not promise a C++-only API.
2. Fix the Windows defect at the header level: replace `PLATFORM_WINDOWS` with `_WIN32` in `include/lensfun/lensfun.h.in` (lines 84, 92), and backport the build-side fix to a 0.3.4.x branch. Cut a patched stable release, not from master (which is 0.3.99 alpha). This is what issue #2676 actually asks for.
3. Add a consumer-compile smoke test in CI: compile a trivial file `#include <lensfun/lensfun.h>` with both MSVC and MinGW against the *installed* header. The existing `cmake-windows.yml` and `cmake-msys2.yml` only build the library, so header-side regressions (the actual #2676 failure) slip through.
4. Triage the stalled PRs rather than batching them: rebase or close #2176 (STL helpers; C++17 already set), keep #2163/#2160 as optional, defer #2138 (smart pointers) until the ownership model is agreed, close anything that no longer applies.
5. Keep GLib for now, but scope any future migration to include `GString`, `GPtrArray`, `g_new`, and `g_strdup` in `auxfun.cpp`, `database.cpp`, and `lfFuzzyStrCmp`, not just I/O.
6. If a fork is wanted, scope it to "stable Windows build + data pipeline," not "C++-only API." The database is the value (see "On a permanent better fork"), and C API compatibility is the binding constraint per #2142.

## How the database data is derived

The database is community-calibrated, not scraped. The flow:

1. A contributor photographs calibration targets (buildings for distortion, sharp edges for TCA, diffuser for vignetting) as RAW, organized into `distortion/`, `tca/`, `vignetting/` directories.
2. `tools/calibrate/calibrate.py` reads EXIF (or filename-encoded metadata), converts RAW to TIFF, and on first run writes a `lenses.txt` template. Distortion coefficients come from Hugin (`a,b,c` for the `ptlens` model, or a single `k1` for `poly3`).
3. The contributor fills in `lenses.txt` (maker, mount, crop factor, coefficients) and runs `calibrate.py` again, which fits TCA and vignetting models and writes a `lensfun.xml` fragment.
4. The fragment is submitted as a PR and merged into `data/db/*.xml` (one file per camera maker family). `tools/check_database/check_database.py` validates XML schema (xmllint) and primary-key uniqueness before merge.
5. `tools/update_database/generate_db.py` + `xml_converter.py` convert the XML into versioned tarballs (`version_0.tar.bz2` ... `version_N.tar.bz2`) plus `versions.json`, using the git commit timestamp of the XML files. These are hosted at `https://lensfun.github.io/db/` and `https://wilson.bronger.org/lensfun-db/`.
6. End users run `apps/lensfun-update-data`, which fetches `versions.json`, picks the best version matching the installed library's database version, and extracts tarballs into `/var/lib/lensfun-updates`.

So "maintaining the database" means merging calibration PRs and regenerating tarballs. The code library is the part that is semi-abandoned; the data pipeline is still active (recent PRs are lens additions).

## Other forks surveyed

Checked the fork network of `lensfun/lensfun` (2 pages, ~40 forks). Findings:

- **charles-dyfis-net/lensfun**: 1 star, 4001 commits (475 behind upstream). No visible code divergence; appears to be a personal mirror.
- **sysfce2/lensfun**: 4474 commits (2 behind). Arch/Manjaro packaging mirror, used by the `lensfun-git` AUR package. No divergence.
- **CAHEK7/lensfun**: 3863 commits. Adds a `.clang-tidy` config and codecov badge; contributor fork for the modernization PRs (#2169 clang-tidy, #2163 lfMount, #2176 STL). No shipped divergence.
- **payano/lensfun**: contributor fork for the smart-pointer PR (#2138).
- All other forks: 0 stars, 0 forks, personal clones tracking upstream. None ship a divergent build or a C++-only API.

Conclusion: there is no incumbent "better fork" to build on. A new fork that wants to diverge (e.g. C++-only API) would be starting fresh, with no existing community or downstream adoption to inherit.

## Missing considerations (from adversarial review)

- **`_lf_terminate_vec` reallocation hazard.** `reserve(size+1)` does not change `size()`; the NULL is written into capacity. Any `push_back`/`insert`/`erase`/`clear` without an immediate re-termination drops the NULL and silently breaks C consumers. No invariant test exists.
- **Ownership of `g_new`-allocated find results** in `database.cpp` is unspecified; callers cannot free them.
- **Deprecated struct members are live ABI.** `Mounts`, `CalibDistortion`, etc. cannot be removed without breaking darktable, lensfunpy, rawtherapee at the source level.
- **Hosting and integrity.** The data pipeline depends on `lensfun.github.io/db/` and `wilson.bronger.org/lensfun-db/`; `lensfun-update-data` fetches tarballs with no integrity verification mentioned. A fork hosting data needs a plan for both.
- **License/fork hygiene.** LGPL implications of a divergent fork and downstream build-system detection (pkg-config, CMake) are not addressed.
- **Test build hygiene.** `COMMON_LIBS` is undefined in `tests/CMakeLists.txt`; verify tests actually link before promising "behind tests."

## Changes made (item #1)

- `include/lensfun/lensfun.h.in`: replaced `PLATFORM_WINDOWS` with `_WIN32` at the two symbol-visibility sites (lines 84, 92). This makes the installed header compile for MinGW/MSVC consumers without requiring `config.h`.
- `tests/test_public_header.c` and `tests/test_public_header.cpp`: minimal compile smoke tests (C and C++) that include the public header and link lensfun, registered in `tests/CMakeLists.txt` as `Public_header_c` and `Public_header_cpp`.
- `.github/workflows/cmake-windows.yml`: added a "Consumer header smoke test" step that compiles a trivial C and C++ file against the installed header using MSVC (via vcvarsall).
- `.github/workflows/cmake-msys2.yml`: added `cmake --build build --target install` and a "Consumer header smoke test" step compiling against the installed header with gcc/g++.
- Verified: full build (`cmake --build`) succeeds (71/71); `ctest` 29/29 pass (including the 2 new header smoke tests); trivial consumer compiles as both C (`gcc -x c`) and C++ (`g++`) against the installed header with no "I don't know how to change symbol visibility" error; `check_database.py` passes.

Remaining for #1 (release management, requires upstream coordination): backport this to a 0.3.4.x stable branch and cut a patched release (not from master, which is 0.3.99 alpha). The code and CI changes above are complete.
## Triage of stalled modernization PRs

Reviewed the 6 open modernization PRs. None apply cleanly to current master without a rebase; all are 2+ years old except #2687.

- **#2176 "replace custom helper templates with C++ STL"** (Mar 2024, approved by CAHEK7). Replaces `clampd`/`absolute`/`square` with `std::clamp`/`std::hypot`/cmath. C++17 already required. Low risk, small diff. **Action: rebase and merge.**
- **#2163 "Modernize lfMount"** (Feb 2024, 13 commits). Converts `char*` to `std::string`, removes deprecated `lf_mount_new`, keeps a separate `MountCompatPtrs` vector for the C API, adds a C-API test. Establishes the pattern for keeping C compatibility while modernizing internals. Active review discussion. **Action: needs maintainer review; adopt its pattern for other classes.**
- **#2138 "Use smart pointers for lfLens Calibrations"** (Jan 2024). Changes `std::vector<lfLensCalibrationSet*>` to `shared_ptr`, which breaks contiguous memory for `GetCalibrationSets()` (returns `lfLensCalibrationSet**`). Contributor's workaround (a malloc'd C-API array kept in sync) adds complexity and CAHEK7 requested changes that were not resolved. **Action: defer until the ownership model is agreed; do not merge as-is.**
- **#2160 "Add C interface for EnableDistortionCorrection"** (Feb 2024). Small, additive C wrapper. **Action: review and merge if still applicable.**
- **#2687 "Modernize calibration tool"** (Feb 2026). Recent, touches `tools/calibrate`. **Action: review.**
- **#2169 "Add clang-tidy CI action"** (Feb 2024). Adds `.clang-tidy` (already present in CAHEK7's fork). **Action: low priority; nice-to-have.**

General recommendation: close or rebase anything that no longer applies rather than letting PRs rot. Do not batch them into one mega-merge.

## Where things live

- Public API: `include/lensfun/lensfun.h.in`
- Build config: `CMakeLists.txt`, `libs/lensfun/CMakeLists.txt`, `include/lensfun/config.h.in.cmake`
- Internals: `libs/lensfun/*.cpp`, `libs/lensfun/lensfunprv.h`
- Windows CI: `.github/workflows/cmake-windows.yml`, `.github/workflows/cmake-msys2.yml`
- Tests: `tests/` (plain executables using `g_assert_*`, run via `ctest`). Note: `COMMON_LIBS` is not defined in the current `tests/CMakeLists.txt`; verify tests link before relying on "behind tests" claims.
- Apps/tools: `apps/`, `tools/`

## Sources

- GitHub `lensfun/lensfun`: issues #2676, #2520, #2142, #1998, #2001, #1971, #1938, #1834; PRs #2176, #2163, #2160, #2138, #2687, #2169.
- Local files: `CMakeLists.txt`, `include/lensfun/lensfun.h.in`, `include/lensfun/config.h.in.cmake`, `libs/lensfun/lensfunprv.h`, `.github/workflows/cmake-windows.yml`, `.github/workflows/cmake-msys2.yml`.