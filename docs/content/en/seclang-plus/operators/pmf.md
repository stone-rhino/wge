+++
title = "@pmf"
weight = 10
+++

**Description:** Loads keywords from file for parallel matching (alias for @pmFromFile)

**Syntax:** `"@pmf /path/to/keywords.txt"`

Same as @pm, but loads keyword list from external file. One keyword per line in the file; lines starting with # are treated as comments and ignored. Suitable for scenarios managing a large number of keywords.

**Example:**

```apache
# Load SQL injection keywords from file
SecRule ARGS "@pmf /etc/wge/sql-keywords.txt" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"
```

**Parameter Type:** `string (file path)`
