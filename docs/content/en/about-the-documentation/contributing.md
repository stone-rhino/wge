+++
title = "Contributing to Documentation"
weight = 1
+++

Welcome to contribute to the WGE documentation! Whether it's fixing typos, improving content, or adding new sections, your contributions will help us improve the documentation quality.

If you want to contribute to the documentation, please follow these steps:

1. **Fork the Repository**: Visit the [WGE repository](https://github.com/stone-rhino/wge.git) on GitHub and click the "Fork" button to copy the repository to your account. This repository contains both WGE source code and documentation.

2. **Clone the Repository**: Clone your forked repository to your local computer:
   ```bash
   git clone https://github.com/your-username/wge.git
   ```

3. **Create a Branch**: In your local repository, create a new branch for your changes:
   ```bash
   git checkout -b docs-your-branch-name
   ```

4. **Edit Documentation**: Use your preferred text editor to modify or add documentation content. WGE documentation is written in Markdown format and located in the `docs` directory.

5. **Multi-language Support**: Please ensure your changes apply to all supported language versions. If you only modify one language version, please consider providing corresponding translations for other language versions. Currently supported languages include English (located in `docs/content/en`) and Chinese (located in `docs/content/zh`).

6. **Create a Pull Request**: After completing your changes, please follow the steps in the [Code Contribution Guide](https://github.com/stone-rhino/wge/blob/main/CONTRIBUTING.md) to submit a pull request for review.

### How to Preview the Local Documentation Site?

The WGE documentation is built using the [Hugo](https://gohugo.io/) static site generator and the [Hugo-theme-relearn](https://github.com/McShelby/hugo-theme-relearn.git) theme. To preview the documentation site locally, follow these steps:

1. **Install Hugo**: Make sure you have Hugo installed. You can get installation instructions from the [Hugo official website](https://gohugo.io/getting-started/installing/). The minimum version requirement is 0.141.0.

2. **Install the Theme**: In the root directory of your local repository, run the following command to fetch the theme:
   ```bash
   git submodule update --init --recursive
   ```
   The theme is included in the repository as a submodule, so you just need to run the above command for submodule initialization.

3. **Start the Local Server**: In the root directory of your local repository, run the following command to start the Hugo local server:
   ```bash
   cd docs
   hugo server
   ```

### Advanced Usage

For more advanced Hugo usage and theme configuration options, please refer to the [Hugo official documentation](https://gohugo.io/documentation/) and the [Hugo-theme-relearn documentation](https://mcshelby.github.io/hugo-theme-relearn).
