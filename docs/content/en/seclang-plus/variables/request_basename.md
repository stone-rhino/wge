+++
title = "REQUEST_BASENAME"
weight = 18
+++

**Description:** Request file basename

**Syntax:** `REQUEST_BASENAME`

REQUEST_BASENAME contains only the filename portion of the request path, excluding the directory path. For example, for /path/to/file.php, this variable's value is file.php. Used to detect specific filenames or file extensions.

**Example:**

```apache
# Block access to backup files
SecRule REQUEST_BASENAME "@rx \.(bak|backup|old|orig)$" \
    "id:1026,phase:1,deny,msg:'Access to backup file forbidden'"

# Block execution of shell scripts
SecRule REQUEST_BASENAME "@endsWith .sh" \
    "id:1027,phase:1,deny,msg:'Shell script execution forbidden'"
```
