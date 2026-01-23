简体中文 | [English](CONTRIBUTING.md)

我们欢迎大家为 WGE 做出贡献！请遵循以下指南，以最大化您的贡献被接受的可能性：
## 交流
- 在开始开发新功能之前，请先打开一个 issue 讨论您的想法。这有助于我们避免重复工作，并确保您的贡献符合项目的目标。
- 如果您发现了一个 bug，请按照 [bug_report.md](.github/ISSUE_TEMPLATE/bug_report.md) 打开一个 issue。这有助于我们更有效地解决问题。如果您有修复方案，欢迎提交一个 pull request，并在 PR 描述中引用相关的 issue。
## 代码风格
- 遵循项目中现有的代码风格。这包括命名约定、缩进和格式化。我们主要遵循 [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)。
- 使用 `clang-format` 格式化您的代码。项目使用 [.clang-format](./.clang-format) 文件来定义编码风格。您可以运行以下命令来格式化代码：
  ```bash
  ./clang-format.sh 
  ```
- 确保您的代码通过现有的单元测试，并为您引入的任何新功能添加新的单元测试。
## Git钩子
- 我们使用 Git钩子来确保代码质量和一致性。请运行以下脚本，在您的本地仓库中设置Git钩子：
  ```bash
  ./tools/setup_git_hooks.sh
  ```
- `pre-commit` 钩子会在每次提交之前自动使用 `clang-format` 格式化您的代码。这有助于保持项目代码风格的一致性。
- `commit-msg` 钩子会验证您的提交信息，确保其符合项目的提交信息规范。请参阅下面的提交信息格式部分了解更多细节。
## 提交拉取请求
- Fork 仓库并为您的更改创建一个新分支。使用描述性名称为您的分支命名，反映您所做的更改。分支名称应采用 `feat/your-feature-name` 或 `fix/your-bugfix-name` 格式。
- 提交信息应清晰且具有描述性。使用以下格式编写提交信息：
  ```
  type(scope): subject
  ```
  其中，`type` 是 `feat`、`fix`、`docs`、`style`、`refactor`、`test` 中的一个，`scope` 是一个可选的范围，描述了此次更改影响的代码区域。
  提交信息的主题长度应限制在50个字符以内，正文应在72个字符处换行。
  提交信息的最后一行应引用相关的 issue 编号（如果有的话），格式为 `Fixes #issue_number` 或 `Related to #issue_number`。
- 例如：
  ```
  feat(variable): add some variable for persistent

  Add variables:
  - GLOBAL
  - RESOURCE
  - IP
  - SESSION
  - USER
  ```

  ```
  fix(test): fix hyperscan error in the crs test

  The test must be run in a separate thread to avoid the thread local
  scratch space of hyperscan was not correctly clone from the main thread.
  Because the other test cases may use the hyperscan scanner, and the
  thread local scratch was initialized by the other test cases in the main
  thread, so if we run this test in the main thread, the scratch space
  will be not correctly initialized.

  Fixes #13
  ```
- 确保您的拉取请求基于 `main` 分支的最新版本，以避免合并冲突。您可以使用以下命令来更新您的分支：
  ```bash
  git checkout main
  git pull origin main
  git checkout your-branch-name
  git rebase main
  ```
- 如果您的拉取请求包含多个提交，考虑将它们压缩成一个具有清晰提交信息的单一提交，以总结所做的更改。
- 如果您的拉取请求被拒绝，请不要气馁。我们感谢您的努力，并鼓励您继续为项目做出贡献。
- 如果您有任何问题或需要帮助，请随时在项目的交流渠道中寻求帮助。
## PR 审核政策
- 我们旨在在合理的时间内审查拉取请求，但请耐心等待，因为我们可能有其他优先事项。
- 如果您在一周内未收到回复，请随时在拉取请求评论中提醒我们。
- 我们可能会在合并您的拉取请求之前请求您进行更改。请及时处理这些请求，以保持审核过程的顺利进行。
- 如果您的拉取请求较大或较复杂，我们可能会要求您将其拆分成更小、更易管理的部分。这使我们更容易审查和理解您的更改。
- 如果您的拉取请求与特定问题相关，请在拉取请求描述中引用该问题。