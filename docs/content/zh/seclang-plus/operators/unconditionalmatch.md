+++
title = "@unconditionalMatch"
weight = 31
+++

**描述:** 无条件匹配 (总是返回 true)

**语法:** `@unconditionalMatch`

**输入数据类型:** `int|string`

@unconditionalMatch 无条件匹配成功，与 noMatch 相同，当存在取反即 !@unconditionalMatch 相当于 @noMatch。

**示例:**
```apache
# 在规则链中使用
SecRule ARGS "@rx user" \
    "id:1118,phase:1,chain,pass"
    SecRule &MACTHED_VARS "@unconditionalMatch" \
        "setvar:tx.is_post_request+=1"
```