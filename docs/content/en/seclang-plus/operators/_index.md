+++
title = "Operators"
weight = 3
ordersectionsby = "title"
+++

Operators consume data from variables. In practice, an operator only evaluates one input item at a time. For example, `ARGS` may contain multiple parameter values, but an operator evaluates each value independently. A rule is considered matched if any evaluated value matches.

From the input data perspective, operators can be divided into two categories: string input and integer input. Most variable data are strings (except count variables such as `&...` and some `TX` usages). If the provided input does not satisfy the operator's required type, the operator returns **no match**.

Operators also support negation. Example:

```apache
SecRule ARGS "!@beginswith admin" "id:1001,phase:2,deny,t:none"
```

A rule can contain multiple operators joined by `|`, which means **OR** relation. Only one sub-operator needs to match:

```apache
SecRule ARGS "!@beginswith admin|@endswith user|@streq passwd" "id:1001,phase:2,deny,t:none"
```

Most operators support macro expansion, so operator parameters can be dynamically resolved from variables:

```apache
SecRule REQUEST_HEADERS:Referer "@rx %{REQUEST_HEADERS:Host}" "id:1001,phase:2,deny,t:none"
```

**Note:** When a variable has no data, actions are not executed in any case. This remains true even with control prefixes such as `!setvar` or `*setvar`. `@noMatch` and `@unconditionalMatch` are also ineffective in this scenario and still return no match.

| Keyword | Implemented | Macro Expansion |
|--------|------|------|
| [`@beginsWith`](./beginswith/) | Yes | Yes |
| [`@containsWord`](./containsword/) | No | - |
| [`@contains`](./contains/) | Yes | Yes |
| [`@detectSQLi`](./detectsqli/) | Yes | No |
| [`@detectXSS`](./detectxss/) | Yes | No |
| [`@endsWith`](./endswith/) | Yes | Yes |
| [`@eq`](./eq/) | Yes | Yes |
| [`@fuzzyHash`](./fuzzyhash/) | No | - |
| [`@ge`](./ge/) | Yes | Yes |
| [`@geoLookup`](./geolookup/) | No | - |
| [`@gt`](./gt/) | Yes | Yes |
| [`@inspectFile`](./inspectfile/) | No | - |
| [`@ipMatchF`](./ipmatchf/) | No | - |
| [`@ipMatchFromFile`](./ipmatchfromfile/) | No | - |
| [`@ipMatch`](./ipmatch/) | Yes | No |
| [`@le`](./le/) | Yes | Yes |
| [`@lt`](./lt/) | Yes | Yes |
| [`@noMatch`](./nomatch/) | Yes | No |
| [`@pmFromFile`](./pmfromfile/) | Yes | No |
| [`@pm`](./pm/) | Yes | No |
| [`@pmf`](./pmf/) | Yes | No |
| [`@rbl`](./rbl/) | No | - |
| [`@rsub`](./rsub/) | No | - |
| [`@rxGlobal`](./rxglobal/) | No | - |
| [`@rx`](./rx/) | Yes | Yes |
| [`@streq`](./streq/) | Yes | Yes |
| [`@strmatch`](./strmatch/) | No | - |
| [`@unconditionalMatch`](./unconditionalmatch/) | Yes | No |
| [`@validateByteRange`](./validatebyterange/) | Yes | No |
| [`@validateDTD`](./validatedtd/) | No | - |
| [`@validateSchema`](./validateschema/) | No | - |
| [`@validateUrlEncoding`](./validateurlencoding/) | Yes | No |
| [`@validateUtf8Encoding`](./validateutf8encoding/) | No | - |
| [`@verifyCC`](./verifycc/) | No | - |
| [`@verifyCPF`](./verifycpf/) | No | - |
| [`@verifySSN`](./verifyssn/) | No | - |
| [`@within`](./within/) | Yes | Yes |
| [`@xor`](./xor/) | Yes | Yes |
