+++
title = "SecRule"
weight = 37
+++

**Description:** Define a security rule.


**Syntax:** `SecRule VARIABLES "OPERATOR" "ACTIONS"`

**Case Sensitive:** Yes

SecRule is the most important directive in SecLang, used to define rules for detecting and responding to security threats. A rule consists of three parts:


- **Variables (VARIABLES):** Specify the data sources to inspect, such as ARGS for query parameters
- **Operator (OPERATOR):** Define the matching conditions, including regex matching, comparison, etc.
- **Actions (ACTIONS):** Contains configuration actions like phase and id, transformation functions, and post-match actions like setvar

The execution logic of a SecRule: obtain the data to be matched from the variables (VARIABLES), process the data through transformation functions for encoding/decoding operations (such as t:urlDecode for URL decoding), perform the specified matching operation on the decoded data, execute post-match actions (such as setvar) after a successful match, and if there are chained rules, continue executing them.


**Basic Example:**


```apache
SecRule ARGS "@contains " \
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
