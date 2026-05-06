+++
title = "deny"
weight = 10
+++

**Description:** Deny request

**Syntax:** `deny`

The deny action is the most commonly used disruptive action, used to immediately terminate processing of the current request and return an error response. When a rule matches, WGE stops processing subsequent rules and returns the specified HTTP status code to the client (default 403 Forbidden).

The deny action is typically used with the status action to specify a custom response status code. In anomaly scoring mode, deny is usually only used for the final scoring threshold check rule.

**Example:**

```apache
# Basic deny - returns default 403 status code
SecRule ARGS "@detectSQLi" "id:1,phase:2,deny,msg:'SQL Injection Detected'"

# Specify custom status code
SecRule REQUEST_URI "@contains ../../../" \
    "id:2,phase:1,deny,status:400,msg:'Path Traversal Attack'"

# Threshold check in anomaly scoring mode
SecRule TX:anomaly_score "@ge 5" \
    "id:3,phase:2,deny,status:403,msg:'Anomaly Score Exceeded: %{TX.anomaly_score}'"

# Return 503 for service temporarily unavailable
SecRule IP:request_count "@gt 1000" \
    "id:4,phase:1,deny,status:503,msg:'Request Rate Limit'"
```

**Parameter Type:** `none`

**Case Sensitive:** Yes
