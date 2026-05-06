+++
title = "@pm"
weight = 9
+++

**Description:** Performs efficient multi-pattern parallel matching using the Aho-Corasick algorithm

**Syntax:** `"@pm keyword1 keyword2 keyword3 ..."`

@pm operator can search for multiple keywords simultaneously, separated by spaces. Compared to multiple @rx rules, @pm performs better when matching a large number of keywords. Matching is case insensitive.

**Example:**

```apache
# Detect common SQL injection keywords
SecRule ARGS "@pm select union insert update delete" \
    "id:1001,phase:2,deny,msg:'SQL keyword detected'"

# Detect sensitive file access
SecRule REQUEST_URI "@pm /etc/passwd /etc/shadow .htaccess" \
    "id:1002,phase:1,deny,msg:'Sensitive file access'"
```

**Parameter Type:** `string (space-separated keyword list)`
