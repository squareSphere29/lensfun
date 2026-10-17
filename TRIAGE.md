# Lensfun issue/PR triage (as of 2026-10-17)

Scope: all 88 open issues and 18 open PRs on `lensfun/lensfun`. Categorization is by primary topic. Age is time since opening. Status: **Actionable** (fixable/mergeable now), **Stale** (no meaningful activity, usually >3y or archived), **Discussion** (design/RFC, no concrete patch), **Data request** (calibration/profile submission).

## Summary counts

### Issues (88)

| Category | Count |
|---|---:|
| Data/database | 47 |
| API/ABI | 11 |
| CI/tooling | 12 |
| Build | 5 |
| C/C++ modernization | 3 |
| Docs | 3 |
| Other | 7 |
### Pull requests (18)

| Category | Count |
|---|---:|
| Data/database | 7 |
| CI/tooling | 4 |
| C/C++ modernization | 3 |
| API/ABI | 2 |
| Docs | 1 |
| Build | 1 |
| Other | 0 |

### Cross-cutting blockers

- Release gap: `master` contains Windows/build and scaling fixes not in any tag; many items blocked on cutting a release (#2634, #1033, #2081, #963).
- API/compat: unified internal coordinates changed distortion scaling (#945), addressed by PR #2639. ABI symbol `lf_db_new`→`lf_db_create` rename broke consumers (#2862).
- C/C++ modernization PRs (#2138, #2163, #2176) require C++17; downstream impact (darktable, packagers, older toolchains) must be assessed. Blocked on agreement in #2142/#2161.
- "Stable release" milestone already selects 16 issues + 2 PRs (#501-#509, #593, #944, #945, #1033, #1062, #1068, #1418; PRs #1151, #1351) — a ready-made priority filter.
- Calibration/data submissions dominate the backlog; most are stale. Recommend a batch triage/close policy.
- Strategic: #1172 (move database to separate repo) has 28 comments and would change data-backlog handling; needs a decision before more data PRs accumulate.
- Modeling gaps underlying many submissions: #2384 (video crop factor), #2743 (acm↔pa vignetting conversion formula).
- Modernization prerequisites: #507 (performance tests) and #501 (docs) are stale but needed before merging #2138/#2163/#2176.

---

## Issues by category

### Build

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2676](https://github.com/lensfun/lensfun/issues/2676) | CMake build for 0.3.4 broken on Windows | letmaik | 8m | Actionable | `PLATFORM_WINDOWS` removed; blocks lensfunpy; reportedly fixed in master (verify before release) |
| [#2110](https://github.com/lensfun/lensfun/issues/2110) | Xcode static lib compile error when calling lfCamera | Dedicatee | 3y1m | Stale | macOS/Xcode consumer build issue (not Windows) |
| [#1834](https://github.com/lensfun/lensfun/issues/1834) | Look for Python using find_package() | moubctez | 4y4m | Actionable | Patch provided; improves multi-Python builds |
| [#1535](https://github.com/lensfun/lensfun/issues/1535) | Missing scipy and numpy after install | robertl78 | 4y10m | Stale | Windows calibration environment support |
| [#692](https://github.com/lensfun/lensfun/issues/692) | cross compiling, static only, make install fails | bronger | 8y0m | Stale | Archived (sf-archive) |

### C/C++ modernization

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2161](https://github.com/lensfun/lensfun/issues/2161) | Get rid of legacy C code in C++ | CAHEK7 | 2y8m | Actionable | Umbrella; drives #2138/#2163/#2176 |
| [#2142](https://github.com/lensfun/lensfun/issues/2142) | Try to separate the C and C++ | payano | 2y9m | Discussion | Architecture; agreement needed before merges |
| [#1264](https://github.com/lensfun/lensfun/issues/1264) | Build without glib | WindowsNT | 6y0m | Stale | Stdlib replacement; overlaps modernization |

### Data/database

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2863](https://github.com/lensfun/lensfun/issues/2863) | Calibration upload eb2693 | bronger | 1m | Data request | Calibration upload |
| [#2743](https://github.com/lensfun/lensfun/issues/2743) | Formula for converting between vignetting models acm and pa | flux77 | 7m | Discussion | Needs documented conversion formula |
| [#2541](https://github.com/lensfun/lensfun/issues/2541) | Google Pixel 9 Pro | gghose | 1y1m | Data request | Phone profile |
| [#2384](https://github.com/lensfun/lensfun/issues/2384) | Video mode crop factor | peci1 | 1y10m | Discussion | Variant/crop-factor modeling |
| [#2181](https://github.com/lensfun/lensfun/issues/2181) | Calibration data for vignette, Xiaomi 2201116PG | acamari | 2y7m | Data request | Stale submission |
| [#2052](https://github.com/lensfun/lensfun/issues/2052) | Lensfun recognizes X100T as X-T100 | 4kbt | 3y3m | Actionable | Misidentification bug |
| [#1972](https://github.com/lensfun/lensfun/issues/1972) | Calibration upload 4d17a5 | bronger | 3y9m | Data request | Stale |
| [#1948](https://github.com/lensfun/lensfun/issues/1948) | OnePlus 10 Pro 5G lens corrections | lintujuh | 3y10m | Data request | Phone |
| [#1896](https://github.com/lensfun/lensfun/issues/1896) | Viltrox 85mm not identified properly | MRedbook | 4y1m | Actionable | 22 comments; identification bug, not tied to PR #1588 |
| [#1876](https://github.com/lensfun/lensfun/issues/1876) | Database location priorities (Windows) | lintujuh | 4y2m | Actionable | User DB shadows system DB |
| [#1851](https://github.com/lensfun/lensfun/issues/1851) | Calibration upload c104c3 | bronger | 4y3m | Data request | Stale |
| [#1778](https://github.com/lensfun/lensfun/issues/1778) | Calibration upload 50636a | bronger | 4y6m | Data request | Stale |
| [#1776](https://github.com/lensfun/lensfun/issues/1776) | Calibration upload 3ee2ff | bronger | 4y6m | Data request | Stale |
| [#1686](https://github.com/lensfun/lensfun/issues/1686) | Calibration upload 1f125f | bronger | 4y7m | Data request | Stale |
| [#1680](https://github.com/lensfun/lensfun/issues/1680) | Calibration data Sony 70-400mm | zemumu | 4y7m | Data request | Stale |
| [#1586](https://github.com/lensfun/lensfun/issues/1586) | Sigma 150-600 Contemporary not identified | benklop | 4y9m | Actionable | PR #1588 renames profile |
| [#1464](https://github.com/lensfun/lensfun/issues/1464) | Samyang 8mm CSII params for Canon EF | electroCutie | 5y2m | Data request | Stale |
| [#1439](https://github.com/lensfun/lensfun/issues/1439) | Insta360 One R support | jonathancolledge | 5y3m | Data request | Stale |
| [#1378](https://github.com/lensfun/lensfun/issues/1378) | Calibration upload bf425a | bronger | 5y7m | Data request | Stale |
| [#1367](https://github.com/lensfun/lensfun/issues/1367) | Calibration upload a0d21e | bronger | 5y7m | Data request | Stale |
| [#1346](https://github.com/lensfun/lensfun/issues/1346) | Calibration upload 16537c | bronger | 5y9m | Data request | Stale |
| [#1304](https://github.com/lensfun/lensfun/issues/1304) | Sony Lens Model | FreddieWitherden | 5y11m | Discussion | EXIF/model identification |
| [#1282](https://github.com/lensfun/lensfun/issues/1282) | OnePlus 6 vignetting raw file | PeterWem | 5y12m | Data request | Stale |
| [#1181](https://github.com/lensfun/lensfun/issues/1181) | Converting lcp to xml, DJI Spark | Kusine | 6y4m | Data request | Stale |
| [#1172](https://github.com/lensfun/lensfun/issues/1172) | Moving database into separate repository? | eszlari | 6y5m | Discussion | Strategic; 28 comments |
| [#1160](https://github.com/lensfun/lensfun/issues/1160) | Calibration upload 1cf7b0 | bronger | 6y5m | Data request | Stale |
| [#1127](https://github.com/lensfun/lensfun/issues/1127) | Calibration upload 24fd4e | bronger | 6y7m | Data request | Stale |
| [#1117](https://github.com/lensfun/lensfun/issues/1117) | Calibration upload f0cc69 | bronger | 6y7m | Data request | Stale |
| [#1076](https://github.com/lensfun/lensfun/issues/1076) | Vignetting Samyang 85mm F1.8 | gscholz | 6y9m | Data request | Stale |
| [#1062](https://github.com/lensfun/lensfun/issues/1062) | Duplicate camera search results | letmaik | 6y9m | Actionable | Stable release milestone |
| [#1057](https://github.com/lensfun/lensfun/issues/1057) | Calibration data Samyang 14mm | OSteinbach | 6y10m | Data request | Stale |
| [#1045](https://github.com/lensfun/lensfun/issues/1045) | Calibration upload 3a4dbf | bronger | 6y10m | Data request | Stale |
| [#1024](https://github.com/lensfun/lensfun/issues/1024) | Canon EF 35-80 distortion calibration | edsvo | 6y11m | Data request | Stale |
| [#1005](https://github.com/lensfun/lensfun/issues/1005) | Calibration upload 2c4c76 | bronger | 7y0m | Data request | Stale |
| [#988](https://github.com/lensfun/lensfun/issues/988) | Calibration upload 2fbde2 | bronger | 7y1m | Data request | Stale |
| [#977](https://github.com/lensfun/lensfun/issues/977) | Calibration upload cdc37f | bronger | 7y1m | Data request | Stale |
| [#930](https://github.com/lensfun/lensfun/issues/930) | Olympus M.Zuiko 40-150mm f/2.8 Pro | wwwFrank | 7y3m | Data request | Stale |
| [#927](https://github.com/lensfun/lensfun/issues/927) | Canon EOS RP should be supported | jmtilli | 7y3m | Data request | Stale |
| [#898](https://github.com/lensfun/lensfun/issues/898) | Calibration upload 00fcde | bronger | 7y4m | Data request | Stale |
| [#894](https://github.com/lensfun/lensfun/issues/894) | Samyang 300mm f/6.3 calibration | aarchiba | 7y4m | Data request | Stale |
| [#866](https://github.com/lensfun/lensfun/issues/866) | Tokina AF 11-16mm | Nadeox1 | 7y5m | Data request | Stale |
| [#863](https://github.com/lensfun/lensfun/issues/863) | Honor 6A vignetting new profile | falket | 7y5m | Data request | Stale |
| [#700](https://github.com/lensfun/lensfun/issues/700) | Sony E PZ 16-50 OSS vignetting | bronger | 7y9m | Data request | Archived |
| [#832](https://github.com/lensfun/lensfun/issues/832) | Zeiss/Fuji/Canon/Sigma distortion info | bronger | 8y4m | Data request | Archived |
| [#671](https://github.com/lensfun/lensfun/issues/671) | Finding fisheyes fails | bronger | 9y2m | Stale | Search bug |
| [#724](https://github.com/lensfun/lensfun/issues/724) | Move aspect-ratio/cropfactor into calibration | bronger | 11y9m | Stale | Feature, archived |
| [#509](https://github.com/lensfun/lensfun/issues/509) | Canon EF-S 60mm calibration | MarioHenze | 7y6m | Data request | Stable release milestone |
### API/ABI

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2862](https://github.com/lensfun/lensfun/issues/2862) | undefined symbol in consumer: lf_db_new | kiefernforst | 1m | Actionable | Breaks GIMP/gegl; `lf_db_new`→`lf_db_create` rename broke ABI; decide compat alias vs. document rename |
| [#1329](https://github.com/lensfun/lensfun/issues/1329) | module 'lensfun' has no attribute system_db_update_path | dimk1 | 5y10m | Stale | Maintainer says likely not relevant |
| [#1292](https://github.com/lensfun/lensfun/issues/1292) | Should distortion admit distance parameter | FreddieWitherden | 5y11m | Discussion | API extension |
| [#984](https://github.com/lensfun/lensfun/issues/984) | RFC UniqueCameraModel for dual-camera phones | junkyardsparkle | 7y1m | Discussion | API/identification |
| [#945](https://github.com/lensfun/lensfun/issues/945) | Scaling factor API changes break edits | jakubfi | 7y2m | Actionable | High impact; compat fix in PR #2639 |
| [#508](https://github.com/lensfun/lensfun/issues/508) | Test new API in application context | seebk | 7y7m | Stale | Discuss |
| [#506](https://github.com/lensfun/lensfun/issues/506) | How to flag enabled modifications? | seebk | 7y7m | Stale | Discuss |
| [#505](https://github.com/lensfun/lensfun/issues/505) | Should LF_MODIFY_GEOMETRY be LF_MODIFY_PROJECTION? | seebk | 7y7m | Stale | Discuss |
| [#504](https://github.com/lensfun/lensfun/issues/504) | Decide next version number and backwards compat | seebk | 7y7m | Stale | Blocks API decisions |
| [#503](https://github.com/lensfun/lensfun/issues/503) | Retrieve calibration data from lfLensCalibrationSet | seebk | 7y7m | Stale | Feature |
| [#502](https://github.com/lensfun/lensfun/issues/502) | Check if C-interface up to date | seebk | 7y7m | Stale | PR #2160 partial |

### CI/tooling

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2587](https://github.com/lensfun/lensfun/issues/2587) | No py3exiv2 for Ubuntu LTS 24.04? | mawob | 1y0m | Stale | Calibration tool dependency |
| [#2496](https://github.com/lensfun/lensfun/issues/2496) | debian packaging should use python directly | wgoetz | 1y2m | Actionable | Packaging fix |
| [#2450](https://github.com/lensfun/lensfun/issues/2450) | git timestamp in profile conversion breaks CI | paperdigits | 1y6m | Actionable | Remove git dependency |
| [#2351](https://github.com/lensfun/lensfun/issues/2351) | setuptools does not like me | rubyFeedback | 1y11m | Stale | Build environment support |
| [#2168](https://github.com/lensfun/lensfun/issues/2168) | Setup clang-tidy and clang-format CI | CAHEK7 | 2y7m | Actionable | PR #2169 open |
| [#2164](https://github.com/lensfun/lensfun/issues/2164) | Enable code coverage | payano | 2y8m | Stale | CI enhancement |
| [#1409](https://github.com/lensfun/lensfun/issues/1409) | convert-lcp acm to ptlens | etlweather | 5y5m | Discussion | Converter capability |
| [#1289](https://github.com/lensfun/lensfun/issues/1289) | lensfun-convert-lcp is broken | hatsunearu | 5y11m | Actionable | PR #2819 updates converter |
| [#593](https://github.com/lensfun/lensfun/issues/593) | lensfun-update-data breaks added adapters | Computerfreaked | 7y6m | Stale | Stable release milestone |
| [#588](https://github.com/lensfun/lensfun/issues/588) | Setup continuous integration services | seebk | 7y6m | Stale | Superseded by GitHub Actions |
| [#507](https://github.com/lensfun/lensfun/issues/507) | Add performance tests | seebk | 7y7m | Stale | Enhancement |
| [#656](https://github.com/lensfun/lensfun/issues/656) | lensfun rpm creates dirs owned by filesystem | bronger | 9y11m | Stale | Packaging, archived |

### Docs

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#1068](https://github.com/lensfun/lensfun/issues/1068) | Update changelog with added/modified profiles | seebk | 6y9m | Stale | Maintenance |
| [#501](https://github.com/lensfun/lensfun/issues/501) | Update Doxygen comments and documentation | seebk | 7y7m | Stale | Stable release milestone |
| [#1418](https://github.com/lensfun/lensfun/issues/1418) | Conflicting license in lensfun.h | SRHMorris | 5y5m | Actionable | License header mismatch |

### Other

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2634](https://github.com/lensfun/lensfun/issues/2634) | Any plans to cut a new release tag? | mamccorm | 11m | Actionable | Release request; blocks consumers |
| [#2081](https://github.com/lensfun/lensfun/issues/2081) | some labeling issue with 0.3.4 release | chenrui333 | 3y2m | Stale | Release packaging |
| [#1285](https://github.com/lensfun/lensfun/issues/1285) | Crash in ApplyColorModification() | WindowsNT | 5y11m | Stale | Runtime bug |
| [#1142](https://github.com/lensfun/lensfun/issues/1142) | Darktable with lensfun-master | uchrisu | 6y6m | Stale | Integration help wanted |
| [#1033](https://github.com/lensfun/lensfun/issues/1033) | Any plans to make new release? | kloczek | 6y11m | Stale | Duplicate release request |
| [#963](https://github.com/lensfun/lensfun/issues/963) | When will dev branch become official release? | githubfredk | 7y1m | Stale | Release request |
| [#944](https://github.com/lensfun/lensfun/issues/944) | Lenstool.cpp incorrect ordering if no COMBINE_13 | gatherto | 7y3m | Stale | Sample code |

---

## Pull requests by category

### Data/database

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2875](https://github.com/lensfun/lensfun/pull/2875) | Add lens data Canon EF 55-200mm II USM | andreytsar | 0m | Actionable | New profile |
| [#2781](https://github.com/lensfun/lensfun/pull/2781) | Add Canon MP-E 65mm 1-5x Macro | byteit101 | 5m | Actionable | New profile |
| [#2408](https://github.com/lensfun/lensfun/pull/2408) | Add 7Artisans APS-C 25mm f/1.8 | ZbyszekK | 1y8m | Stale | Needs review |
| [#2004](https://github.com/lensfun/lensfun/pull/2004) | Add vignetting Samyang 12mm AF | interru | 3y7m | Stale | Needs review |
| [#1588](https://github.com/lensfun/lensfun/pull/1588) | corrected name sigma 150-600 contemporary | benklop | 4y9m | Actionable | Closes #1586 |
| [#1151](https://github.com/lensfun/lensfun/pull/1151) | Give User-Profiles always priority | uchrisu | 6y5m | Stale | DB priority logic |
| [#1039](https://github.com/lensfun/lensfun/pull/1039) | Another name for Sigma 18-300mm | goddisignz | 6y11m | Stale | Profile alias |

### C/C++ modernization

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2176](https://github.com/lensfun/lensfun/pull/2176) | replace custom helper templates with C++ STL | jonas-ott | 2y7m | Actionable | Approved; requires C++17 |
| [#2163](https://github.com/lensfun/lensfun/pull/2163) | Modernize lfMount | CAHEK7 | 2y8m | Actionable | Large; in progress |
| [#2138](https://github.com/lensfun/lensfun/pull/2138) | Use smart pointers for lfLens Calibrations | payano | 2y9m | Actionable | Needs rework (shared vs unique) |

### API/ABI

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2639](https://github.com/lensfun/lensfun/pull/2639) | Add flag to reproduce legacy distortion scaling | seebk | 11m | Actionable | Addresses #945; needs review/merge |
| [#2160](https://github.com/lensfun/lensfun/pull/2160) | Add C interface for EnableDistortionCorrection | CAHEK7 | 2y8m | Actionable | Needs TCA/vignetting additions |

### CI/tooling

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2819](https://github.com/lensfun/lensfun/pull/2819) | [codex] Update LCP converter | shenmintao | 3m | Actionable | Many commits; verify count; needs maintainer review |
| [#2687](https://github.com/lensfun/lensfun/pull/2687) | Modernize calibration tool | yuri1969 | 8m | Actionable | Python 3.9+ |
| [#2169](https://github.com/lensfun/lensfun/pull/2169) | Add clang-tidy CI action | CAHEK7 | 2y7m | Actionable | Approved; closes #2168 |
| [#1166](https://github.com/lensfun/lensfun/pull/1166) | Make plots more readable | uchrisu | 6y5m | Stale | Calibration tooling |

### Docs

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#2768](https://github.com/lensfun/lensfun/pull/2768) | Add table-filter for generated lens list HTML | byteit101 | 5m | Actionable | Docs UX |

### Build

| # | Title | Author | Age | Status | Notes |
|---|---|---|---|---|---|
| [#1351](https://github.com/lensfun/lensfun/pull/1351) | Match library file name with soname | mikhailnov | 5y8m | Stale | Packaging/naming |

---

## Top 5 recommendations (revised per adversarial review)

1. **Cut a release** (issues #2634, #1033, #2081, #963; includes #2676, #2639). Rationale: master already fixes the Windows build and scaling compatibility; consumers (lensfunpy, darktable, GIMP) are blocked on a tag. Highest overall impact. Note: 0.3.5 does not exist yet; cut from a stable base.
2. **Merge PR #2639** (legacy distortion scaling flag) to resolve #945. Rationale: fixes backwards-incompatible image scaling for all RAW processors using POLY3/PTLENS models; maintainer-authored. Note: default behavior stays new scaling; consumers must opt in via the flag, so release notes must define the migration. Add test/ABI verification.
3. **Fix #2862** (`lf_db_new` undefined symbol). Rationale: breaks GIMP/gegl at runtime; `lf_db_new`→`lf_db_create` rename broke ABI; decide compat alias vs. document rename. Higher priority than #2450 (runtime load failure vs. build reproducibility).
4. **Fix #2676** (Windows CMake build) — verify master status first. Rationale: unblocks Windows Python bindings and CI; concrete, small fix. Reportedly fixed in master; confirm before release.
5. **Fix #2450** (remove git from `generate_db.py` timestamp). Rationale: unblocks reproducible NixOS/flatpak builds; small, well-scoped change.

## Citations

- Issues: https://github.com/lensfun/lensfun/issues (88 open)
- Pull requests: https://github.com/lensfun/lensfun/pulls (18 open)
- Individual items cited above by number and linked inline.
