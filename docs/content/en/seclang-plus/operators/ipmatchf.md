+++
title = "@ipMatchF"
weight = 19
+++

**Description:** Loads IP address list from file for matching (alias for @ipMatchFromFile)

**Syntax:** `"@ipMatchF /path/to/ip-list.txt"`

@ipMatchF is the short form of @ipMatchFromFile. Loads IP address/CIDR range lists from an external file for matching. One IP address or CIDR range per line in the file; lines starting with # are treated as comments and ignored. Suitable for managing large IP address lists such as dynamically updated blacklists and CDN node lists.

**Example:**

```apache
# Load IP blacklist from file
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/ip-blacklist.txt" \
    "id:1107,phase:1,deny,msg:'IP is in blacklist'"

# Load trusted proxy IP list from file
SecRule REMOTE_ADDR "@ipMatchF /etc/wge/trusted-proxies.txt" \
    "id:1108,phase:1,pass,nolog,setvar:tx.is_trusted_proxy=1"
```

**File format example (/etc/wge/ip-blacklist.txt):**

```
# Malicious IP blacklist
# Updated: 2024-01-01
1.2.3.4
5.6.7.8
10.0.0.0/8
192.168.100.0/24
```

**Parameter Type:** `string (file path)`

**Case Sensitive:** Yes
