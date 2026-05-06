+++
title = "集成到 Apache"
weight = 1
+++

## 概述

WGE Apache Connector 用于将 WGE 安全引擎集成到 Apache HTTP Server 中，使其能够在请求处理过程中对 HTTP 流量进行检测与拦截。

---

## 配置指令

### wge

**语法：**
```apache
wge on | off;
```
**作用域：**
```apache
rsrc, access
```
**默认值：**
```apache
off
```
**说明：** 用于开启或关闭 WGE 检测功能。当设置为 `off` 时，WGE 不会对请求进行检测。

### wge_rules_file

**语法：**
```apache
wge_rules_file <规则文件路径>;
```
**作用域：**
```apache
rsrc, access
```
**默认值：**
```apache
无
```
**说明：** 指定需要加载的规则文件路径。请确保 apache 运行用户对该文件具有读取权限。

### wge_rules

**语法：**
```apache
wge_rules "<规则内容>";
```
**作用域：**
```apache
rsrc, access
```
**默认值：**
```apache
无
```
**说明：** 支持以内联字符串的方式定义规则，适用于简单配置或测试场景。

## 配置示例
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
## 规则加载顺序

对于同一请求，WGE 会按照配置层级从上到下加载规则：

1. `1.conf`（父级配置）
2. `2.conf`（子级配置）

这种机制确保父级规则优先生效，适用于统一基础规则 + 局部扩展的场景。

## 请求阻断行为（Request Body 阶段）

由于 Apache input filter 的限制，在 request body 阶段被 WGE 拦截的请求实际返回 `HTTP 400(Bad Request)` 而非预期的 `HTTP 403(Forbidden)` 。

原因：Apache input filter 层无法直接设置 HTTP 状态码，阻断行为会被解释为协议错误。

## 当前限制

### WGE 依赖处理

当前构建方式对 WGE 依赖较强，即使系统已安装 WGE 预编译版本，也可能需要重新编译。

### APXS 路径限制

当前构建流程可能无法自动识别非标准路径下的 apxs。

### Apache 开发依赖缺失

如果系统未安装 Apache 开发包，会导致构建失败：
```bash
# Ubuntu / Debian
apt-get install apache2-dev

# CentOS / RHEL
yum install httpd-devel
```