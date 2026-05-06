+++
title = "ARGS"
weight = 1
+++

**Description:** All request parameters

**Syntax:** `ARGS | ARGS:Key`

ARGS contains parameters from multiple sources, including request-line parameters, request bodies of type `application/x-www-form-urlencoded`, `multipart/form-data`, and `application/json`, and, when `SecParseXmlIntoArgs` is set to `On` or `OnlyArgs`, content parsed from `application/xml` request bodies.

**Note:** When this variable is used in phase 1, it behaves the same as `ARGS_GET`.

**Example:**

```apache
SecRule ARGS:user "@rx admin" "id:1001,deny,msg:'Test'"
```

**ARGS Parsing Examples**

## 1. JSON Request Body Parsing (`application/json`)

For JSON values of type number, boolean, or null, WGE stores only the key because those values usually do not carry attack payloads.

During parsing, one JS decode pass is applied by default, including escapes and `\u` encodings. For example, `\\"` is automatically decoded to `"`. Its behavior is equivalent to the `jsDecode` transformation.

**Request:**

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

**ARGS parsing result:**

`-` indicates that the parameter name or parameter value is empty:

| Parameter Name | Parameter Value |
|----------------|-----------------|
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

## 2. XML Request Body Parsing (`application/xml`)

**Request:**

```
POST /api/order HTTP/1.1
Content-Type: application/xml

```

**ARGS parsing result:**

`-` indicates that the parameter name or parameter value is empty:

| Parameter Name | Parameter Value |
|----------------|-----------------|
| username | admin |

## 3. URL-Encoded Form (`application/x-www-form-urlencoded`)

**Request:**

```
POST /login HTTP/1.1
Content-Type: application/x-www-form-urlencoded

username=admin&password=secret%26123&remember=true
```

**ARGS parsing result:**

`-` indicates that the parameter name or parameter value is empty:

| Parameter Name | Parameter Value |
|----------------|-----------------|
| username | admin |

## 4. Multipart Form (`multipart/form-data`)

**Request:**

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

**ARGS parsing result:**

`-` indicates that the parameter name or parameter value is empty:

| Parameter Name | Parameter Value |
|----------------|-----------------|
| username | admin |
