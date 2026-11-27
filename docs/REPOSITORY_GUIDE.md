# Repository audit and reorganisation plan

[🇪🇸 Versión en español](./REPOSITORY_GUIDE.es.md) · [Back to README](../README.md#en)

This plan records the **actual repository contents checked on 9 November 2026**. We will not move, rename or delete the team's existing materials until we verify them.

## Existing root directories

| Current location | Verified contents | Improvement |
|---|---|---|
| `3D DESIGN/` | Bookmark STL, bookmark merchandise STL, alphabet game STL, keychain PNG, station PNG | Organise models and previews by device |
| `DOCUMENTS/` | Project report PDF, style guide PDF, presentation script PDF, a duplicate README | Keep reports and remove README duplication only after review |
| `ELECTRIC  DIAGRAM AND CODING/CODING/` | Three **PNG screenshots** (`CODE 1.PNG`, `CODE 2.PNG`, `CODE 3.PNG`) | Add the actual editable `.ino`/`.py` source code |
| `ELECTRIC  DIAGRAM AND CODING/FRITZING/` | PNG wiring diagrams for bookmark, station, wireless charger and display | Keep diagrams, add verified pinout tables |
| `ELECTRIC  DIAGRAM AND CODING/SKETCHES/` | Component and measurement images | Group by module |
| `IMAGES/` | APP, ENTREPRENEURSHIP, NEXT, PROBLEMS, RRSS, TEAM PHOTOS | Continue using and credit existing artwork |

Two image-path problems were found in the old root README: it pointed to a Canva banner URL and to three photos in `IMAGES/` that are not at that path. The real logo is at `IMAGES/NEXT/NEXT logo.png` and the team photo is at `IMAGES/TEAM PHOTOS/TEAM PHOTO.jpg`. The revised root README uses these existing assets.

There is also an additional `DOCUMENTS/README.md` that differs from the root README, including different team-role descriptions. The published root README intentionally omits role assignments until the team reconciles them.

## Proposed future folder layout

```text
NEXT-PAGE/
├── README.md
├── bookmark/
│   ├── firmware/
│   └── hardware/
├── station/
│   ├── next_mmwave_uart.py
│   ├── next_station_api.py
│   ├── static/
│   └── requirements.txt
├── mobile-app/
├── 3d-design/
├── docs/
│   ├── installation/
│   ├── electronics/
│   └── testing/
├── assets/
└── .gitignore
```

This is the **target structure**, not a claim that these directories or files already exist.

## Safe migration sequence

1. **Presentation:** improve root `README.md` and add documentation without altering working files.
2. **Inventory:** list the exact purpose and latest version of every original file.
3. **Source preservation:** copy the **actual tested** Python and microcontroller programs from Station 1 and the smart bookmark. Do not substitute draft code from documentation.
4. **Organise:** move files by module only after owners and references are verified. Use Git history and adjust Markdown links and Python working directories.
5. **Reproducibility:** test the installation on Station 2 with the hardware attached and record test results.
6. **Release:** tag an independently tested version before the competition.

## Quality standards

- Use accurate words: *working*, *tested*, *experimental*, *planned*.
- Never claim the SEN0609 directly measures emotional enjoyment or concentration.
- Keep secrets, Wi-Fi credentials, Spotify tokens, generated JSON and `.venv` out of Git.
- Publish editable text source files instead of only screenshots.
- Decide software/CAD/media licences before permitting redistribution.
- Document screen and board revisions and local Python/Raspberry Pi OS versions.
