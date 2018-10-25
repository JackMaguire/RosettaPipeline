FROM ubuntu:bionic

RUN apt-get update

RUN apt-get -y install git cmake g++-5 libboost-all-dev build-essential && \
    git clone https://github.com/JackMaguire/RosettaPipeline.git && \
    git clone https://github.com/emweb/wt.git && \
    cd wt && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j8 && \
    cd ../../RosettaPipeline && \
    mkdir bin build && \
    ln -s ../wt/src wt_src && \
    ln -s /wt/build/Wt/WConfig.h wt_src/Wt/WConfig.h && \
    find ../../wt/build/src | grep "\.so" | xargs -n1 ln -s && \
    ln -s ../wt/build/src/http/libwthttp.so libwthttp2.so && \
    ln -s ../wt/build/src/libwt.so libwt2.so && \
#    ln -s ../wt/resources/
    ln -s ../wt/resources/ && \
    make all -j8


CMD RosettaPipeline/bin/save_and_load_graph