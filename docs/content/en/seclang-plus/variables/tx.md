+++
title = "TX"
weight = 81
+++

**Description:** Transaction variable collection (temporary variables)

**Syntax:** `TX:key`

TX (Transaction) is the most commonly used collection variable, used to store and pass data within a single transaction (request-response cycle). TX variables are automatically destroyed when the transaction ends. Set via the setvar action and accessed via TX:key. TX:0 through TX:9 are special variables that automatically store regex capture group results from the @rx operator. Additionally, TX variables are commonly used to implement anomaly scoring patterns (e.g., tx.anomaly_score).

**Example:**

```apache
# Set anomaly score
SecRule ARGS "@detectSQLi" \
    "id:1052,phase:2,pass,setvar:tx.anomaly_score=+5,msg:'SQL injection score added'"

# Block after score accumulates
SecRule TX:anomaly_score "@ge 10" \
    "id:1053,phase:2,deny,msg:'Anomaly score exceeded threshold: %{TX.anomaly_score}'"

# Using regex capture groups
SecRule REQUEST_URI "@rx ^/user/(\d+)" \
    "id:1054,phase:1,pass,setvar:tx.user_id=%{TX.1}"
```
