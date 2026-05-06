+++
title = "@validateSchema"
weight = 25
+++

**描述:** 根据提供的XML Schema验证XML DOM节点树

**语法:** `@validateSchema xsd_path`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
SecRule XML "@validateSchema /path/to/xml.xsd" "phase:2,id:191,deny,msg:'Failed DTD validation'"
```