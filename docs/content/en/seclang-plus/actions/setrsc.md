+++
title = "setrsc"
weight = 30
+++

**Description:** Set resource ID

**Syntax:** `setrsc:identifier`

The setrsc action sets a resource identifier for the current transaction. This identifier is used to initialize and track RESOURCE collection variables, enabling per-resource tracking such as rate limiting specific endpoints or files.

**Example:**

```apache
SecRule REQUEST_FILENAME "@rx ^/api/" "id:1001,pass,setrsc:%{REQUEST_FILENAME}"
```

**Parameter Type:** `string`

**Implementation Status:** Not yet implemented

**Case Sensitive:** Yes
