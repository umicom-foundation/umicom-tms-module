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

## Review linked context values

Treasury Management exposes Framework's reviewed context changes through
`umi_tms_workspace_context_review`, `umi_tms_workspace_context_apply`
and `umi_tms_workspace_clear_context` in
`umicom/tms/workspace_commands.h`. A context group is a named value that
related panels can share; for example, a host could use `treasury.account` with the sample
value `practice-account`. The host must connect that name to its panel consumers.

1. Use a runtime initialised with this product's canonical experience. Prepare
   the requested changes with the review function; preparation changes no live state.
2. Display the copied Framework summary and rows. Keep the runtime and its
   workbench alive while the user reviews the proposed values. If the summary
   reports UI differences, show the captured UI value beside the cached value.
3. Apply only after acceptance, then destroy the review. If the workspace changed,
   prepare a fresh review. Cancelling only destroys the review.

This is a module API; native review screens are separate host work. Context edits
do not execute product commands or external operations. The shared guide at
`framework/docs/guides/REVIEWING_LINKED_CONTEXTS.html` in the Applications checkout
explains capacity, ownership, thread coordination and recovery in more detail.


### Dated cash planning

Open **Cash planning** to maintain local dated inflows and outflows. Apply the
plan's settings, add entries with unique IDs, and inspect end-of-day balances and
the first projected buffer shortfall. You can disable or edit an entry, undo the
last stored change, save a new JSON file, review a loaded plan before applying
it, and export a CSV report with explicit currency and minor units.

Framework owns the complete workflow. These are manual assumptions; the planner
does not read account balances, broker buying power, submit payments or place
orders. Save explicitly before closing. See Framework's
`docs/learning/cash-planning.html` for a worked example and recovery steps.
