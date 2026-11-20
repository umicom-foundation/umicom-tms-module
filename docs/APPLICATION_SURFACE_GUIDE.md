![Umicom](../../../assets/umicom-logo.svg)

# Umicom TMS Application Surface

Umicom TMS now starts the Framework-owned treasury workspace through the same
product surface used by other Umicom applications. The standard layout contains
trade blotter, trade capture, market data, pricing, risk, workflow, settlement
and accounting components. Learning and focus recipes show smaller workspaces.

The surface can start without market data, positions or settlement services.
Its empty states explain what a user must connect or select next. Commands are
staged for approval and never described as executed by the presentation layer.

Use `umi_tms_application_surface_create()` for the standard layout or
`umi_tms_application_surface_create_for_audience()` to choose learning or
focus. Product services can later replace guidance with real data while the
Framework-owned layout and lifecycle stay unchanged.

## Read a practice FX forward forecast

1. Open a terminal where the TMS console built with the FX cashflow forecast
   is available. Run `umicom-tms-console --fx-cashflows`.
2. Read the payment date `2026-12-01`. The practice contract buys GBP and
   pays USD on that date. The console should print `Receive: 1000.00 GBP`
   and `Pay: 1250.00 USD`.
3. Check the calculation by multiplying the GBP amount by the agreed rate
   of `1.25 USD` per GBP. The Framework service uses decimal coefficients
   and refuses a result that cannot fit the specified quote precision.

A forecast lists the expected movements under the reviewed contract. A
market valuation estimates the position's value using a market quote and a
discount factor. A ledger records a separate approved posting. This example
uses fictional data and writes no payments or postings.

The Framework forecast service expects the exact terms and payment date from
the reviewed confirmation, plus the book and trade revisions used in that
review. When the revisions or the recorded terms disagree, reload the trade
and review the confirmation before retrying. The legacy book stores the amount
and rate as binary floating-point values. Use the confirmation as the source
of exact precision and verify its provenance in the upstream review workflow.
If the console reports that its
SDK lacks forecasts, rebuild against a Framework SDK that provides the
`umicom/open_tms/fx_forward_cashflows.h` contract.
