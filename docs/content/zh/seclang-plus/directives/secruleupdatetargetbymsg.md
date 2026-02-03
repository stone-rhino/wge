+++
title = "SecRuleUpdateTargetByMsg"
weight = 45
+++

**描述:** 根据消息内容更新规则的检查目标。

            
**语法:** `SecRuleUpdateTargetByMsg "MESSAGE" VARIABLES`

            
**区分大小写:** 是

            
            
该指令将使用第二个参数提供的目标，向指定规则的当前目标列表追加（或替换）变量。
此方法适用于实现例外情况，即需要外部更新目标列表以排除对特定变量的检测。
前例中生成的有效规则将目标附加至变量列表末尾如下：
您也可以完全替换目标列表，使其更适合您的环境。例如，若需检查 REQUEST_URI 而非 REQUEST_FILENAME，可采用以下方式：
前例中生成的有效规则将按如下方式将目标附加到变量列表末尾：

            
**示例:**

            
```apache
SecRuleUpdateTargetByMsg "SQL Injection" !ARGS:id
```
