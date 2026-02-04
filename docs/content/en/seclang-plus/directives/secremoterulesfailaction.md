+++
title = "SecRemoteRulesFailAction"
weight = 29
+++

**Description:** Configure how to handle remote rule loading failures.

**Syntax:** `SecRemoteRulesFailAction Abort|Warn`

**Default:** Abort

**Options:**

- **Abort:** Abort startup, ensuring the WAF does not run without rules
- **Warn:** Only log a warning and continue startup; the WAF will continue running with local rules

The default behavior is to terminate immediately if there is a problem downloading from the specified URL, ensuring the integrity of security policies.

**Example:**

```
SecRemoteRulesFailAction Warn
```

**Case Sensitive:** Yes
