# NITRO

Niels' Image-processing Thesis on Reconstruction Operations


## Installing

```sh
git clone --recurse-submodules -j8 git@github.com:BugelNiels/continuous-bit-depth-images.git
```

In case `--recurse-submodules` was not used, you can initialize the submodules using:

```sh
git submodule init
git submodule update
```

## Known Bugs

- Viewer node: image position moves when switching between images of different resolutions
- Grid lines are not drawn nicely when zooming in/out
- Keyboard shortcuts in the image viewer don't work
- Disconnecting the image viewer node does not properly center the empty image again