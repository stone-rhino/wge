+++
title = "QUERY_STRING"
weight = 21
+++

**Description:** Query string

**Syntax:** `QUERY_STRING`

QUERY_STRING contains the query string portion of the URL after the question mark (?), excluding the question mark itself. This is the raw query string, not parsed into individual parameters. Used to detect abnormal patterns in the overall query string.

**Example:**

```apache
# Detect SQL injection in query string
SecRule QUERY_STRING "@detectSQLi" \
    "id:1032,phase:1,deny,msg:'SQL injection detected in query string'"

# Limit query string length
SecRule QUERY_STRING "@gt 2048" "t:length,id:1033,phase:1,deny,msg:'Query string too long'"
```
