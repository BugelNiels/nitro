# Use an Ubuntu base image
FROM ubuntu:latest

# Install necessary dependencies
RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata
RUN apt-get install cmake qt6-base-dev libopencv-dev build-essential libgl1-mesa-dev -y

# Copy the source code to the container
COPY . /app

# Set the working directory
WORKDIR /app
RUN rm build/ -rf

RUN mkdir -p build
WORKDIR build/
RUN cmake .. -DCMAKE_BUILD_TYPE=Release && make -j$(nproc)
