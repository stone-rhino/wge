+++
title = "SecRule"
weight = 37
+++

**Description:** Define a security rule.

**Syntax:** `SecRule VARIABLES "OPERATOR" "ACTIONS"`

**Version:** 1.0

SecRule is the most important directive in SecLang, used to define rules for detecting and responding to security threats. A rule consists of three parts:

- **Variables (VARIABLES):** Specify the data sources to inspect
- **Operator (OPERATOR):** Define the matching conditions
- **Actions (ACTIONS):** Define the behavior after rule match

**Basic Example:**

```apache
SecRule ARGS "@contains <script>" \
    "id:1001,phase:2,deny,status:403,msg:'XSS Attack Detected'"
```

**Multiple Variables Example:**

```apache
SecRule ARGS|REQUEST_HEADERS "@rx (?i:eval\s*\()" \
    "id:1002,phase:2,deny,status:403,msg:'Code Injection Detected'"
```

**Using Transformations Example:**

```apache
SecRule ARGS "@rx select.*from" \
    "id:1003,phase:2,t:lowercase,t:removeWhitespace,deny,msg:'SQL Injection'"
```

**Case Sensitive:** Yes
