FROM ubuntu:24.04
RUN rm /bin/sh && ln -s /bin/bash /bin/sh
RUN apt update && apt install -y build-essential cmake gcc-14 g++-14 mesa-utils libwebkitgtk-6.0-dev libadwaita-1-dev git libopencv-dev wget curl
ENV NVM_DIR=/usr/local/nvm
ENV NODE_VERSION=16.20.2
ENV CC=/usr/bin/gcc-14
ENV CXX=/usr/bin/g++-14

# install nvm
# https://github.com/creationix/nvm#install-script
RUN curl --silent -o- https://raw.githubusercontent.com/creationix/nvm/v0.31.2/install.sh | bash

# install node and npm
RUN source $NVM_DIR/nvm.sh \
    && nvm install $NODE_VERSION \
    && nvm alias default $NODE_VERSION \
    && nvm use default 
# add node and npm to path so the commands are available
ENV NODE_PATH=$NVM_DIR/v$NODE_VERSION/lib/node_modules
ENV PATH=$NVM_DIR/versions/node/v$NODE_VERSION/bin:$PATH

RUN npm i -g @saucer-dev/cli@6.1.1

RUN cd /tmp && git clone https://github.com/nadjieb/cpp-mjpeg-streamer.git && cd /tmp/cpp-mjpeg-streamer && mkdir build && cd build && cmake .. && make -j4 && make install && ldconfig -v && cd / && rm -rf /tmp/cpp-mjpeg-streamer 

ENV WEBKIT_DISABLE_SANDBOX_THIS_IS_DANGEROUS=1

