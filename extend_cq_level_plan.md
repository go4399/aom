# Plan: Extend cq-level Range from [0, 63] to [0, 80]

## Context

In AOM_Q (constant quality) mode, the user-facing `cq-level` parameter (0-63) maps to internal `qindex` values (0-255) via a lookup table. Due to quality boost multipliers applied per hierarchical layer (KF/ARF via qstep_ratio, intermediate ARF via midpoint interpolation), cq-level=63 only produces qindex ~173 for key frames. The full qindex range [0, 255] is unreachable for boosted frame types.

Extending cq-level to 80 allows all frame types -- including key frames -- to reach qindex 255, giving the encoder full quantizer coverage across all hierarchical layers.

This feature is only needed in constant quality mode (`end-usage=q`). Bitrate control modes (CBR, VBR) are unaffected.

## Scope

The extension must work regardless of the `use-fixed-qp-offsets` setting:
- **`use-fixed-qp-offsets=1`**: Uses qstep_ratio approach in `encoder_utils.c` — covered in Part 1
- **`use-fixed-qp-offsets=0`**: Uses polynomial minq tables / `av1_compute_qdelta` in `ratectrl.c` — covered in Part 2

The behavior differs between the two paths because they use different formulas to derive per-layer qindex from the base cq-level.

---

## Part 1: Fixed QP Offsets Path (`use-fixed-qp-offsets=1`)

### Approach: Virtual qindex > 255 with Linear qstep_ratio Ramp

Store unclamped "virtual" qindex values (259–323) in `oxcf->cq_level` for cq-levels 64–80.

For the `use_fixed_qp_offsets=1` code path in `encoder_utils.c`, a **unified qstep_ratio approach** is used across the entire cq-level range:

- **cq 0–63** (qindex 0–255): `qstep_ratio = 0.2 + (1.0 - awq/255) * qratio_grad` — ratio decreases from 0.5 to 0.2 as quality decreases (existing formula, unchanged)
- **cq 64–80** (virtual qindex 259–323): `qstep_ratio = 0.2 + (cq_level - 255) / (323 - 255) * 0.8` — ratio increases from 0.2 toward 1.0, so KF/ARF qindex converges to MAXQ

At cq=63, the ratio reaches 0.2 (minimum boost). For cq 64–80, the ratio smoothly ramps back toward 1.0, meaning KF/ARF qindex approaches the leaf qindex (255). At cq=80, all layers converge to qindex 255.

When `qstep_ratio >= 1.0` (which occurs at cq=80), the `av1_get_q_index_from_qstep_ratio()` search is bypassed and `min(cq_level, MAXQ)` is returned directly, ensuring exact convergence to qindex 255 without off-by-1 rounding from the DC quantizer lookup.

### Verified qindex mapping (10-bit, `use-fixed-qp-offsets=1`)

Values verified against actual encoder output. At cq <= 79, KF/ARF shows off-by-1 from predicted values due to DC quantizer rounding in `av1_get_q_index_from_qstep_ratio()`; IARF and Leaf are exact. At cq=80, the `qstep_ratio >= 1.0` shortcut ensures all layers reach exactly 255.

| cq | qindex | KF/ARF | IARF_d2 | IARF_d3 | IARF_d4 | Leaf |
|----|--------|--------|---------|---------|---------|------|
| 55 | 220    | 122    | 171     | 196     | 208     | 220  |
| 60 | 240    | 142    | 191     | 216     | 228     | 240  |
| 63 | 255    | 172    | 214     | 235     | 245     | 255  |
| 64 | 259    | 187    | 221     | 238     | 247     | 255  |
| 66 | 267    | 212    | 234     | 245     | 250     | 255  |
| 68 | 275    | 227    | 241     | 248     | 252     | 255  |
| 70 | 283    | 235    | 245     | 250     | 253     | 255  |
| 72 | 291    | 242    | 249     | 252     | 254     | 255  |
| 74 | 299    | 247    | 251     | 253     | 254     | 255  |
| 76 | 307    | 250    | 253     | 254     | 255     | 255  |
| 78 | 315    | 253    | 254     | 255     | 255     | 255  |
| 79 | 319    | 254    | 255     | 255     | 255     | 255  |
| 80 | 323    | 255    | 255     | 255     | 255     | 255  |

