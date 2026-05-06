+++
title = "集成到 Nginx"
weight = 3
+++
## 概述

WGE Nginx Connector 用于将 WGE 安全引擎集成到 Nginx 中，使其能够在请求处理过程中对 HTTP 流量进行检测与拦截。

---

## 使用方式

WGE Nginx Connector 提供以下配置指令：


### wge

**语法：**
```nginx
wge on | off;
```
**作用域：**
```nginx
http, server, location
```
**默认值：**
```nginx
off
```
**说明：** 用于开启或关闭 WGE 检测功能。当设置为 `off` 时，WGE 不会对请求进行检测。

### wge_rules_file

**语法：**
```nginx
wge_rules_file <规则文件路径>;
```
**作用域：**
```nginx
http, server, location
```
**默认值：**
```nginx
无
```
**说明：** 指定需要加载的规则文件路径。请确保 Nginx 运行用户对该文件具有读取权限。

### wge_rules

**语法：**
```nginx
wge_rules "<规则内容>";
```
**作用域：**
```nginx
http, server, location
```
**默认值：**
```nginx
无
```
**说明：** 支持以内联字符串的方式定义规则，适用于简单配置或测试场景。

## 配置示例

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

## 动态模块加载
如果 WGE 以动态模块方式编译，需要在 Nginx 配置中显式加载：
```nginx
load_module /usr/local/nginx/modules/ngx_http_wge_module.so;
```

## 配置继承

WGE 支持 Nginx 不同配置层级之间的继承（http / server / location）。

规则加载顺序为：从上层到下层依次加载。

例如：
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

对于 `location /` 的请求，规则加载顺序为：

1. `1.conf`（http 层）
2. `2.conf`（server 层）
3. `3.conf`（location 层）

这种机制适用于：

在 http 层定义通用规则
在 server / location 层进行补充或覆盖

## 当前限制

- **固定 Nginx 版本：** 当前构造脚本使用固定的 Nginx 版本（1.29.3），暂不支持自定义版本。
- **覆盖已有 Nginx：** 如果系统中已经安装 Nginx，那么构建过程会直接覆盖原有 Nginx文件。