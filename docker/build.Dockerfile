# Use an Ubuntu base image
FROM ubuntu:20.04

# Install necessary dependencies
RUN apt-get -y update && DEBIAN_FRONTEND=noninteractive apt-get -y install \
    git \
    cmake \
    python3 \
    python3-pip \
    build-essential \
    libdbus-1-3 \
    libpulse-mainloop-glib0 \
    libopengl0  \
    libegl1  \
    libfontconfig1  \
    libharfbuzz0b  \
    libgl1-mesa-glx \
    libsm6 -y \
    cmake \
    libopencv-dev \
    build-essential  \
    patchelf \
    libfuse2 \
    libxkbcommon-x11-0 \
    libxcb-*    \
    libgl1-mesa-dev -y

RUN apt-get upgrade -y

RUN pip3 install aqtinstall


ARG QT=6.5.1

RUN aqt install-qt linux desktop ${QT} -m all --outputdir /lib/qt6
# Copy the source code to the container
COPY . /app

ENV PATH /lib/qt6/${QT}/gcc_64/bin:$PATH
ENV QT_PLUGIN_PATH /lib/qt/${QT}/gcc_64/plugins/
ENV QML_IMPORT_PATH /lib/qt/${QT}/gcc_64/qml/
ENV QML2_IMPORT_PATH /lib/qt/${QT}/gcc_64/qml/

RUN apt-get install -y \
    libx11-xcb-dev  \
    libglu1-mesa-dev  \
    libxrender-dev libxi-dev \
    libxkbcommon-dev  \
    libxkbcommon-x11-dev \
    wget

# Set the working directory
WORKDIR /app/scripts

RUN chmod +x clean.sh
RUN ./clean.sh

RUN chmod +x build-image.sh
ENTRYPOINT ["./build-image.sh"]