### cq-level to qindex plot (fixed QP)

![cq-level to qindex mapping — use-fixed-qp-offsets=1](cq_to_qindex_fixed.svg)

### Changes for Fixed QP Path

#### 1. Extend `quantizer_to_qindex` table and reverse function
**File:** `av1/encoder/av1_quantize.c`

- Update comment: "0-63" to "0-80"
- Add 17 entries continuing from 255 with step +4 (unclamped virtual values):
  ```c
  259, 263, 267, 271, 275, 279, 283, 287, 291,
  295, 299, 303, 307, 311, 315, 319, 323,
  ```
- `av1_qindex_to_quantizer()`: change loop bound `< 64` to `< 81`, fallback `return 63` to `return 80`
- Total table size: 81 entries (cq 0–80)

#### 2. Clamp `best_allowed_q` and `worst_allowed_q`
**File:** `av1/av1_cx_iface.c`

`best_allowed_q` and `worst_allowed_q` feed directly into quantization and must stay in [0, 255]. `cq_level` is only used in rate control delta math and can exceed 255:
```c
oxcf->best_allowed_q =
    extra_cfg->lossless ? 0 : AOMMIN(av1_quantizer_to_qindex(cfg->rc_min_quantizer), MAXQ);
oxcf->worst_allowed_q =
    extra_cfg->lossless ? 0 : AOMMIN(av1_quantizer_to_qindex(cfg->rc_max_quantizer), MAXQ);
oxcf->cq_level = av1_quantizer_to_qindex(extra_cfg->cq_level);  // can be > 255
```

#### 3. Guard `av1_convert_qindex_to_q()` against qindex > 255
**File:** `av1/encoder/ratectrl.c`

Add clamp at the top of the function so `ac_qlookup[]` is not accessed out of bounds:
```c
double av1_convert_qindex_to_q(int qindex, aom_bit_depth_t bit_depth) {
  qindex = AOMMIN(qindex, MAXQ);  // clamp to valid ac_qlookup range
  // ... rest unchanged
}
```

#### 4. Update range checks
**File:** `av1/av1_cx_iface.c`

| Location | Current | New | Notes |
|----------|---------|-----|-------|
| `RANGE_CHECK_HI(extra_cfg, cq_level, 63)` | 63 | 80 | Allow extended cq-level |

**Not changed** (decision deferred):
- `RANGE_CHECK_HI(cfg, rc_max_quantizer, 63)` — stays at 63 (used by CBR/VBR)
- `63, // rc_max_quantizer` default — stays at 63

#### 5. Update fixed QP offsets formula for extended range
**File:** `av1/encoder/encoder_utils.c`

The existing code already has the extended range branch. Update `max_virtual_qindex` from 327 to 323, and add a `qstep_ratio >= 1.0` shortcut to bypass `av1_get_q_index_from_qstep_ratio()` when the ratio means no boost:
```c
if (cq_level <= MAXQ) {
    qstep_ratio = 0.2 + (1.0 - (double)cpi->rc.active_worst_quality / MAXQ) * qratio_grad;
} else {
    const int max_virtual_qindex = 323;  // quantizer_to_qindex[80]
    qstep_ratio = 0.2 + (double)(cq_level - MAXQ) / (max_virtual_qindex - MAXQ) * 0.8;
}
if (qstep_ratio >= 1.0)
    *q = AOMMIN(cq_level, MAXQ);
else
    *q = av1_get_q_index_from_qstep_ratio(
        cpi->rc.active_worst_quality, qstep_ratio,
        cm->seq_params->bit_depth);
```

