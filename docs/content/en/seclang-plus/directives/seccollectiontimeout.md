+++
title = "SecCollectionTimeout"
weight = 21
+++

**Description:** Configure the expiration time for persistent collections in seconds.

**Syntax:** `SecCollectionTimeout SECONDS`

**Default:** 3600

Persistent collections (such as IP, SESSION, USER, etc.) are used to track data across requests. This directive defines the time-to-live for collection records. After this time expires, collection records will be automatically cleaned up. Setting this value appropriately balances security tracking needs and storage space usage.

**Example:**

```
SecCollectionTimeout 3600
```

**Case Sensitive:** Yes
