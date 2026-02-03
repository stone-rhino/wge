+++
title = "REMOTE_ADDR"
weight = 31
+++

**Description:** Client IP address

**Syntax:** `REMOTE_ADDR`

REMOTE_ADDR contains the IP address of the client making the request. This is one of the most commonly used variables for implementing IP blacklists/whitelists, rate limiting, and other security policies. Often used with the @ipMatch operator to match IP addresses or CIDR ranges.

**Example:**

```apache
# IP whitelist - skip checks for internal IPs
SecRule REMOTE_ADDR "@ipMatch 10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1038,phase:1,pass,nolog,allow"

# IP blacklist - block specific IPs
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1039,phase:1,deny,status:403,msg:'IP has been banned'"
```
