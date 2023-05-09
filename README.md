# NITRO

Niels' Image-processing Thesis on Reconstruction Operations


## Installing

```sh
git clone --recurse-submodules -j8 git@github.com:BugelNiels/nitro.git
```

In case `--recurse-submodules` was not used, you can initialize the submodules using:

```sh
git submodule init
git submodule update
```

## Known Bugs

- Zooming in a lot can cause the embedded widget to float outside of the node
- You can create an infinite loop by creating a cycle in the node graph

## TODO

- Drag and drop files (spawn image source node)
- Improved file name display of source model
- Image info node
- Additional comparison nodes (SSIM, MSE, )
- Custom port labels
- Drag nodes onto connection
- Delete existing connection when creating a new one
- Bottom align in-ports
- Progress bar for node editor
- Matrix data type
- Group node
- Improved spinbox (slider combo)
- Remove infite loop possibility