+++
title = "MULTIPART_UNMATCHED_BOUNDARY"
weight = 45
+++

**Description:** Multipart unmatched boundary

**Syntax:** `MULTIPART_UNMATCHED_BOUNDARY`

MULTIPART_UNMATCHED_BOUNDARY is set to 1 when unmatched boundary markers appear in the multipart request body. Unmatched boundaries usually indicate the request has been tampered with or constructed abnormally, possibly an attacker attempting to bypass upload file detection.

**Example:**

```apache
# Reject multipart requests with unmatched boundaries
SecRule MULTIPART_UNMATCHED_BOUNDARY "!@eq 0" \
    "id:1043,phase:2,deny,status:403,msg:'Multipart boundary mismatch'"
```
