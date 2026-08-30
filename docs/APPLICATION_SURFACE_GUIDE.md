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
