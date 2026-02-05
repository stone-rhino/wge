+++
title = "Include"
weight = 61
+++

**描述:** 将外部配置文件包含到当前配置中。


**语法:** `Include /path/to/file.conf`


**区分大小写:** 是


此指令在当前位置包含外部配置文件的内容。支持绝对路径和相对路径（相对于当前配置文件）。可以使用通配符来包含多个文件。

通常用于将规则集组织到单独的文件中，以便于管理。


**示例:**


```apache
Include /etc/modsecurity/crs/crs-setup.conf
Include /etc/modsecurity/crs/rules/*.conf
```
