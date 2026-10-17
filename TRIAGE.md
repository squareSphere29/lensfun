# Lensfun issue/PR triage

Scope: open issues and PRs on `lensfun/lensfun`, filtered to actionable code/behavior items. Stale data-submission requests (~30 "Calibration upload" issues) are omitted; recommend a batch close policy for those.

## Fixed in this fork

| Item | What | Commit |
|---|---|---|
| #2676 | Windows CMake build (`_WIN32` instead of `PLATFORM_WINDOWS`) | 315b344a |
| #2862 | `lf_db_new` ABI compat alias | 97eca42a |
| #2450 | Remove git from `generate_db.py` timestamp | 97eca42a |
| #1418 | License header LGPL-2 → LGPL-3 | 1fe20756 |
| #1062 | Duplicate camera search (uninitialized `Score`) | (local) |
| #945 / PR #2639 | Legacy distortion scaling flag | 8ac3b246 |
| PR #2176 (partial) | `square`/`absolute` → std equivalents | fa994a0c |

## Issues worth fixing (code/behavior)

| # | Title | Age | Status | Notes |
|---|---|---|---|---|
| #2052 | X100T misidentified as X-T100 | 3y3m | Actionable | Identification bug |
| #1896 | Viltrox 85mm not identified | 4y1m | Actionable | 22 comments |
| #1586 | Sigma 150-600 Contemporary not identified | 4y9m | Actionable | PR #1588 renames profile |
| #1876 | DB location priorities (Windows) | 4y2m | Actionable | User DB shadows system DB |
| #671 | Finding fisheyes fails | 9y2m | Stale | Search bug |
| #1285 | Crash in `ApplyColorModification()` | 5y11m | Stale | Runtime bug; no follow-up |
| #2496 | Debian packaging (use python directly) | 1y2m | Actionable | Packaging |
| #2743 | acm↔pa vignetting conversion formula | 7m | Discussion | Modeling gap |
| #2384 | Video mode crop factor | 1y10m | Discussion | Modeling gap |
| #1172 | Separate database repository | 6y5m | Discussion | Strategic; 28 comments |
| #1304 | Sony lens model | 5y11m | Discussion | EXIF/model identification |

## Pull requests

| # | Title | Age | Status | Notes |
|---|---|---|---|---|
| #2819 | [codex] Update LCP converter | 3m | Actionable | Many commits; verify count |
| #2687 | Modernize calibration tool | 8m | Actionable | Python 3.9+ |
| #2768 | Table-filter for lens list HTML | 5m | Actionable | Docs UX |
| #2160 | C interface for `EnableDistortionCorrection` | 2y8m | Actionable | Needs TCA/vignetting additions |
| #2169 | clang-tidy CI action | 2y7m | Actionable | Approved; closes #2168 |
| #2176 | STL helper templates | 2y7m | Partially applied (in fork) | square/absolute done |
| #2163 | Modernize lfMount | 2y8m | Actionable | Large; needs review |
| #2138 | Smart pointers for calibrations | 2y9m | Actionable | Needs rework (shared vs unique) |


## Cross-cutting blockers

- Release gap: master has fixes not in any tag; consumers blocked on a release (#2634, #1033, #2081, #963).
- API/compat: unified coordinates changed distortion scaling (#945), now addressed by PR #2639 (applied). ABI rename `lf_db_new`→`lf_db_create` broke consumers (#2862, fixed via compat alias).
- Modernization PRs (#2138, #2163, #2176) require C++17; downstream impact (darktable, packagers) must be assessed. Blocked on agreement in #2142/#2161.
- "Stable release" milestone selects 16 issues + 2 PRs — a ready-made priority filter.
- ~30 stale "Calibration upload" issues: recommend batch close policy.
- Strategic: #1172 (separate DB repo, 28 comments) needs a decision.

## Top recommendations (remaining)

1. **Cut a release** — highest impact; consumers blocked on a tag.
2. **Fix #1876** — DB location priority on Windows.
3. **Fix #671, #1285** — search/crash bugs.
4. **Merge #2687, #2819, #2768** — recent tooling/docs PRs.
5. **Modernize: #2160, #2169** — small, additive.
6. **Decide #1172, #2384, #2743** — strategic modeling gaps.

## Citations

- Issues: https://github.com/lensfun/lensfun/issues (88 open)
- Pull requests: https://github.com/lensfun/lensfun/pulls (18 open)
- Individual items cited above by number and linked inline.