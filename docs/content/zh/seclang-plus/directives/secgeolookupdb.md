+++
title = "SecGeoLookupDb"
weight = 26
+++

**描述:** 配置地理位置数据库文件路径，用于 @geoLookup 操作符。


**语法:** `SecGeoLookupDb PATH`


**区分大小写:** 是



WGE 使用 MaxMind 的 GeoIP2 格式数据库（.mmdb 文件）进行 IP 地址地理位置查询。配置此指令后，可在规则中使用 @geoLookup 操作符获取客户端 IP 的地理位置信息，实现基于地理位置的访问控制。


**示例:**


```
SecGeoLookupDb /usr/share/GeoIP/GeoLite2-Country.mmdb
```
