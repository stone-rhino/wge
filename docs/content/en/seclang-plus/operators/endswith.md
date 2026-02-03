+++
title = "@endsWith"
weight = 4
+++

**Description:** Ends with the specified string

**Syntax:** `"@endsWith string"`

@endsWith checks if the variable value ends with the specified string. Matching is case sensitive. Compared to using @rx "suffix$" regex, @endsWith performs better for simple suffix checks. Commonly used for file extension checks, path validation, and similar scenarios.

**Example:**

```apache
# Detect dangerous file extension access
SecRule REQUEST_FILENAME "@endsWith .bak" \
    "id:1074,phase:1,deny,msg:'Access to backup files forbidden'"

# Detect config file access
SecRule REQUEST_URI "@endsWith .conf" \
    "id:1075,phase:1,deny,msg:'Access to config files forbidden'"

# Detect PHP file upload
SecRule FILES_NAMES "@endsWith .php" \
    "id:1076,phase:2,deny,msg:'PHP file upload forbidden'"

# Detect hidden files (Unix-style)
SecRule REQUEST_FILENAME "@endsWith /.htaccess" \
    "id:1077,phase:1,deny,msg:'Access to .htaccess file forbidden'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
