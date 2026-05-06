+++
title = "Include"
weight = 61
+++

**Description:** Include an external configuration file into the current configuration.


**Syntax:** `Include /path/to/file.conf`


**Case Sensitive:** Yes


This directive includes the contents of an external configuration file at the current position. It supports both absolute paths and relative paths (relative to the current configuration file). Wildcards can be used to include multiple files.

This is commonly used to organize rule sets into separate files for easier management.


**Example:**


```apache
Include /etc/modsecurity/crs/crs-setup.conf
Include /etc/modsecurity/crs/rules/*.conf
```
