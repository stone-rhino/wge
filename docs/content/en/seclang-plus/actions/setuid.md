+++
title = "setuid"
weight = 28
+++

**Description:** Set user ID

**Syntax:** `setuid:identifier`

The setuid action sets a user identifier for the current transaction. This identifier is used to initialize and track USER collection variables, enabling per-user rate limiting, behavior analysis, and session tracking.

**Example:**

```apache
SecRule REQUEST_COOKIES:user_id "@rx ^[a-z0-9]+$" "id:1001,pass,setuid:%{REQUEST_COOKIES.user_id}"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented

**Case Sensitive:** Yes
