+++
title = "@eq"
weight = 13
+++

**Description:** Numeric equality

**Syntax:** `"@eq number"`

@eq performs numeric equality comparison on the variable value. Variable values are first converted to integers before comparison; non-numeric strings are converted to 0. Commonly used for status code checks, count values, and other exact numeric matching scenarios. Supports dynamic comparison values using macro expansion.

**Example:**

```apache
# Detect 404 response status
SecRule RESPONSE_STATUS "@eq 404" \
    "id:1085,phase:3,pass,log,msg:'Page not found'"

# Detect empty request body
SecRule REQUEST_HEADERS:Content-Length "@eq 0" \
    "id:1086,phase:1,pass,nolog,setvar:tx.empty_body=1"

# Detect if anomaly score reaches threshold
SecRule TX:anomaly_score "@eq %{TX.threshold}" \
    "id:1087,phase:2,deny,msg:'Anomaly score reached threshold'"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
