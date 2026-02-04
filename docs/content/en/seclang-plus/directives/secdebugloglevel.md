+++
title = "SecDebugLogLevel"
weight = 18
+++

**Description:** Configure the verbosity level of the debug log.

**Syntax:** `SecDebugLogLevel LEVEL`

**Default:** 0

**Options:**

- **0:** No logging
- **1:** Errors only
- **2:** Warnings
- **3:** Notices
- **4:** Detailed information
- **5:** Debug information
- **9:** Most verbose

Keeping debug logging always enabled in production environments is generally not recommended. Even when troubleshooting specific issues, be aware that using level 4 or higher will significantly impact performance.

**Example:**

```
SecDebugLogLevel 3
```

**Case Sensitive:** Yes
