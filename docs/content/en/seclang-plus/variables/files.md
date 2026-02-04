+++
title = "FILES"
weight = 75
+++

**Description:** Uploaded file content

**Syntax:** `FILES | FILES:fieldname`

FILES contains the original filenames of files uploaded via multipart/form-data. You can access files uploaded in a specific form field using FILES:fieldname. This variable is central to file upload security checks and can be used to verify file extensions, filename formats, etc.

**Example:**

```apache
# Check for dangerous extensions in uploaded files
SecRule FILES "@rx \.(php|jsp|asp|exe|sh|bat)$" \
    "id:1050,phase:2,deny,status:403,msg:'Uploading this file type is forbidden'"
```
