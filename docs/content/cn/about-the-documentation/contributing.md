+++
title = "文档贡献"
weight = 1
+++

欢迎贡献 WGE 文档！无论是修正错别字、改进内容，还是添加新章节，您的贡献都将帮助我们提升文档质量。
如果您想贡献文档，请按照以下步骤操作：
1. **Fork 仓库**：在 GitHub 上访问[WGE仓库](https://github.com/stone-rhino/wge.git)，点击“Fork”按钮，将仓库复制到您的账户中。该仓库包含WGE的源代码和文档。
2. **克隆仓库**：将您 Fork 的仓库克隆到本地计算机：
   ```bash
   git clone https://github.com/your-username/wge.git
   ```
3. **创建分支**：在本地仓库中，为您的更改创建一个新分支：
   ```bash
   git checkout -b improve-documentation
   ```
4. **编辑文档**：使用您喜欢的文本编辑器修改或添加文档内容。WGE 文档使用 Markdown 格式编写，位于 `docs` 目录下。
5. **多语言支持**：请确保您的更改适用于所有支持的语言版本。如果您只修改了某个语言版本，请考虑为其他语言版本提供相应的翻译。目前支持的语言包括英语（位于`docs/content/en`）和中文（位于`docs/content/cn`）。
6. **创建拉取请求**：完成更改后，请遵循[代码贡献指南](https://github.com/stone-rhino/wge/blob/main/CONTRIBUTING_CN.md)中的步骤，提交拉取请求以供审查。
### 如何预览本地文档站点？
WGE文档使用[Hugo](https://gohugo.io/)静态站点生成器以及[Hugo-theme-relearn](https://github.com/McShelby/hugo-theme-relearn.git)主题构建。要在本地预览文档站点，请按照以下步骤操作：
1. **安装Hugo**：确保您已安装Hugo。您可以从[Hugo官方网站](https://gohugo.io/getting-started/installing/)获取安装说明，最低版本要求为0.141.0。
2. **安装主题**：在本地仓库的根目录下，运行以下命令以获取主题：
   ```bash
   git submodule update --init --recursive
   ```
   主题以子模块的形式包含在仓库中，所以您只要运行上述命令进行子模块初始化即可。
3. **启动本地服务器**：在本地仓库的根目录下，运行以下命令以启动Hugo本地服务器：
   ```bash
   cd docs
   hugo server
   ```
### 高级用法
更多Hugo高级用法以及主题配置选项，请参阅[Hugo官方文档](https://gohugo.io/documentation/)以及[Hugo-theme-relearn文档](https://mcshelby.github.io/hugo-theme-relearn)。
