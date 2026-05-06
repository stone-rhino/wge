+++
title = "SecCollectionTimeout"
weight = 21
+++

**Description:** Configure the expiration time (seconds) for persistent collections.


**Syntax:** `SecCollectionTimeout SECONDS`


**Default:** 3600


**Case Sensitive:** Yes


**Implemented:** No


Persistent collections (such as IP, SESSION, USER, etc.) are used to track data across requests. This directive defines the lifetime of collection records. After this time, collection records will be automatically cleaned up. Setting this value appropriately can balance security tracking needs and storage space usage.


**Example:**


```apache
SecCollectionTimeout 3600
```
