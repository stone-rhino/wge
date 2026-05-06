+++
title = "@ipMatch"
weight = 18
+++

**Description:** IP address/CIDR matching

**Syntax:** `"@ipMatch ip1,ip2,cidr1,cidr2..."`

@ipMatch checks if the variable value matches the specified IP address or CIDR range. Supports both IPv4 and IPv6 addresses. Multiple addresses/ranges are separated by commas. Commonly used for IP whitelisting, blacklisting, geo-restrictions, and other access control scenarios. More efficient than using @rx for IP matching.

**Example:**

```apache
# IP whitelist - allow specific IPs to bypass detection
SecRule REMOTE_ADDR "@ipMatch 192.168.1.0/24,10.0.0.0/8" \
    "id:1103,phase:1,pass,nolog,ctl:ruleEngine=Off"

# IP blacklist - block known malicious IPs
SecRule REMOTE_ADDR "@ipMatch 1.2.3.4,5.6.7.8" \
    "id:1104,phase:1,deny,msg:'Blacklisted IP address'"

# Restrict admin panel access by source IP
SecRule REQUEST_URI "@beginsWith /admin" \
    "id:1105,phase:1,chain"
    SecRule REMOTE_ADDR "!@ipMatch 192.168.1.0/24" \
        "deny,msg:'Admin panel access from external network forbidden'"

# Detect internal IP (possible SSRF attack)
SecRule ARGS "@ipMatch 127.0.0.0/8,10.0.0.0/8,172.16.0.0/12,192.168.0.0/16" \
    "id:1106,phase:2,deny,msg:'Internal IP access detected, possible SSRF'"
```

**Parameter Type:** `string`

**Case Sensitive:** Yes
