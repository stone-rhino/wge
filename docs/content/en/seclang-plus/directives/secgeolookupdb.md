+++
title = "SecGeoLookupDb"
weight = 26
+++

**Description:** Configure the geolocation database file path for the @geoLookup operator.

**Syntax:** `SecGeoLookupDb PATH`

WGE uses MaxMind's GeoIP2 format database (.mmdb files) for IP address geolocation lookups. After configuring this directive, you can use the @geoLookup operator in rules to obtain geographic location information for client IPs and implement geolocation-based access control.

**Example:**

```
SecGeoLookupDb /usr/share/GeoIP/GeoLite2-Country.mmdb
```

**Case Sensitive:** Yes
