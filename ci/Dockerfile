FROM debian:buster

# See https://bugs.debian.org/cgi-bin/bugreport.cgi?bug=632779 for the explicit call to update-ccache-symlinks
ARG clang_v=10
ARG gcc_v=10
RUN echo 'deb http://deb.debian.org/debian/ testing main contrib non-free' >> /etc/apt/sources.list \
  && apt-get update \
  && apt-get upgrade -y \
  && apt-get install -y --no-install-recommends \
    build-essential \
    ca-certificates \
    ccache \
    clang-$clang_v \
    git \
    g++-$gcc_v \
    libsfml-dev \
    llvm-$clang_v-dev \
    ninja-build \
    wget \
  && update-ccache-symlinks \
  && apt-get clean
ENV CXX=clang++-$clang_v

ARG cmake_v=3.17.0
RUN wget --no-verbose https://github.com/Kitware/CMake/releases/download/v$cmake_v/cmake-$cmake_v-Linux-x86_64.tar.gz -O - \
  | tar --no-same-owner -zxC /usr --strip-components=1

RUN cd /usr/bin \
  && wget https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage \
  && mv linuxdeploy-x86_64.AppImage linuxdeploy \
  && chmod +x linuxdeploy

ARG imgui_v=v1.77
ARG imgui_sfml_v=v2.1
RUN git clone https://github.com/ocornut/imgui.git --branch $imgui_v /deps/imgui \
  && git clone https://github.com/eliasdaler/imgui-sfml --branch $imgui_sfml_v /deps/imgui-sfml \
  && cd /deps/imgui-sfml \
  && mkdir build && cd build \
  && cmake \
    -GNinja \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_INSTALL_PREFIX=/usr \
    -DIMGUI_DIR=/deps/imgui \
    .. \
  && cmake --build . \
  && cmake --install .
