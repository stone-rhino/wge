+++
title = "SecDefaultAction"
weight = 39
+++

**Description:** Define the default action list for a specific phase.


**Syntax:** `SecDefaultAction "ACTIONS"`


**Case Sensitive:** Yes


SecDefaultAction sets a series of actions that will be inherited by all rules in the same phase. Therefore, SecDefaultAction must contain a phase. If there are multiple SecDefaultAction directives in the same phase, the later one will replace the earlier one. SecDefaultAction mainly affects three aspects:

1. **Transformation functions**: Transformation functions in SecDefaultAction will be executed by all rules in the same phase, and executed before their own transformation functions, unless the rule has t:none;
2. **Disruptive actions**: Rules using **block** will determine whether to block based on whether the DefaultAction in the same phase has disruptive actions (such as allow, deny);
3. **Post-match actions**: Actions like setvar, ctl, etc. - when rules in the same phase match successfully, they will first execute DefaultAction and then execute their own actions.


**Example:**


```apache
SecDefaultAction "phase:1,log,auditlog,pass"
SecDefaultAction "phase:2,log,auditlog,deny,status:403"
```
