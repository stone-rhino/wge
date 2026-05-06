+++
title = "setvar"
weight = 25
+++

**Description:** Set variable

**Syntax:** `setvar:collection.key=value`

The setvar action creates, modifies, or deletes collection variables. This is a core action for implementing anomaly scoring, state tracking, and passing data between rules. It supports multiple operations:

- **setvar:tx.key=value** - Set variable to specified value
- **setvar:tx.key=+value** - Increment variable by specified value (numeric addition)
- **setvar:tx.key=-value** - Decrement variable by specified value
- **setvar:!tx.key** - Delete variable

Multiple setvar actions can be used in a single rule. Supports macro expansion using %{VARIABLE} to reference other variable values.

**Example:**

```apache
# Initialize anomaly score
SecAction "id:1,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# Increment score when attack detected
SecRule ARGS "@detectSQLi" \
    "id:2,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL Injection +5'"

# Set flag variable
SecRule REQUEST_URI "@beginsWith /api" \
    "id:3,phase:1,pass,nolog,setvar:tx.is_api=1"

# Set variable using macro expansion
SecRule REQUEST_HEADERS:User-Agent "@rx (bot|spider)" \
    "id:4,phase:1,pass,setvar:tx.client_type=%{MATCHED_VAR}"

# Delete variable
SecRule TX:temporary "@eq 1" \
    "id:5,phase:5,pass,nolog,setvar:!tx.temporary"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