**Why the shortcut is needed:** `av1_get_q_index_from_qstep_ratio()` searches downward from `leaf_qindex` for a qindex whose DC qstep satisfies `qstep + 0.1 <= target_qstep`. When `qstep_ratio = 1.0`, `target_qstep = leaf_qstep`, so `leaf_qstep + 0.1 <= leaf_qstep` is false and the search drops to `leaf_qindex - 1` (254 instead of 255). The shortcut avoids this off-by-1 at the boundary, ensuring all layers converge to exactly 255 at cq=80.

The shortcut only activates at cq=80 where the ratio reaches exactly 1.0. For all other cq levels (0–79), the ratio is < 1.0 and the normal search path is used.

#### 6. No changes needed
- `aomenc.c`: CLI option definitions have no hardcoded range — validation is in `av1_cx_iface.c`
- `warnings.c`: `check_quantizer()` only warns about narrow Q range
- `QINDEX_RANGE`, `MAXQ`, `ac_qlookup[]`: Internal qindex space [0, 255] is unchanged
- Rate control minq tables: Indexed by qindex [0, 255], unchanged
- Existing `clamp()` calls in `rc_pick_q_and_bounds_*`: Already clamp final q to `[rc->best_quality, rc->worst_quality]` = [0, 255]

---

## Part 2: Non-Fixed QP Path (`use-fixed-qp-offsets=0`)

### How it works

In AOM_Q mode without fixed QP offsets, per-layer qindex is derived through these code paths:

| Frame Type | Dispatch | Formula |
|------------|----------|---------|
| **KF** (first) | `rc_pick_q_and_bounds_no_stats` | `cq_level + av1_compute_qdelta(q_val, q_val * 0.25)` |
| **KF** (subsequent) | `rc_pick_q_and_bounds_q_mode` → `get_intra_q_and_bounds` | `get_kf_active_quality(active_worst_quality)` (state-dependent) |
| **ARF/GF** | `rc_pick_q_and_bounds_q_mode` → `get_active_best_quality` | `cq_level + av1_compute_qdelta(q_val, q_val * 0.40)` (after fix) |
| **IARF** | `rc_pick_q_and_bounds_q_mode` → `get_active_best_quality` | Midpoint interpolation from `arf_q` toward `min(cq_level, 255)` |
| **Leaf/Overlay** | `rc_pick_q_and_bounds_q_mode` → `get_active_best_quality` | `return cq_level` directly |

### Natural extension behavior

For extended cq > 63, the `av1_compute_qdelta` approach produces a **naturally smooth linear ramp** without any special formula:

1. `q_val = av1_convert_qindex_to_q(cq_level)` clamps to MAXQ=255, so `q_val` is constant (457.0 for 10-bit)
2. The delta is therefore constant: -47 for ARF (factor 0.40)
3. `active_best = clamp(cq_level + delta, 0, 255)` increases by +4 per cq step until the clamp kicks in
4. Final q is clamped to `[0, 255]`

This means each +1 on cq translates to +4 on actual qindex until convergence.

### Verified qindex mapping (10-bit, `use-fixed-qp-offsets=0`, actual encoder)

Values from actual encoder output after the first-ARF bug fix. ARF qindex is now consistent across all GOPs. KF varies slightly between first KF and subsequent KFs due to state adaptation. IARF values are derived from midpoint interpolation between `arf_q` and `min(cq_level, 255)`.

| cq | qindex | KF (range) | ARF | IARF_d2 | IARF_d3 | IARF_d4 | Leaf |
|----|--------|------------|-----|---------|---------|---------|------|
| 55 | 220    | ~135       | 173 | 197     | 209     | 215     | 220  |
| 60 | 240    | 155–159    | 192 | 216     | 228     | 234     | 240  |
| 63 | 255    | 171–185    | 208 | 232     | 244     | 250     | 255  |
| 64 | 259    | 175–187    | 212 | 236     | 248     | 254     | 255  |
| 70 | 283    | 200–211    | 236 | 255     | 255     | 255     | 255  |
| 80 | 323    | 219–251    | 255 | 255     | 255     | 255     | 255  |

### cq-level to qindex plot (non-fixed QP)

![cq-level to qindex mapping — use-fixed-qp-offsets=0](cq_to_qindex_nonfixed.svg)

