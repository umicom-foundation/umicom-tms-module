# Umicom TMS Module

Thin C23 application composition for the Umicom Treasury Management System.

The module intentionally contains **no duplicated trade, account, risk,
persistence, selection-routing or linked-workbench logic**. Those reusable
capabilities belong in Umicom Framework. This first module phase consumes the
Framework-owned Trader/TMS workbench profile and exposes the TMS composition.

Current composition:

- Trading group: `trading.red`
- Operations group: `operations.green`
- Trade List
- Trade Details
- Account
- Context Inspector

Future TMS work can add product-specific treasury workflows and presentation,
while canonical financial services and context infrastructure remain Framework
responsibilities.

TMS now starts learning, standard and focus treasury layouts through the
Framework product surface. The presentation layer can run without live services
and stages financial commands for approval. See
[the TMS application surface guide](docs/APPLICATION_SURFACE_GUIDE.md).

The native `umicom-tms` workstation presents role-based treasury layouts with
Framework-owned identity, appearance, docking, floating, grouping and layout
editing. The console remains available for verification and automation tasks.
Umicom Desk launches the native workstation when TMS is selected.
