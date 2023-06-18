# Use an Ubuntu base image
FROM ubuntu:latest

# Install necessary dependencies
RUN apt-get update
RUN apt-get install \
    libopengl0  \
    libegl1  \
    libfontconfig1  \
    libharfbuzz0b  \
    libgl1-mesa-glx \
    libsm6 -y

# Copy the source code to the container
RUN mkdir /app
COPY nitro.AppImage /app

# Set the working directory
WORKDIR /app

# Binary can now be run using ./squash-root/AppRun
RUN ./nitro.AppImage --appimage-extract