### Convergence at max_cq=80

With max_cq=80 (virtual qindex=323):
- **ARF**: reaches 255 (fully converged)
- **IARF/Leaf**: converged at 255
- **KF**: reaches ~219–251 depending on content/state (first KF vs subsequent KFs use different code paths)

KF does not fully reach 255 at cq=80 in the non-fixed QP path, but gets close enough (~219-251). This is acceptable since the non-fixed QP path is inherently content-adaptive, and KF quality is state-dependent by design.

### First-ARF Bug: Root Cause and Fix

**Bug:** When `cq_level > 255` and `use-fixed-qp-offsets=0`, the first ARF frame in a sequence got qindex=0, causing massive bitrate inflation (e.g., 1,077 kbps at cq=64 vs expected ~69 kbps).

**Root cause:** In AOM_Q mode, non-ARF_UPDATE frames are dispatched via `rc_pick_q_and_bounds()` → `rc_pick_q_and_bounds_q_mode()` → `get_active_best_quality()`. For ARF/GF frames (non-leaf, non-IARF), the code used `get_gf_active_quality()` — a boost-factor-based minq lookup designed for VBR/CQ modes. This function ignores `cq_level` entirely and computes quality based on `avg_frame_qindex`, which is very low early in the sequence (dominated by the high-quality KF). This produced near-zero qindex for the first ARF, which then cascaded into bad IARF values via midpoint interpolation.

The second and subsequent ARFs appeared to work because by that point `avg_frame_qindex[INTER_FRAME]` had been updated to reasonable values, masking the underlying issue. However, even for those ARFs, the quality was determined by boost tables rather than `cq_level`, making the behavior inconsistent with AOM_Q's constant-quality design.

**Fix:** In `get_active_best_quality()` (`ratectrl.c`), add an early return for `rc_mode == AOM_Q` non-IARF frames that uses the same `cq_level + av1_compute_qdelta(q_val, q_val * 0.40)` formula:

```c
// Determine active_best_quality for frames that are not leaf or overlay.
if (rc_mode == AOM_Q && !is_intrl_arf_boost) {
    const double q_val = av1_convert_qindex_to_q(cq_level, bit_depth);
    const int delta_qindex =
        av1_compute_qdelta(rc, q_val, q_val * 0.40, bit_depth);
    active_best_quality =
        clamp(cq_level + delta_qindex, rc->best_quality, rc->worst_quality);
    return active_best_quality;
}
```

This ensures:
- ARF/GF qindex is derived from `cq_level` (consistent with AOM_Q constant-quality design)
- The delta is constant for cq > 63 (q_val clamped), producing a smooth linear ramp
- Clamping to `[0, 255]` provides natural convergence
- IARF frames still use `p_rc->arf_q` with midpoint interpolation (unchanged, line 1728+)

### Bitrate validation (non-fixed QP, after fix)

Bitrate now decreases monotonically with increasing cq-level:

| cq | Bitrate (bps) | PSNR-Y (dB) | Before fix (bps) |
|----|--------------|-------------|------------------|
| 60 | 100,246      | 32.16       | 137,435          |
| 63 | 72,612       | 30.19       | 99,891           |
| 64 | 68,676       | 29.87       | 1,077,227        |
| 70 | 47,294       | 27.81       | 493,705          |
| 80 | 31,805       | 25.19       | 307,166          |

---

## Safety Analysis: Impact on Other Rate Control Modes

| Mode             | Uses cq_level? | Impact                                                                 | Status         |
|------------------|---------------|------------------------------------------------------------------------|----------------|
| **AOM_CBR**      | No            | Completely unaffected                                                  | SAFE           |
| **AOM_VBR**      | No            | Completely unaffected                                                  | SAFE           |
| **AOM_CQ**       | Yes (comparisons) | cq_level used in `if (q < cq_level)` — final q clamped to [0, 255] | SAFE           |
| **AOM_Q (fixed QP)**| Yes (direct) | Extended range handled by qstep_ratio ramp + shortcut              | SAFE with fixes |
| **AOM_Q (non-fixed)**| Yes (direct) | ARF uses cq_level + delta formula; linear ramp with convergence    | SAFE with fix  |

