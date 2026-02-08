+++
title = "Actions"
weight = 4
ordersectionsby = "title"
+++

Actions can be categorized into three types based on their execution timing:

- **Configuration Actions**: Such as `id`, `phase`, etc., used to define rule metadata.
- **Match-Triggered Actions**: Such as `setvar`, `ctl`, etc., executed when a single rule matches. A "single rule" here refers to one `SecRule`, not a parent-child rule chain as a whole.
- **Disruptive Actions**: Such as `deny`, `allow`, etc., executed after the entire rule (including chained sub-rules) completes matching, typically affecting subsequent rule processing.

| Keyword | Description |
|---------|-------------|
| [`id`](./id/) | Unique rule identifier (required) |
| [`phase`](./phase/) | Processing phase (1-5) |
| [`msg`](./msg/) | Log message |
| [`severity`](./severity/) | Severity level (0-7) |
| [`tag`](./tag/) | Rule tag |
| [`ver`](./ver/) | Rule version |
| [`rev`](./rev/) | Rule revision |
| [`accuracy`](./accuracy/) | Accuracy level (1-9) |
| [`maturity`](./maturity/) | Maturity level (1-9) |
| [`deny`](./deny/) | Deny request |
| [`drop`](./drop/) | Drop connection |
| [`allow`](./allow/) | Allow request to pass |
| [`allow:phase`](./allowphase/) | Allow request to pass current phase |
| [`allow:request`](./allowrequest/) | Allow the entire request to pass |
| [`redirect`](./redirect/) | Redirect to specified URL |
| [`pass`](./pass/) | Continue processing subsequent rules |
| [`block`](./block/) | Use default disruptive action |
| [`log`](./log/) | Log to error log |
| [`nolog`](./nolog/) | Do not log to error log |
| [`auditlog`](./auditlog/) | Log to audit log |
| [`noauditlog`](./noauditlog/) | Do not log to audit log |
| [`logdata`](./logdata/) | Log additional data |
| [`capture`](./capture/) | Capture regex match content |
| [`multiMatch`](./multimatch/) | Execute action for all matches |
| [`setvar`](./setvar/) | Set variable |
| [`expirevar`](./expirevar/) | Set variable expiration time |
| [`setenv`](./setenv/) | Set environment variable |
| [`setuid`](./setuid/) | Set user ID |
| [`setsid`](./setsid/) | Set session ID |
| [`setrsc`](./setrsc/) | Set resource ID |
| [`initcol`](./initcol/) | Initialize persistent collection |
| [`chain`](./chain/) | Chain rules together |
| [`skip`](./skip/) | Skip specified number of subsequent rules |
| [`skipAfter`](./skipafter/) | Skip to a rule with specified marker |
| [`ctl`](./ctl/) | Modify engine configuration at runtime |
| [`exec`](./exec/) | Execute external script or command |
| [`status`](./status/) | Set HTTP response status code |
| [`xmlns`](./xmlns/) | Define XML namespace |
