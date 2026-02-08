+++
title = "GEO"
weight = 68
+++

**Description:** Geolocation information

**Syntax:** `GEO`

**Implementation Status:** No

**Example:**

```apache
SecRule GEO "@rx value" "id:1001,deny,msg:'Test'"
```