**Key safety guarantees:**
- `av1_ac_quant()` in `quant_common.c` already has internal `clamp(qindex + delta, 0, MAXQ)` — defense-in-depth
- All `rc_pick_q_and_bounds_*` functions clamp final q to `[rc->best_quality, rc->worst_quality]` = [0, 255]
- `best_allowed_q`/`worst_allowed_q` flow into `rc->worst_quality` which is used as array index for `inter_minq[]`/`rtc_minq[]` (256 elements) — must stay <= 255 (Fix #2)
- `cq_level` is never used as a direct array index — only in arithmetic and as input to `av1_convert_qindex_to_q()` (Fix #3)
- The `qstep_ratio >= 1.0` shortcut in `encoder_utils.c` only activates at cq=80 and returns `AOMMIN(cq_level, MAXQ)` = 255
- The AOM_Q early return in `get_active_best_quality()` clamps to `[rc->best_quality, rc->worst_quality]` = [0, 255]

## Files Modified

| File                          | Changes                                                        | Status |
|-------------------------------|----------------------------------------------------------------|--------|
| `av1/encoder/av1_quantize.c`  | Extend table to 81 entries; update reverse lookup              | Done   |
| `av1/av1_cx_iface.c`         | cq_level range 63→80; clamp best/worst_allowed_q              | Done   |
| `av1/encoder/ratectrl.c`     | Clamp qindex in `av1_convert_qindex_to_q()`; fix ARF qindex in `get_active_best_quality()` for AOM_Q mode | Done |
| `av1/encoder/encoder_utils.c`| Update max_virtual_qindex 327→323; add qstep_ratio≥1.0 shortcut | Done |

## Test Configuration

The primary test configuration uses `use_fixed_qp_offsets=1` with `deltaq-mode=0` to achieve
deterministic, content-independent qindex assignment per hierarchical layer.

### Encoding Command

```
aomenc --verbose --codec=av1 -v --psnr --obu \
  --frame-parallel=0 --cpu-used=0 --limit=600 --passes=1 --end-usage=q \
  --i420 --use-fixed-qp-offsets=1 --deltaq-mode=0 --enable-tpl-model=0 \
  --fps=60/1 --input-bit-depth=10 --bit-depth=10 --cq-level=$CQ \
  --tile-columns=0 --threads=1 --enable-keyframe-filtering=1 \
  --kf-min-dist=65 --kf-max-dist=65 --min-gf-interval=16 --max-gf-interval=16 \
  --gf-min-pyr-height=4 --gf-max-pyr-height=4 --lag-in-frames=19 --auto-alt-ref=1 \
  -w 1920 -h 1080 \
  -o output.obu input.y4m
```

### Key flags explained

| Flag | Value | Purpose |
|------|-------|---------|
| `--use-fixed-qp-offsets=1` | 1 | Bypass polynomial minq lookup tables; use unified qstep_ratio approach for ARF/KF qindex across entire cq range |
| `--deltaq-mode=0` | 0 | Disable per-SB delta-q adaptation; uniform qindex within each frame |
| `--enable-tpl-model=0` | 0 | Disable temporal propagation model; prevent TPL from overriding the fixed qp offsets |
| `--passes=1` | 1 | Single-pass encode; uses `rc_pick_q_and_bounds_no_stats` for ARF frames |
| `--kf-min-dist=65 --kf-max-dist=65` | 65 | Fixed keyframe interval at every 65 frames |
| `--min-gf-interval=16 --max-gf-interval=16` | 16 | Fixed golden frame interval at 16 frames |
| `--gf-min-pyr-height=4 --gf-max-pyr-height=4` | 4 | 4-level hierarchical pyramid (ARF → IARF_d2 → IARF_d3 → IARF_d4 → Leaf) |
| `--bit-depth=10` | 10 | 10-bit encoding; uses `dc_qlookup_10_QTX[]` for quantizer lookup |

### Q selection code path with this configuration

With `use_fixed_qp_offsets=1`, the final qindex override happens in
`av1_set_size_dependent_vars()` (`encoder_utils.c`), which runs **after**
the initial `av1_rc_pick_q_and_bounds()` selection:

1. **KF/ARF/GF frames** (`is_frame_tpl_eligible()` returns true):
   - cq_level ≤ 255: `qstep_ratio = 0.2 + (1.0 - awq/255) * qratio_grad`, search downward for matching qindex via `av1_get_q_index_from_qstep_ratio()`
   - cq_level > 255 (cq 64–80): `qstep_ratio = 0.2 + (cq_level - 255) / (323 - 255) * 0.8`, same search with `leaf_qindex = min(cq_level, 255) = 255`
   - When `qstep_ratio >= 1.0` (cq=80): shortcut returns `min(cq_level, 255)` = 255 directly
   - KF and ARF use the same qstep_ratio (unified, no separate boost factors)
2. **INTNL_ARF frames** (intermediate pyramid levels): midpoint interpolation `q = (arf_q + min(cq_level, 255) + 1) / 2` per depth level
3. **Leaf/Overlay frames**: `q = min(cq_level, 255)`

This produces a deterministic qindex-per-layer mapping that depends only on `cq_level`.

## Verification

### Build
```
cmake path/to/aom && make -j$(nproc)
```

### Fixed QP path (`use-fixed-qp-offsets=1`)

| Test | Command | Expected Result | Status |
|------|---------|-----------------|--------|
| Original range | `--cq-level=32` | No regression | Pass |
| Extended range | `--cq-level=70` | Valid output, per-layer qindex differentiation | Pass |
| Max cq | `--cq-level=80` | All layers reach qindex 255 | Pass |
| Out of range | `--cq-level=81` | Rejected with error | Pass |

Bitrate (480x270, 130 frames, cpu-used=4):

| cq | Bitrate (bps) | PSNR-Y (dB) |
|----|--------------|-------------|
| 60 | 154,870      | 34.38       |
| 63 | 100,685      | 31.58       |
| 64 | 82,633       | 30.28       |
| 70 | 39,651       | 25.98       |
| 80 | 27,969       | 23.63       |

### Non-fixed QP path (`use-fixed-qp-offsets=0`)

| Test | Command | Expected Result | Status |
|------|---------|-----------------|--------|
| Original range | `--cq-level=32` | No regression | Pass |
| Extended range | `--cq-level=70` | Valid output, monotonic bitrate decrease | Pass |
| Max cq | `--cq-level=80` | ARF/IARF/Leaf reach qindex 255 | Pass |
| First ARF | `--cq-level=64` | No qindex=0 anomaly | Pass |

Bitrate (480x270, 130 frames, cpu-used=4):

| cq | Bitrate (bps) | PSNR-Y (dB) |
|----|--------------|-------------|
| 60 | 100,246      | 32.16       |
| 63 | 72,612       | 30.19       |
| 64 | 68,676       | 29.87       |
| 70 | 47,294       | 27.81       |
| 80 | 31,805       | 25.19       |

### Other modes

| Test | Command | Expected Result | Status |
|------|---------|-----------------|--------|
| CBR mode | `--end-usage=cbr --target-bitrate=1000` | No regression (cq_level not used) | Pass |
| CQ mode | `--end-usage=cq --cq-level=70` | Runs correctly | Pass |
| Unit tests | `make runtests` | All pass | — |

## Reference

- Extended range plot (fixed QP): `cq_extended_fixed_maxcq80.svg`
- Unified plot: `qindex_per_layer_chart_unified.svg`
- Original range comparison: `cq_to_qindex_fixed.svg` (fixed QP) / `cq_to_qindex_nonfixed.svg` (non-fixed QP)
- Alternative max_cq options: `cq_extended_fixed_maxcq70.svg`, `cq_extended_fixed_maxcq75.svg`
- Data: `cq_to_qindex_data.csv`
- Computation script: `plot_cq_to_qindex.py`
