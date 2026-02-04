+++
title = "MULTIPART_STRICT_ERROR"
weight = 44
+++

**Description:** Multipart strict error flag

**Syntax:** `MULTIPART_STRICT_ERROR`

MULTIPART_STRICT_ERROR is a composite flag variable set to 1 when any strict mode violations are detected during multipart request body parsing. It combines multiple multipart error flags (such as MULTIPART_UNMATCHED_BOUNDARY, etc.) for quick determination of whether a multipart request contains anomalies. This is an important checkpoint for preventing file upload bypass attacks.

**Example:**

```apache
# Reject requests with multipart parsing errors
SecRule MULTIPART_STRICT_ERROR "!@eq 0" \
    "id:1042,phase:2,deny,status:403,msg:'Multipart request parsing anomaly'"
```
