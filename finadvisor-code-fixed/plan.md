# FinAdvisor C — Implementation Plan

## Scope
A responsive personal-finance advisor dashboard inspired by the supplied FinPulse reference. It includes persona presets, live financial inputs, deterministic financial scoring, allocation recommendations, an action roadmap, wealth projection, and a visible C decision pipeline.

## Architecture
- `src/engine.c/.h`: pure C financial engine. Computes surplus, savings rate, DTI, emergency runway, FinHealth score, allocation, actions, and future value.
- `src/server.c`: dependency-free C HTTP server on port 3000. Serves `public/` and exposes `GET /api/advice` with query parameters.
- `public/`: HTML/CSS/JavaScript dashboard. JavaScript calls the C API and renders results.
- `tools/dev.py`: optional Python helper that compiles the C server and launches it for local development.

## Design
- **Style options:** editorial fintech console (0.06); quiet neo-brutalist planner (0.07); dark observatory dashboard (0.05).
- **Chosen movement:** dark observatory dashboard: a calm, high-signal control room for personal money decisions.
- **Core principles:** dense but readable information hierarchy; live cause-and-effect feedback; high contrast with restrained accent colors; progressive disclosure from score to action.
- **Color philosophy:** deep ink backgrounds create focus, warm paper panels communicate trust, and lime/blue/coral accents distinguish positive momentum, data, and risk.
- **Layout paradigm:** asymmetric left rail plus a wide workbench; metrics are arranged as a narrative from current state to next action rather than a generic centered grid.
- **Signature elements:** lime status rail, oversized score ring, thin technical labels, and a flowing wealth curve.
- **Interaction:** every slider updates the same C engine state; persona cards are fast scenarios; actions are concrete and prioritized.
- **Animation:** short eased transitions for score, cards and bars; no looping decoration; chart redraws only when values change.
- **Typography:** Inter for UI and IBM Plex Mono for labels/data.
- **Brand essence:** practical financial clarity for people who want a plan, not a lecture. Calm, precise, actionable.
- **Brand voice:** “Make the next money move obvious.” and “Your numbers change. Your plan should too.”
- **Wordmark:** `fin/` lime slash mark beside `advisor` in a compact technical lockup.
- **Signature color:** signal lime `#c6f36b`.

## Runtime
The development server listens on `0.0.0.0:3000`. The C server owns `/api/advice` and static file serving. Python is tooling only and does not replace the C backend.
