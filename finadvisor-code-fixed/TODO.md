# FinAdvisor C — Outcomes

## Dashboard and responsive interface
- Deliver a modern, clean, responsive dashboard inspired by the supplied reference website.
- Include a navigation rail, live diagnostic snapshot, persona presets, financial controls, score cards, allocation view, roadmap, wealth projection and C architecture panel.

## C financial engine
- Implement deterministic calculations in C for monthly surplus, savings rate, debt-to-income ratio, emergency-fund runway, FinHealth score, investment allocation and wealth projection.
- Keep rules readable in C and return a structured JSON response for the frontend.

## Interactive frontend integration
- HTML/CSS/JavaScript must call the C API and update score, metrics, allocation bars, actions and chart without a page reload.
- Persona buttons and all financial inputs must trigger a fresh calculation.

## Python tooling
- Provide a Python helper that compiles the C sources and launches the C server for development.

## Project runtime
- Serve `GET /manus-routes.json` and listen on configured port 3000.
- Keep the project buildable with the included Makefile and Dockerfile.
