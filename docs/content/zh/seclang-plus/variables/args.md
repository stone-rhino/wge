+++
title = "ARGS"
weight = 1
+++

**描述:** 所有请求参数

**语法:** `ARGS | ARGS:Key`

ARGS 包含多个部分，其包括请求行的参数、application/x-www-form-urlencoded、multipart/form-data和application/json类型请求体中的参数，同时若SecParseXmlIntoArgs设置为On或者OnlyArgs时，application/xml请求体解析出来的内容也会包含在ARGS中。

**注意：** 当此variable在第一阶段使用时，其相当于`ARGS_GET`。

**示例:**

```apache
SecRule ARGS:user "@rx admin" "id:1001,deny,msg:'Test'"
```


**ARGS 解析示例**

## 1. JSON 请求体解析 (application/json)

对于json中value类型为数字、bool值、null类型的，WGE会只保存其key，因为这些value通常不具备攻击能力。

解析过程中会默认发生一次js解码，包括转义和\u等编码，例如\\"会被自动解码为"，其能力与转换函数中的jsDecode一致。

**请求:**

```
POST /api/users HTTP/1.1
Content-Type: application/json

{
  "username": "admin",
  "password": "secret\"123",
  "profile": {
    "email": "admin@example.com",
    "role": "administrator"
  },
  "tags": ["vip", "test"],
  "enable":false,
  "num":10,
  "body":null
}
```
**ARGS 解析结果:**

“-”表示参数名或者参数值为空：

| 参数名 | 参数值 |
|--------|------|
| username | admin |
| password | secret"123 |
| profile | - |
| email | admin@example.com |
| role | administrator |
| tags | - |
| - | vip |
| - | test |
| enable | - |
| num | - |
| body | - |

## 2. XML 请求体解析 (application/xml)

**请求:**
```
POST /api/order HTTP/1.1
Content-Type: application/xml

 
```

**ARGS 解析结果:**

“-”表示参数名或者参数值为空：

| 参数名 | 参数值 |
|--------|------|
| username | admin |

## 3. URL-encoded 表单 (application/x-www-form-urlencoded)

**请求:**
```
POST /login HTTP/1.1
Content-Type: application/x-www-form-urlencoded

username=admin&password=secret%26123&remember=true
```


**ARGS 解析结果:**

“-”表示参数名或者参数值为空：

| 参数名 | 参数值 |
|--------|------|
| username | admin |

## 4. Multipart 表单 (multipart/form-data)

**请求:**

```
POST /upload HTTP/1.1
Content-Type: multipart/form-data; boundary=----WebKitFormBoundary

------WebKitFormBoundary
Content-Disposition: form-data; name="title"

My Document
------WebKitFormBoundary
Content-Disposition: form-data; name="file"; filename="report.pdf"
Content-Type: application/pdf

[binary content]
------WebKitFormBoundary--
```

**ARGS 解析结果:**

“-”表示参数名或者参数值为空：

| 参数名 | 参数值 |
|--------|------|
| username | admin |