+++
title = "@ge"
weight = 16
+++

**Description:** Greater than or equal to

**Syntax:** `"@ge number"`

@ge checks if the variable value (converted to integer) is greater than or equal to the specified number. Commonly used for threshold detection, especially determining if cumulative anomaly scores have reached the blocking threshold. Can be combined with @le for closed-interval range detection.

**Example:**

```apache
# Anomaly score reaches blocking threshold
SecRule TX:anomaly_score "@ge 5" \
    "id:1095,phase:2,deny,msg:'Anomaly score reached threshold: %{TX.anomaly_score}'"

# Detect large file upload
SecRule FILES_COMBINED_SIZE "@ge 5242880" \
    "id:1096,phase:2,deny,msg:'Total uploaded file size exceeds 5MB'"

# Detect brute force (failed attempts >= 5)
SecRule IP:failed_login "@ge 5" \
    "id:1097,phase:2,deny,msg:'Suspected brute force attack'"
```

**Parameter Type:** `int`

**Case Sensitive:** Yes
