+++
title = "动作"
weight = 4
ordersectionsby = "title"
+++

动作根据其执行时机可以分为以下三种类型：

- **配置类动作（Configuration Actions）**：如 `id`、`phase` 等，用于标明规则的元数据信息。
- **匹配触发动作（Match-Triggered Actions）**：如 `setvar`、`ctl` 等，在单条规则匹配后执行的动作。这里的"单条规则"指的是一个 `SecRule` 等，而不是父子规则作为一个整体。
- **规则结束时动作（Disruptive Actions）**：如 `deny`、`allow` 等，在整条规则（包括子规则）匹配完成后执行的动作，通常会直接影响后续规则的执行。

| 关键字 | 说明 |
|--------|------|
| [`id`](./id/) | 规则的唯一标识符（必填） |
| [`phase`](./phase/) | 处理阶段（1-5） |
| [`msg`](./msg/) | 日志消息 |
| [`severity`](./severity/) | 严重级别（0-7） |
| [`tag`](./tag/) | 规则标签 |
| [`ver`](./ver/) | 规则版本 |
| [`rev`](./rev/) | 规则修订号 |
| [`accuracy`](./accuracy/) | 准确度等级（1-9） |
| [`maturity`](./maturity/) | 成熟度等级（1-9） |
| [`deny`](./deny/) | 拒绝请求 |
| [`drop`](./drop/) | 断开连接 |
| [`allow`](./allow/) | 允许请求通过 |
| [`allow:phase`](./allowphase/) | 允许请求通过当前阶段 |
| [`allow:request`](./allowrequest/) | 允许整个请求通过 |
| [`redirect`](./redirect/) | 重定向到指定 URL |
| [`pass`](./pass/) | 继续处理后续规则 |
| [`block`](./block/) | 使用默认的规则结束时动作 |
| [`log`](./log/) | 记录到错误日志 |
| [`nolog`](./nolog/) | 不记录到错误日志 |
| [`auditlog`](./auditlog/) | 记录到审计日志 |
| [`noauditlog`](./noauditlog/) | 不记录到审计日志 |
| [`logdata`](./logdata/) | 记录附加数据 |
| [`capture`](./capture/) | 捕获正则匹配内容 |
| [`multiMatch`](./multimatch/) | 对所有匹配执行动作 |
| [`setvar`](./setvar/) | 设置变量 |
| [`expirevar`](./expirevar/) | 设置变量过期时间 |
| [`setenv`](./setenv/) | 设置环境变量 |
| [`setuid`](./setuid/) | 设置用户 ID |
| [`setsid`](./setsid/) | 设置会话 ID |
| [`setrsc`](./setrsc/) | 设置资源 ID |
| [`initcol`](./initcol/) | 初始化持久化集合 |
| [`chain`](./chain/) | 链接规则 |
| [`skip`](./skip/) | 跳过指定数量的后续规则 |
| [`skipAfter`](./skipafter/) | 跳转到指定标记的规则 |
| [`ctl`](./ctl/) | 运行时修改引擎配置 |
| [`exec`](./exec/) | 执行外部脚本或命令 |
| [`status`](./status/) | 设置 HTTP 响应状态码 |
| [`xmlns`](./xmlns/) | 定义 XML 命名空间 |
