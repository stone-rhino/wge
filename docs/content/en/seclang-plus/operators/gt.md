+++
title = "@gt"
weight = 14
+++

**Description:** Greater than

**Syntax:** `"@gt number"`

@gt checks if the variable value (converted to integer) is greater than the specified number. Commonly used for detecting threshold violations such as oversized requests, rate limiting, and anomaly score thresholds. One of the core operators for implementing rate limiting and threshold detection.

**Example:**

```apache
# Detect oversized request body (exceeds 10MB)
SecRule REQUEST_HEADERS:Content-Length "@gt 10485760" \
    "id:1088,phase:1,deny,msg:'Request body exceeds size limit'"

# Detect anomaly score exceeded
SecRule TX:anomaly_score "@gt 10" \
    "id:1089,phase:2,deny,msg:'Anomaly score exceeded threshold: %{TX.anomaly_score}'"

# Detect IP request rate limit exceeded
SecRule IP:request_count "@gt 100" \
    "id:1090,phase:1,deny,msg:'IP request rate limit exceeded'"

# Detect abnormal parameter count
SecRule &ARGS "@gt 100" \
    "id:1091,phase:2,deny,msg:'Abnormal parameter count'"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
