+++
title = "SecRuleScript"
weight = 47
+++

**Description:** Define rule logic using Lua scripts.

**Syntax:** `SecRuleScript PATH "ACTIONS"`

This directive creates a special rule that decides whether to match by executing a Lua script. The main difference from SecRule is that there are no target and operator parameters. The script can access any variables from the WAF context and use Lua operators for testing. All Lua scripts are compiled and cached in memory at configuration load time; if you need to reload scripts, you must reload the entire configuration.

**Example:**

```apache
SecRuleScript /etc/wge/scripts/custom_check.lua "id:5001,phase:2,deny,msg:'Script check failed'"
```

**Case Sensitive:** Yes
