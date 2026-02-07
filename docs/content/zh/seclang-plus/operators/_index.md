+++
title = "操作符"
weight = 3
ordersectionsby = "title"
+++

以下为操作符，其输入数据来源于 variable，在实际上操作符仅关注单次输入是否匹配，如ARGS代表参数值，但是一个请求可能存在多个参数值，操作符只关注其中的一个，在规则中，只需有一个匹配成功就代表规则匹配成功。

下面的操作符从输入数据格式可以分为两类，一类接收字符串作为输入数据，一类接收整数作为输入源，几乎所有参数的数据都为字符串（除了&取数量和TX变量），当输入数据不符合其数据类型要求是会默认返回**不匹配**（如给正则匹配传入了一个整数）。

操作符也支持取反操作，如下面的规则检测参数不以admin开头：

```apache
SecRule ARGS "!@beginswith admin" "id:1001,phase:2,deny,t:none"
```

支持一个规则多个操作符，以符号|连接，它们之间是**或**的关系，只需要其中一个匹配成功即可，如：

```apache
SecRule ARGS "!@beginswith admin|@endswith user|@streq passwd" "id:1001,phase:2,deny,t:none"
```

另外几乎所有的操作符都支持**宏扩展**功能，即传入操作符的参数不是固定的，而是根据变量动态获取的，如：
```apache
SecRule REQUEST_HEADERS:Referer "@rx REQUEST_HEADERS:Host" "id:1001,phase:2,deny,t:none"
```
**注意：** 当 variable 中没有任何数据时，action 任何情况下都不会执行，即使使用了控制符如`!setvar`或者`*setvar`，noMatch 和 unconditionalMatch 也无效，任何情况下都会返回匹配失败。

| 关键字 | 是否实现| 是否支持宏扩展 |
|--------|------|------|
| [`@beginsWith`](./beginswith/) | 是 | 是 |
| [`@containsWord`](./containsword/) | 否 | - |
| [`@contains`](./contains/) | 是 | 是 |
| [`@detectSQLi`](./detectsqli/) | 是 | 否 |
| [`@detectXSS`](./detectxss/) | 是 | 否 |
| [`@endsWith`](./endswith/) | 是 | 是 |
| [`@eq`](./eq/) | 是 | 是 |
| [`@fuzzyHash`](./fuzzyhash/) | 否 | - |
| [`@ge`](./ge/) | 是 | 是 |
| [`@geoLookup`](./geolookup/) | 否 | - |
| [`@gt`](./gt/) | 是 | 是 |
| [`@inspectFile`](./inspectfile/) | 否 | - |
| [`@ipMatchF`](./ipmatchf/) | 否 | - |
| [`@ipMatchFromFile`](./ipmatchfromfile/) | 否 | - |
| [`@ipMatch`](./ipmatch/) | 是 | 否 |
| [`@le`](./le/) | 是 | 是 |
| [`@lt`](./lt/) | 是 | 是 |
| [`@noMatch`](./nomatch/) | 是 | 否 |
| [`@pmFromFile`](./pmfromfile/) | 是 | 否 |
| [`@pm`](./pm/) | 是 | 否 |
| [`@pmf`](./pmf/) | 是 | 否 |
| [`@rbl`](./rbl/) | 否 | - |
| [`@rsub`](./rsub/) | 否 | - |
| [`@rxGlobal`](./rxglobal/) | 否 | - |
| [`@rx`](./rx/) | 是 | 是 |
| [`@streq`](./streq/) | 是 | 是 |
| [`@strmatch`](./strmatch/) | 否 | - |
| [`@unconditionalMatch`](./unconditionalmatch/) | 是 | 否 |
| [`@validateByteRange`](./validatebyterange/) | 是 | 否 |
| [`@validateDTD`](./validatedtd/) | 否 | - |
| [`@validateSchema`](./validateschema/) | 否 | - |
| [`@validateUrlEncoding`](./validateurlencoding/) | 是 | 否 |
| [`@validateUtf8Encoding`](./validateutf8encoding/) | 否 | - |
| [`@verifyCC`](./verifycc/) | 否 | - |
| [`@verifyCPF`](./verifycpf/) | 否 | - |
| [`@verifySSN`](./verifyssn/) | 否 | - |
| [`@within`](./within/) | 是 | 是 |
| [`@xor`](./xor/) | 是 | 是 |