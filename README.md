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
- Zooming out in the image viewer and then moving gives some wonky results