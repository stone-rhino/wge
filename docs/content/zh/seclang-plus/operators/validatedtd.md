+++
title = "@validateDTD"
weight = 24
+++

**描述:** 根据 DTD 验证 XML DOM节点树

**语法:** `@validateDTD dts_path`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule XML "@validateDTD /path/to/xml.dtd" "id:1001,phase:2,deny,msg:'Failed DTD validation'"
```