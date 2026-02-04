+++
title = "PATH_INFO"
weight = 39
+++

**Description:** Path information

**Syntax:** `PATH_INFO`

PATH_INFO contains additional path information after the file path in the URL and before the query string. For example, for /cgi-bin/script.py/extra/path, PATH_INFO's value is /extra/path. In RESTful APIs, this variable may contain routing parameters.

**Example:**

```apache
# Detect directory traversal in PATH_INFO
SecRule PATH_INFO "@contains .." \
    "id:1040,phase:1,deny,msg:'Directory traversal detected in PATH_INFO'"
```
