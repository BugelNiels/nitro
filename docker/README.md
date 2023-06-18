# Docker Images

This directory contains two Docker images:

- **run.Dockerfile** This file simply runs the created app image to verify no additional packages need to be installed.
- **ubuntu.Dockerfile** This sets up a basic docker container with all the packages required for building the project
  from the source code.

Note that both Dockerfiles should be executed from the root of the project:
```shell
docker build -f docker/ubuntu.Dockerfile .
```
