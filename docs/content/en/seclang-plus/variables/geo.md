+++
title = "GEO"
weight = 68
+++

**Description:** Geolocation information

**Syntax:** `GEO`

**Implementation Status:** No

WGE can parse this directive, but the functionality is not implemented yet.

**Example:**

```apache
SecRule GEO "@rx value" "id:1001,deny,msg:'Test'"
```
