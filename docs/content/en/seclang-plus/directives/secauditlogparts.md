+++
title = "SecAuditLogParts"
weight = 56
+++

**Description:** Configure which parts of a transaction should be recorded in the audit log.


**Syntax:** `SecAuditLogParts PARTS`


**Default:** ABCFHZ

**Parts:**


- **A:** Audit log header (mandatory)
- **B:** Request headers
- **C:** Request body (only if request body access is enabled and the body exists)
- **D:** Reserved for intermediary response headers
- **E:** Response body (only if response body access is enabled and the body exists)
- **F:** Final response headers
- **G:** Reserved for actual response body
- **H:** Audit log trailer (contains additional data)
- **I:** Compact request body (alternative to C, excludes files)
- **J:** Reserved for upload file information
- **K:** List of all rules that matched
- **Z:** Final boundary marker (mandatory)

**Case Sensitive:** Yes

**Implemented:** No


**Example:**


```apache
SecAuditLogParts ABCFHZ
```
