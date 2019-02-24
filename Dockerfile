FROM ubuntu:bionic

RUN apt-get update

# WT
# Try 21cae5a11513c70f1a6d451a6dd237e4c0f7b4e7 if this fails
RUN apt-get -y install git cmake g++-8 libboost-all-dev build-essential && \
    git clone https://github.com/emweb/wt.git && \
    cd wt && \
    mkdir build && \
    cd build && \
    cmake .. && \
    make -j8 && \
    cd ../../

# TineMCE
#RUN apt-get install tinymce && \
#    ln -s /wt/resources/ /resources && \
#    mkdir /resources/tiny_mce && \
#    cp -r /usr/share/tinymce/www/* /resources/tiny_mce/

RUN if [ ! -d /published_pipelines ]; then mkdir /published_pipelines; fi

#CEREAL
RUN cd / && git clone https://github.com/USCiLab/cereal.git

#Now RosettaPipeline gets rebuilt every time
ARG CACHEBUST=1

# Rosetta Pipeline
RUN git clone https://github.com/JackMaguire/RosettaPipeline.git && \
    cd RosettaPipeline && \
    mkdir bin build && \
    ln -s ../wt/src wt_src && \
    ln -s /wt/build/Wt/WConfig.h wt_src/Wt/WConfig.h && \
    find ../../wt/build/src | grep "\.so" | xargs -n1 ln -s && \
    ln -s ../wt/build/src/http/libwthttp.so libwthttp2.so && \
    ln -s ../wt/build/src/libwt.so libwt2.so && \
    ln -s ../wt/resources/ && \
    make all -j8

EXPOSE 8080

CMD cd RosettaPipeline && bin/graph_view_app --docroot . --http-address 0.0.0.0 --http-port 8080
