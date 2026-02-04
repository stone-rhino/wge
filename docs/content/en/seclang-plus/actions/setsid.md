+++
title = "setsid"
weight = 29
+++

**Description:** Set session ID

**Syntax:** `setsid:identifier`

The setsid action sets a session identifier for the current transaction. This identifier is used to initialize and track SESSION collection variables, enabling session-based tracking and per-session rate limiting.

**Example:**

```apache
SecRule REQUEST_COOKIES:session_id "@rx ^[a-f0-9]{32}$" "id:1001,pass,setsid:%{REQUEST_COOKIES.session_id}"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented

**Case Sensitive:** Yes
