We welcome contributions to WGE! Please follow these guidelines to maximize the chances of your contribution being accepted:
## Communication
- Before starting work on a new feature, please open an issue to discuss your idea. This helps us avoid duplicated efforts and ensures that your contribution aligns with the project's goals.
- If you find a bug, please open an issue follow the [bug_report.md](.github/ISSUE_TEMPLATE/bug_report.md). This helps us address issues more effectively. And if you have a fix, feel free to submit a pull request and reference the issue in your PR description.
## Code Style
- Follow the existing code style in the project. This includes naming conventions, indentation, and formatting. For the most part, we follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
- Use `clang-format` to format your code. The project uses a [.clang-format](./.clang-format) file to define the coding style. You can run the following command to format your code:
  ```bash
  ./clang-format.sh 
  ```
- Ensure that your code passes the existing tests and add new tests for any new functionality you introduce.
## Submitting Pull Requests
- Fork the repository and create a new branch for your changes. Use a descriptive name for your branch that reflects the changes you are making. The branch name should be in the format `feat/your-feature-name` or `fix/your-bugfix-name`.
- The commit messages should be clear and descriptive. Use the following format for commit messages:
  ```
  type(scope): subject
  ```
  Where `type` is one of `feat`, `fix`, `docs`, `style`, `refactor`, `test`, and `scope` is an optional scope that describes the area of the code affected by the change.
  The length of the subject should be limited to 50 characters, and the body should be wrapped at 72 characters.
  The last line of the commit message should be a reference to the issue number, if applicable, in the format `Fixes #issue_number` or `Related to #issue_number`.
- For example:
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
- Ensure that your pull request is based on the latest version of the `main` branch to avoid merge conflicts. You can use the following commands to update your branch:
  ```bash
  git checkout main
  git pull origin main
  git checkout your-branch-name
  git rebase main
  ```
- If your pull request includes multiple commits, consider squashing them into a single commit with a clear commit message that summarizes the changes.
- If your pull request is rejected, please don't be discouraged. We appreciate your effort and encourage you to continue contributing in the future.
- If you have any questions or need help with your pull request, feel free to ask for assistance in the project's communication channels.
## PR review policy
- We aim to review pull requests within a reasonable timeframe, but please be patient as we may have other priorities.
- If you have not received a response within a week, feel free to ping us in the pull request comments.
- We may request changes to your pull request before merging it. Please address these requests promptly to keep the review process moving smoothly.
- If your pull request is large or complex, we may ask you to break it down into smaller, more manageable pieces. This makes it easier for us to review and understand your changes.
- If your pull request is related to a specific issue, please reference that issue in the pull request description.