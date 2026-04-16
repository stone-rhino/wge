+++
title = "@ipMatchF"
weight = 19
+++

**Description:** Short form of `@ipMatchFromFile`

**Syntax:** `@ipMatchF file_path`

**Input Data Type:** `string`

**Implementation Status:** Not yet implemented

WGE can parse this directive, but the feature is not implemented yet.

**Example:**

```apache
# Load IP blacklist from file
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/ip-blacklist.txt" \
    "id:1107,phase:1,deny,msg:'IP is in blacklist'"

# Load trusted proxy IP list from file
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/trusted-proxies.txt" \
    "id:1108,phase:1,pass,nolog,setvar:tx.is_trusted_proxy=1"
```
