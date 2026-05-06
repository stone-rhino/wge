+++
title = "severity"
weight = 4
+++

**Description:** Severity level (0-7)

**Syntax:** `severity:CRITICAL|ERROR|WARNING|NOTICE|INFO|DEBUG`

The severity action assigns a severity level to a rule. This is used for categorizing threats and prioritizing alerts. Common values include CRITICAL for severe attacks, WARNING for suspicious activity, and NOTICE for informational events.

**Example:**

```apache
SecRule ARGS "@rx test" "id:1001,deny,severity:CRITICAL"
```

**Parameter Type:** `int or string`

**Case Sensitive:** Yes
