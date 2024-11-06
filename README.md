# Nanamo

GL-based webpage display.

This is intended to display overlays of a certain plugin for a certain MMORPG on Linux.

## Building and running

First, setup build directory:
```
$ meson setup build -Dprefix=/opt
```

Due to the way libCEF is linked in, you'll need to install the program first before it can be started:
```
$ cd build
$ meson install --destdir ./install
```

Once it's being installed you can then start the program:
```
$ cd install/opt/nanamo
$ LD_LIBRARY_PATH=. ./nanamo -tr <overlay_url>
```
