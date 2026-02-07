+++
title = "@geoLookup"
weight = 35
+++

**描述:** 查询指定IP的对应的地理位置，结果会存入GEO集合中。


**语法:** `@geoLookup`

**输入数据类型:** `string`

**是否实现:** 否

此指令WGE可以正常解析，但是暂未实现该功能。

**示例:**

```apache
#配置地理定位数据库
SecGeoLookupDb /path/to/GeoLiteCity.dat

#查找IP地址
SecRule REMOTE_ADDR "@geoLookup" "phase:1,id:155,nolog,pass"

#阻止地理位置失败的IP地址
SecRule &GEO "@eq 0" "phase:1,id:156,deny,msg:'Failed to lookup IP'"
```