+++
title = "RULE"
weight = 69
+++

**Description:** Current rule information

**Syntax:** `RULE`

The RULE variable provides access to the metadata of the currently executing rule. Specific properties can be accessed via subkeys, such as RULE.id (rule ID), RULE.msg (rule message), RULE.severity (severity level), etc. Primarily used in logging and dynamic messages to reference information about the rule itself.

**Example:**

```apache
# Reference rule information in logs
SecRule ARGS "@detectSQLi" \
    "id:1047,phase:2,deny,msg:'Rule %{RULE.id} triggered: SQL injection detected',\
    severity:CRITICAL,tag:'attack-sqli'"
```
