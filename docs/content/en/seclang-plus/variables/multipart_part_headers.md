+++
title = "MULTIPART_PART_HEADERS"
weight = 42
+++

**Description:** Multipart part headers

**Syntax:** `MULTIPART_PART_HEADERS | MULTIPART_PART_HEADERS:HeaderName`

MULTIPART_PART_HEADERS contains the header information of each part in a multipart/form-data request. Used to detect abnormal content in headers like Content-Type and Content-Disposition of uploaded file parts.

**Example:**

```apache
# Detect abnormal Content-Type in multipart headers
SecRule MULTIPART_PART_HEADERS "@rx application/(php|x-php|x-httpd-php)" \
    "id:1041,phase:2,deny,msg:'Uploaded file has abnormal Content-Type'"
```
