FROM ubuntu:latest

# 避免交互式提示
ENV DEBIAN_FRONTEND=noninteractive

# 设置环境变量
ENV JAVA_VERSION=21
ENV ANTLR4_VERSION=4.13.2
ENV ANTLR4_JAR_PATH=/usr/local/lib/antlr-4.13.2-complete.jar

# 安装基础工具和编译环境
RUN apt-get update && apt-get install -y --no-install-recommends \
  wget \
  curl \
  git \
  make \
  automake \
  ragel \
  zip \
  gcc-13 \
  g++-13 \
  cmake \
  libyajl-dev \
  liblzma-dev \
  libxml2 \
  libxml2-dev \
  libcurl4-openssl-dev \
  libpcre3 \
  libpcre3-dev \
  ca-certificates \
  ninja-build \
  tar unzip \
  pkg-config \
  python3 \
  libtool \
  && rm -rf /var/lib/apt/lists/* && apt-get clean s

# 设置gcc-13为默认编译器
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 100 \
  && update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 100 \
  && update-alternatives --set gcc /usr/bin/gcc-13 \
  && update-alternatives --set g++ /usr/bin/g++-13


# 安装Java
RUN apt-get update && apt-get install -y --no-install-recommends \
  wget \
  && mkdir -p /opt/java \
  && wget -q https://github.com/adoptium/temurin21-binaries/releases/download/jdk-21.0.2%2B13/OpenJDK21U-jdk_x64_linux_hotspot_21.0.2_13.tar.gz -O /opt/java/openjdk.tar.gz \
  && tar -xzf /opt/java/openjdk.tar.gz -C /opt/java \
  && rm /opt/java/openjdk.tar.gz \
  && ln -s /opt/java/jdk-21.0.2+13 /opt/java/current

ENV JAVA_HOME=/opt/java/current
ENV PATH=$JAVA_HOME/bin:$PATH

# 安装ANTLR4
RUN wget -q https://www.antlr.org/download/antlr-${ANTLR4_VERSION}-complete.jar -O ${ANTLR4_JAR_PATH} \
  && echo 'export CLASSPATH=".:${ANTLR4_JAR_PATH}:$CLASSPATH"' >> /etc/profile \
  && echo 'alias antlr4="java -Xmx500M -cp \"${ANTLR4_JAR_PATH}:$CLASSPATH\" org.antlr.v4.Tool"' >> /etc/profile \
  && echo 'alias grun="java -Xmx500M -cp \"${ANTLR4_JAR_PATH}:$CLASSPATH\" org.antlr.v4.gui.TestRig"' >> /etc/profile

# 安装vcpkg
RUN git clone https://github.com/stone-rhino/vcpkg.git /opt/vcpkg \
  && cd /opt/vcpkg \
  && ./bootstrap-vcpkg.sh 

ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_DEFAULT_TRIPLET=x64-linux
ENV PATH=$VCPKG_ROOT:$PATH

# 定义 GITHUB_TOKEN 环境变量
ARG GITHUB_TOKEN
ARG GITHUB_REPOSITORY
ARG BRANCH_NAME
ARG GITHUB_ACTOR
# ENV GITHUB_TOKEN_DOCKER=$GITHUB_TOKEN
# ENV GITHUB_REPOSITORY_DOCKER=$GITHUB_REPOSITORY
# ENV BRANCH_NAME_DOCKER=${BRANCH_NAME}
# ENV GITHUB_ACTOR_DOCKER=GITHUB_ACTOR
# 编译wge
RUN  mkdir -p /home/runner/work/wge/

RUN git clone -b ${BRANCH_NAME} https://${GITHUB_ACTOR}:${GITHUB_TOKEN}@github.com/${GITHUB_REPOSITORY}.git --depth=1  /home/runner/work/wge/wge \
  && cd /home/runner/work/wge/wge  && git submodule update --init --depth=1 --recursive && cmake --preset=release \
  && rm /opt/vcpkg/buildtrees/* -rf && rm /home/runner/work/wge/* -rf && rm /opt/vcpkg/packages/* -rf 
#删除

# 清理缓存
RUN apt-get clean && rm -rf /var/lib/apt/lists/*

WORKDIR /workspace

# 设置默认命令
CMD ["/bin/bash"]