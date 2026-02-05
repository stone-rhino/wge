+++
title = "SecRuleUpdateTargetById"
weight = 44
+++

**描述:** 根据 ID 更新规则的参数列表。


**语法:** `SecRuleUpdateTargetById ID VARIABLES`


**区分大小写:** 是

更新指定ID规则的变量（VARIABLES），可以尝试增加、删除变量等操作;

**注意:**此规则是作为配置的指令，其并不是运行时的，也就是说skipafter等对这并不有效；且其必须在其更新的规则之后加载。

**示例:**

若ID为1001的规则为：

```apache
SecRule ARGS|REQUEST_HEADERS "@rx admin" "id:1001,phase:1,deny,tag:'template_rule',t:none"
```

**增加变量:**

下面的示例会让1001规则增加对于REQUEST_LINE的检测：
```apache
SecRuleUpdateTargetById 1001 REQUEST_LINE
```

**删除变量:**

下面的示例会删除指定1001规则对ARGS的检测：
```apache
SecRuleUpdateTargetById 1001 !ARGS
```

**删除特定子变量:**

下面的示例会让1001规则不检测参数名为username的参数：
```apache
SecRuleUpdateTargetById 1001 !ARGS:username
```

**使用正则:**

下面的示例会让1001规则不检测参数名符合指定正则的参数：
```apache
SecRuleUpdateTargetById 1001 !ARGS:/[a-zA-Z]{100,}/
```