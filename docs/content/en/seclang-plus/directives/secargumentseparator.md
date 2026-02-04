+++
title = "SecArgumentSeparator"
weight = 11
+++

**Description:** Specify the parameter separator character used for application/x-www-form-urlencoded content.

**Syntax:** `SecArgumentSeparator CHARACTER`

**Default:** &

This directive is required when the backend web application uses a non-standard parameter separator. Some applications use semicolons as separators. Do not change the default setting unless you confirm that the application you are protecting requires a different separator. If this directive is not correctly configured for each web application, parameters will not be parsed correctly, significantly reducing rule matching effectiveness.

**Example:**

```
SecArgumentSeparator &
```

**Case Sensitive:** Yes
