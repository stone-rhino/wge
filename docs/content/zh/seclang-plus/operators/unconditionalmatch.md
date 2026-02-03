+++
title = "@unconditionalMatch"
weight = 31
+++

**描述:** 无条件匹配 (总是返回 true)


**语法:** `"@unconditionalMatch"`


@unconditionalMatch 无条件返回匹配成功（true）。不检查任何变量值，始终触发规则动作。常用于创建无条件执行的规则，如初始化变量、设置默认值、记录所有请求等场景。通常与 SecAction 配合使用效果相同，但在需要指定变量的规则链中很有用。


**示例:**


```apache
# 初始化事务变量
SecRule REQUEST_URI "@unconditionalMatch" \
    "id:1116,phase:1,pass,nolog,setvar:tx.anomaly_score=0"

# 记录所有请求
SecRule REMOTE_ADDR "@unconditionalMatch" \
    "id:1117,phase:1,pass,log,msg:'请求来自: %{REMOTE_ADDR}'"

# 在规则链中使用
SecRule REQUEST_METHOD "@streq POST" \
    "id:1118,phase:1,chain,pass"
    SecRule REQUEST_URI "@unconditionalMatch" \
        "setvar:tx.is_post_request=1"
```


**参数类型:** `无`


**区分大小写:** 是
