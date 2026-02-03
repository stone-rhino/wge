+++
title = "SecRuleUpdateTargetById"
weight = 44
+++

**描述:** 根据 ID 更新规则的检查目标。

            
**语法:** `SecRuleUpdateTargetById ID VARIABLES`

            
**区分大小写:** 是

            
            
该指令将使用第二个参数提供的目标，向指定规则的当前目标列表追加（或替换）变量。
此功能适用于实现例外情况，即需要外部更新目标列表以排除对特定变量的检查。
前例中生成的有效规则将目标附加至变量列表末尾如下：
请注意，在目标规范中也可使用正则表达式：
请注意，若要在正则表达式中使用分组，必须使用单引号将其括起来。
您也可将目标替换为更适合环境的选项。例如，若需检查 REQUEST_URI 而非 REQUEST_FILENAME，可采用以下方式：
前例中生成的有效规则将变量列表开头的目标替换为：

            
**示例:**

            
```apache
SecRuleUpdateTargetById 1001 !ARGS:username
```
