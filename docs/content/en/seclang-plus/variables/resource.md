+++
title = "RESOURCE"
weight = 86
+++

**Description:** Resource variable collection

**Syntax:** `RESOURCE:key`

The RESOURCE collection is used to store persistent data associated with specific resources (such as URL paths). Can be used to track access patterns for specific resources, such as the call frequency of a particular API endpoint.

**Example:**

```apache
# Initialize resource collection
SecAction "id:1062,phase:1,pass,nolog,initcol:RESOURCE=%{REQUEST_FILENAME}"
```
