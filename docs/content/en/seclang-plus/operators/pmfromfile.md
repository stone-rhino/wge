+++
title = "@pmFromFile"
weight = 11
+++

**Description:** Loads keywords from file for parallel matching

**Syntax:** `"@pmFromFile /path/to/keywords.txt"`

Functions identically to @pmf. One keyword per line in the file; lines starting with # are treated as comments and ignored. Supports both relative paths (relative to config directory) and absolute paths.

**Example:**

```apache
# Load malicious User-Agent list from file
SecRule REQUEST_HEADERS:User-Agent "@pmFromFile /etc/wge/bad-ua.txt" \
    "id:1001,phase:1,deny,msg:'Malicious User-Agent'"
```

**Parameter Type:** `string (file path)`
