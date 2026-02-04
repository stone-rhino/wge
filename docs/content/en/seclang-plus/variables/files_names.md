+++
title = "FILES_NAMES"
weight = 76
+++

**Description:** Uploaded file names

**Syntax:** `FILES_NAMES`

FILES_NAMES contains the form field names corresponding to all uploaded files. Can be used to detect anomalous content in form field names, or to verify that uploaded files are submitted through expected form fields.

**Example:**

```apache
# Ensure files are only uploaded through the 'avatar' field
SecRule FILES_NAMES "!@streq avatar" \
    "id:1051,phase:2,deny,msg:'Illegal file upload field'"
```
