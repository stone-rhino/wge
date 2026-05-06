+++
title = "ctl"
weight = 41
+++

**Description:** Modify runtime configuration

**Syntax:** `ctl:directive=value`

The ctl (control) action allows runtime modification of processing directives for the current transaction. This enables dynamic rule behavior changes based on request characteristics.

Available options:
- **ctl:auditEngine=On|Off|RelevantOnly** - Change audit logging mode
- **ctl:auditLogParts=+/-PARTS** - Add or remove audit log parts
- **ctl:forceRequestBodyVariable=On|Off** - Force REQUEST_BODY variable population
- **ctl:requestBodyAccess=On|Off** - Enable/disable request body inspection
- **ctl:requestBodyProcessor=URLENCODED|MULTIPART|XML|JSON** - Set body processor
- **ctl:ruleEngine=On|Off|DetectionOnly** - Change rule engine mode
- **ctl:ruleRemoveById=ID** - Remove rule by ID for current transaction
- **ctl:ruleRemoveByTag=TAG** - Remove rules by tag for current transaction
- **ctl:ruleRemoveTargetById=ID;VARIABLE** - Remove target from rule
- **ctl:ruleRemoveTargetByTag=TAG;VARIABLE** - Remove target from rules by tag

**Example:**

```apache
# Disable rule engine for static files
SecRule REQUEST_URI "\.(css|js|png|jpg|gif)$" \
    "id:100,phase:1,pass,nolog,ctl:ruleEngine=Off"

# Enable JSON body processing for API endpoints
SecRule REQUEST_URI "^/api/" \
    "id:101,phase:1,pass,nolog,ctl:requestBodyProcessor=JSON"

# Remove specific rule for trusted paths
SecRule REQUEST_URI "^/trusted/" \
    "id:102,phase:1,pass,nolog,ctl:ruleRemoveById=942100"

# Add audit log parts for suspicious requests
SecRule TX:suspicious "@eq 1" \
    "id:103,phase:5,pass,nolog,ctl:auditLogParts=+EKZ"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
