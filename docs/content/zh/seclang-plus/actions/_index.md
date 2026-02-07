+++
title = "动作"
weight = 4
ordersectionsby = "title"
+++

动作根据其执行时机可以分为三种：配置类动作（Configuration Actions）、匹配触发动作（Match-Triggered Actions）、规则结束时动作（Disruptive Actions）

**配置类动作：**如id、phase等，其是标明规则的信息的。

**匹配触发动作：**如setvar、ctl等，其是在单条规则匹配后执行的动作，这里的单条规则只的是一个Rule等，而不是父子规则这种作为一个整体。

**规则结束时动作：**如deny、allow等，其是需要整条规则（包括子规则）后执行的动作，其通常会直接影响后续规则的执行。

**注意：** 动作`t`同样属于动作，但是因为其独特的运行时机和类型繁多，所以将其单独分类为转换函数，其具体描述见**转换函数**一节。

| 关键字 | 说明 |
|--------|------|
| [`id`](./id/) | - |
| [`phase`](./phase/) | - |
| [`msg`](./msg/) | - |
| [`severity`](./severity/) | - |
| [`tag`](./tag/) | - |
| [`ver`](./ver/) | - |
| [`rev`](./rev/) | - |
| [`accuracy`](./accuracy/) | - |
| [`maturity`](./maturity/) | - |
| [`deny`](./deny/) | - |
| [`drop`](./drop/) | - |
| [`allow`](./allow/) | - |
| [`redirect`](./redirect/) | - |
| [`pass`](./pass/) | - |
| [`block`](./block/) | - |
| [`log`](./log/) | - |
| [`nolog`](./nolog/) | - |
| [`auditlog`](./auditlog/) | - |
| [`noauditlog`](./noauditlog/) | - |
| [`logdata`](./logdata/) | - |
| [`capture`](./capture/) | - |
| [`multiMatch`](./multimatch/) | - |
| [`setvar`](./setvar/) | - |
| [`expirevar`](./expirevar/) | - |
| [`setenv`](./setenv/) | - |
| [`setuid`](./setuid/) | - |
| [`setsid`](./setsid/) | - |
| [`setrsc`](./setrsc/) | - |
| [`initcol`](./initcol/) | - |
| [`exec`](./exec/) | - |
