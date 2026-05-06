+++
title = "@pmFromFile"
weight = 11
+++

**描述:** 从文件加载关键字进行并行匹配

**语法:** `@pmFromFile /path/to/keywords.txt`

**输入数据类型:** `string (文件路径)`

当 pm 中关键词太多会导致规则过长，可读性下降，这时候就可以使用操作符 pmFromFile 替代，其支持将所有需要匹配的关键词放在文件中，需要保证WGE对于该文件可读。

pmFromFile 与 pm 一样都是使用 hyperscan 实现的，为了增强 pmFromFile 的能力，我们增加了一些控制符以便使用者可以方便的控制多模匹配。

**可选项：**
- `##!+ i`：大小写不区分
- `##!+ -i`：大小写区分
- `##!+ l`：字符串匹配
- `##!+ -l`：正则匹配
- `##!^ prefix_string`：后续所有模式前加上前缀prefix_string
- `##!$ suffix_string`：后续所有模式前加上后缀suffix_string
- `##`：提前结束，后续模式不匹配
- `#`：注释，需要注意前缀不要符合上述格式

其中除了`##!+ l`和`##!+ -l`是全局仅生效一个，即对于一个 pmFromFile 中的所有模式正则匹配，和字符串匹配是二选一的，也即是最后一个。

其它是可以部分生效的，如可以指定范围大小写不区分，执行部分大小写区分：
```
# 下面的匹配大小不区分
##!+ i
admin
user
passwd
# 下面的匹配大小写区分
##!+ -i
root
bash
echo
```
前缀和后缀的设置也同理可以部分设置，同时可以直接使用`##!^ `和`##!$ `清空前缀和后缀。

**默认：**大小写不区分，无前后缀，字符串匹配。

**示例:**


```apache
# 从文件加载恶意 User-Agent 列表
SecRule REQUEST_HEADERS:User-Agent "@pmFromFile /etc/wge/bad-ua.txt" \
    "id:1001,phase:1,deny,msg:'Malicious User-Agent'"
```