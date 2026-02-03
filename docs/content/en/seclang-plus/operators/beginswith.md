+++
title = "@beginsWith"
weight = 3
+++

**Description:** Begins with the specified string

**Syntax:** `"@beginsWith string"`

@beginsWith checks if the variable value begins with the specified string. Matching is case sensitive. Compared to using @rx "^prefix" regex, @beginsWith performs better for simple prefix checks as it avoids regex engine overhead. Commonly used for URL path checks, protocol validation, and similar scenarios.

**Example:**

```apache
# Detect admin panel access
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1070,phase:1,deny,msg:'Unauthorized access to admin path'"

# Detect API path and tag
SecRule REQUEST_URI "@beginsWith /api/v" \
    "id:1071,phase:1,pass,nolog,setvar:tx.is_api_request=1"

# Detect dangerous protocols (e.g., javascript:)
SecRule ARGS "@beginsWith javascript:" \
    "id:1072,phase:2,deny,msg:'JavaScript protocol injection detected'"

# Detect Base64 encoded data
SecRule REQUEST_BODY "@beginsWith data:image" \
    "id:1073,phase:2,pass,nolog,setvar:tx.has_base64_image=1"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
