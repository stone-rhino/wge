+++
title = "SESSION"
weight = 84
+++

**Description:** Session variable collection

**Syntax:** `SESSION:key`

The SESSION collection is used to store persistent data associated with user sessions. You need to first set the session identifier via the setsid action, then initialize via initcol:SESSION=%{SESSIONID}. Can be used to track session-level behavior and implement session-level access control.

This variable is not recommended at the moment because its parsing and assignment behavior still has some issues.

**Example:**

```apache
# Set session ID and initialize
SecRule REQUEST_COOKIES:PHPSESSID "@rx (.+)" \
    "id:1060,phase:1,pass,nolog,setsid:%{TX.1},initcol:SESSION=%{SESSIONID}"
```
