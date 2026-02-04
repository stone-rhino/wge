+++
title = "SecDataDir"
weight = 15
+++

**Description:** Configure the persistent data storage directory.

**Syntax:** `SecDataDir PATH`

This directory is used to store persistent data such as IP reputation records, session data, user tracking data, etc. The WAF uses this directory to save collection data that needs to persist across requests (initialized through the initcol action). The directory must have write permissions for the user running the WAF process.

**Example:**

```
SecDataDir /var/lib/wge/
```

**Case Sensitive:** Yes
