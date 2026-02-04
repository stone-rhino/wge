+++
title = "@rx"
weight = 1
+++

**Description:** Matches variable values using Perl Compatible Regular Expressions (PCRE)

**Syntax:** `"@rx pattern"`

@rx is the most powerful and commonly used operator in SecLang. It uses PCRE syntax and supports all standard regular expression features. If no operator is specified, @rx is used as the default operator.

**Example:**

```apache
# Detect SQL injection keywords
SecRule ARGS "@rx (?i:select|union|insert|update|delete|drop)" \
    "id:1001,phase:2,deny,msg:'SQL Injection detected'"

# Detect XSS attack patterns
SecRule ARGS "@rx <script[^>]*>.*?</script>" \
    "id:1002,phase:2,deny,msg:'XSS Attack detected'"
```

**Parameter Type:** `string (regular expression)`
