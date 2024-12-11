# srsecurity
## 一、项目简介
由于ModSecurity性能太差，对其优化后提升了一倍QPS左右（8C8G的环境中从700多到1500）。如果想要继续优化的话受限于其原有代码的架构，很难再有大的提升。如果想要有大的提升必须得大刀阔斧调整其架构，索性重写。
## 二、项目目标
* 在8C8G的环境中QPS能达到1W左右
* 完全兼容现有CRS规则
## 三、编译
### 3.1 前置依赖条件
* 安装jdk
```shell
apt install openjdk-21-jdk-headless
```
* 下载antlr4
```shell
cd /usr/local/lib
curl -O https://www.antlr.org/download/antlr-4.13.2-complete.jar
```
* 在/etc/profile中加入
```shell
export CLASSPATH=".:/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH"
alias antlr4='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.Tool'
alias grun='java -Xmx500M -cp "/usr/local/lib/antlr-4.13.2-complete.jar:$CLASSPATH" org.antlr.v4.gui.TestRig'
```