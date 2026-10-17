# Design sketch: candidate 8 — public header C/C++ + ABI entanglement

Goal: make the 25 deprecated struct members (`Mounts`, `CalibDistortion`, `CalibTCA`, `CalibVignetting`, `CalibCrop`, `CalibFov`, `CropFactor`, `AspectRatio`) **derived views** of the modern stores, so there is a single source of truth and the legacy pointers can no longer drift.

## Constraints (from `why` / issue #2142)
- ABI: C consumers (darktable, lensfunpy, rawtherapee, ffmpeg) read these fields directly. They cannot be removed without a SONAME bump + downstream porting. They must remain as stored members.
- C++ consumers use the modern members (`MountNames`, `Calibrations`).
- `Save()` in `database.cpp` reads `m->Compat` and `l->Calib*` — these must keep working.
- PR #2163's pattern (separate C-API pointer vector refreshed in mutators) is the proven incremental approach.

## Current state
- `lfMount`: `MountCompat` (std::vector<char*>) + `Compat` (char**). Synced in `AddCompat` only. Copy ctor/assignment duplicated.
- `lfLens`: `Calibrations` vector + deprecated `Calib*` pointers + `CropFactor`/`AspectRatio`. Synced in `AddCalib*` via `UpdateLegacyCalibPointers()` (only for `Calibrations[0]`). `RemoveCalibrations()` did not sync (fixed). `GetCalibrationSetForAttributes` unconditionally used `Calibrations[0]` (dead code below removed).
- `lfCamera`: `CropFactor` (no deprecated list pointer). Simple.

## Design: derived-view accessors
Keep the deprecated fields as stored members (ABI), but make their values a **read-time projection** of the modern store, not a manually-synced copy.

1. **`lfMount`**: remove `Compat` from being written in `AddCompat`. Instead, `GetCompats()` already returns `MountCompat.data()` (done). Make `Compat` a lazily-refreshed cache: refresh in `GetCompats()` (const_cast terminate) and in copy/assign. Save uses `GetCompats()` (done). Eventually `Compat` becomes write-never, read-via-accessor. For ABI, keep the field but mark "do not read directly; use GetCompats()".

2. **`lfLens`**: 
   - `CropFactor`/`AspectRatio`: sync only in `GetCalibrationSets()`/read accessors, not in every AddCalib*. Actually they're already synced in `GetCalibrationSetForAttributes`. Keep that. Additionally, expose `GetCropFactor()`/`GetAspectRatio()` that read from `Calibrations[0]->Attributes` when available, falling back to the legacy field. Consumers can migrate to the accessor.
   - `Calib*` pointers: already refreshed in `UpdateLegacyCalibPointers()` (called from AddCalib*). Keep. Add a `RefreshLegacyPointers()` (rename of UpdateLegacyCalibPointers) called also from copy/assign and GetCalibrationSets.

3. **Single sync chokepoint**: introduce `lfLens::SyncLegacyViews()` and `lfMount::SyncLegacyViews()` that refresh all deprecated fields from the modern store. Call from every read accessor (GetMounts, GetCameras, GetLenses, GetCompats, GetMountNames, GetCalibrationSets) and from copy/assign. Mutators only touch the modern store.

4. **Invariant tests**: 
   - After any sequence of Add*/Remove*, assert `Compat == MountCompat.data()` (or NULL when empty) and `CalibDistortion == Calibrations[0]->CalibDistortion.data()`.
   - Add to `test_public_header` style smoke tests for consumer compile.

## Migration path (no ABI break)
- Phase 1 (done): defensive termination in accessors; RemoveCalibrations fix; Save uses GetCompats.
- Phase 2: introduce `SyncLegacyViews()` chokepoint; remove sync calls from mutators (AddCompat, AddCalib*). Accessors call SyncLegacyViews.
- Phase 3: mark deprecated members more strongly (doc: "derived view, do not write"). Add accessors (`GetCropFactor`, etc.) for C++ consumers.
- Phase 4 (future, SONAME bump): remove the deprecated members entirely once all consumers (darktable, lensfunpy, rawtherapee) migrate. Provide a `lensfun-c` compat shim.

## Files to touch
- `lensfun.h.in`: add accessor declarations; doc comments on deprecated members.
- `lens.cpp`, `mount.cpp`, `database.cpp`: add SyncLegacyViews; call from accessors; remove sync from mutators.
- `lensfunprv.h`: declare SyncLegacyViews (or keep in class).
- tests: add invariant tests.

## Risk
- Medium. The sync-at-read approach changes timing (legacy pointers refreshed on every read) but not values. Must verify no read occurs during mutation (reentrancy). Tests cover this.

## Not in scope
- Removing the deprecated members (ABI break, separate phase).
- GLib removal (separate effort).
