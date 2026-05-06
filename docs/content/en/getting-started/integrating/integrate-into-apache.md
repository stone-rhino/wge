+++
title = "Integrate into Apache"
weight = 1
+++

## Overview

The WGE Apache Connector integrates the WGE security engine into Apache HTTP Server, enabling inspection and interception of HTTP traffic during request processing.

---

## Configuration Directives

### wge

**Syntax:**
```apache
wge on | off;
```
**Scope:**
```apache
rsrc, access
```
**Default:**
```apache
off
```
**Description:** Enables or disables WGE inspection. When set to `off`, WGE does not inspect requests.

### wge_rules_file

**Syntax:**
```apache
wge_rules_file <path-to-rules-file>;
```
**Scope:**
```apache
rsrc, access
```
**Default:**
```apache
none
```
**Description:** Specifies the path to the rule file to load. Ensure the Apache process user has read permission on this file.

### wge_rules

**Syntax:**
```apache
wge_rules "<rule-content>";
```
**Scope:**
```apache
rsrc, access
```
**Default:**
```apache
none
```
**Description:** Allows rules to be defined as inline strings. Useful for simple configurations or testing.

## Configuration Example

```apache
LoadModule wge_module modules/mod_wge.so

<IfModule wge_module>
    wge Off
    wge_rules_file 1.conf
</IfModule>

<Location />
    <IfModule wge_module>
        wge On
        wge_rules "SecDefaultAction \"phase:1,log,auditlog,pass\""
        wge_rules "SecDefaultAction \"phase:2,log,auditlog,pass\""
        wge_rules_file /opt/apache/conf/1.conf
    </IfModule>
</Location>
```

## Rule Loading Order

For a given request, WGE loads rules from the configuration hierarchy top-down:

1. `1.conf` (parent scope)
2. `2.conf` (child scope)

This ensures parent-level rules take effect first, which suits the pattern of shared base rules with local extensions.

## Request Blocking Behavior (Request Body Phase)

Due to a limitation in Apache's input filter mechanism, requests blocked by WGE during the request body phase return `HTTP 400 (Bad Request)` rather than the expected `HTTP 403 (Forbidden)`.

This occurs because the Apache input filter layer cannot directly set the HTTP status code, so the blocking action is interpreted as a protocol error.

## Current Limitations

### WGE Dependency Handling

The current build process has a strong dependency on WGE source. Even if a pre-built version of WGE is installed on the system, recompilation may still be required.

### APXS Path Detection

The current build process may not automatically locate `apxs` when it is installed in a non-standard path.

### Missing Apache Development Headers

If Apache development packages are not installed, the build will fail. Install them first:

```bash
# Ubuntu / Debian
apt-get install apache2-dev

# CentOS / RHEL
yum install httpd-devel
```
