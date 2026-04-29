+++
title = "Integrate into Nginx"
weight = 3
+++

## Overview

The WGE Nginx Connector integrates the WGE security engine into Nginx, enabling inspection and interception of HTTP traffic during request processing.

---

## Configuration Directives

### wge

**Syntax:**
```nginx
wge on | off;
```
**Scope:**
```nginx
http, server, location
```
**Default:**
```nginx
off
```
**Description:** Enables or disables WGE inspection. When set to `off`, WGE does not inspect requests.

### wge_rules_file

**Syntax:**
```nginx
wge_rules_file <path-to-rules-file>;
```
**Scope:**
```nginx
http, server, location
```
**Default:**
```nginx
none
```
**Description:** Specifies the path to the rule file to load. Ensure the Nginx process user has read permission on this file.

### wge_rules

**Syntax:**
```nginx
wge_rules "<rule-content>";
```
**Scope:**
```nginx
http, server, location
```
**Default:**
```nginx
none
```
**Description:** Allows rules to be defined as inline strings. Useful for simple configurations or testing.

## Configuration Example

```nginx
server {
    wge off;

    location / {
        root /var/www/html;

        wge on;
        wge_rules "SecRuleEngine On";
        wge_rules "SecRequestBodyLimitAction Reject";
        wge_rules_file /usr/local/nginx/conf/my_rule.conf;
    }
}
```

## Dynamic Module Loading

If WGE is compiled as a dynamic module, load it explicitly in your Nginx configuration:

```nginx
load_module /usr/local/nginx/modules/ngx_http_wge_module.so;
```

## Configuration Inheritance

WGE supports inheritance across Nginx configuration scopes (`http` / `server` / `location`).

Rules are loaded in order from the outermost scope to the innermost scope.

For example:

```nginx
http {
    wge_rules_file /usr/local/nginx/conf/1.conf;

    server {
        wge_rules_file /usr/local/nginx/conf/2.conf;

        location / {
            wge on;
            wge_rules_file /usr/local/nginx/conf/3.conf;
        }
    }
}
```

For a request matching `location /`, rules are loaded in the following order:

1. `1.conf` (http scope)
2. `2.conf` (server scope)
3. `3.conf` (location scope)

This mechanism is well-suited for defining common rules at the `http` level and adding supplements or overrides at the `server` / `location` level.

## Current Limitations

- **Fixed Nginx version**: The current build script uses a fixed Nginx version (1.29.3). Custom versions are not yet supported.
- **Overwrites existing Nginx**: If Nginx is already installed on the system, the build process will overwrite the existing Nginx files.
