<a name="readme-top"></a>

[![stars](https://img.shields.io/github/stars/bugelniels/nitro.svg?style=for-the-badge)](https://github.com/BugelNiels/nitro/stargazers)
[![Github All Releases](https://img.shields.io/github/downloads/BugelNiels/nitro/total.svg?style=for-the-badge)](https://github.com/BugelNiels/nitro/releases/latest)
[![watchers](https://img.shields.io/github/watchers/bugelniels/nitro.svg?style=for-the-badge)](https://github.com/BugelNiels/nitro/watchers)
[![license](https://img.shields.io/github/license/bugelniels/nitro.svg?style=for-the-badge)](https://github.com/BugelNiels/nitro/blob/main/LICENCE.md)
[![follow](https://img.shields.io/github/followers/bugelniels.svg?style=for-the-badge&label=Follow&maxAge=2592000)](https://github.com/BugelNiels?tab=followers)
[![LinkedIn](https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555)](https://nl.linkedin.com/in/nielsbugel)

<br />
<div align="center">
  <a href="https://bugelniels.github.io/nitro/">
    <img src="appdir/logo.png" alt="Logo" width="128" height="128">
  </a>
<h3 align="center" style="padding-top: 0">NITRO</h3>
  <p align="center">
    Powerful and flexible node editor that facilitates building complex combinations of various image processing techniques.
    <br />
    <a href="https://bugelniels.github.io/nitro/"><strong>Explore the docs »</strong></a>
    <br />
    <br />
    <a href="https://github.com/BugelNiels/nitro/releases/latest">Latest Release</a>
    ·
    <a href="https://github.com/BugelNiels/nitro/blob/main/docs/INSTALL.md">Install Instructions</a>
    ·
    <a href="https://github.com/BugelNiels/nitro/issues">Report Bug</a>
    ·
    <a href="https://github.com/BugelNiels/nitro/issues">Request Feature</a>
  </p>
</div>

![Node Editor](screenshots/front.png)

## About the project

NITRO is a powerful tool for building complex image processing routines. Instead of needing to modify source code or
scripts to process images in different ways, you can use this visual node editor to do that instead. The project was
developed with extendability in mind, making it easy to add your own types nodes, and widgets. For more information on
this, read the docs. The UI was inspired by
Blender's [Shader Editor](https://docs.blender.org/manual/en/latest/editors/shader_editor.html)

The project was originally developed for my Master thesis at the University of Groningen. That is also the origin of the
name NITRO: Niels' Image-processing Thesis on Reconstruction Operations.

To get a local copy up and running, download the [latest release](https://github.com/BugelNiels/nitro/releases/latest).
Once downloaded, you should have a file
called `nitro-x86_64.AppImage`. You can run this using:

```shell
chmod u+x nitro-x86_64.AppImage
./nitro-x86_64.AppImage
```

Alternatively, you can read through the [Installation Instructions](docs/INSTALL.md) to build the project from the source code.

## Disclaimer

The project is still in development and can be considered in early-alpha. As such, I am still working hard on fixing
bugs and adding new features. If you encounter any, please feel free to create an issue.

## Features

- Flexible Node system with support for numerous image processing techniques
- Supports both grayscale and color images
- Easily extendable with new nodes and data types
- Module system configurable at compile time
- Image Viewer widget
- 3D Image Viewer widget
- Saving and loading support
- Undo/Redo support
- Dark Mode and Light Mode

## Controls

Nodes can be spawned via the side menu (drag and drop). Connections can be made between nodes via drag and drop as well.

A few useful shortcuts:

- Holding `ctrl+shift` while clicking on a node will spawn a viewer node for said node.
- Pressing `shift+a` will open a menu that allows for node spawning.
- Selecting two nodes and pressing `f` will attempt to create a connection between them.
- Pressing `shift+f` will enter first person mode in the 3D viewer dock.

## Demo

![Node Editor](screenshots/demo.gif)
![Image Viewer](screenshots/imviewer.gif)
![3D Viewer](screenshots/3dviewer.gif)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

