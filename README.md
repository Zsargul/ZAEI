![ZAEI Logo](img/logo.png)

**<h2 align="center">Zsargul's Artix Environment Initializer</h2>**

ZAEI (pronounced "zey"), is a tool inspired by [Luke Smith's LARBS](https://github.com/LukeSmithxyz/LARBS), for
automatically downloading and/or configuring my packages, graphical display, and other configurations that I use
on my Artix install. While inspired by Luke Smith's script, I felt like writing my own utility and fine-tuning
it to my own needs and specific setup. I did however choose to write this one in C instead of bash. While bash
would likely be more efficient and more lightweight, I like C and didn't want to just copy Luke Smith's scripts.

### Usage

TODO: Write this stuff once the program is actually usable

### Building from source

#### Building libconfig

If you want to compile this from source, you will also need to build [libconfig](https://github.com/hyperrealm/libconfig),
a statically linked dependency of ZAEI. Since it is statically linked, you _must_ build this from source. This is because,
while the offical arch repositories contain the `libconfig` package, the package only contains dynamic `.so` libraries 
installed into `/usr/lib`. Building from source gets you the static `.a` libs, which go into `/usr/include/lib`.

TODO: Check if using the AUR gets you those .a libs too

The specific version of libconfig that the current release binary was linked with is `libconfig 1.7.3`. The
quick instructions to build and install it are below, or you can read the more detailed instructions
[here](https://github.com/hyperrealm/libconfig/blob/master/INSTALL).

**Note:** libconfig has a long history and is very well maintained. You can likely use any newer version to build
with ZAEI and it'll still work, but I'm specifying the version used just incase.

#### Building libconfig 1.7.3

Install libconfig;s build dependencies:
```
sudo pacman -S autoconf automake libtool
```

Download and extract the source code:
```
mkdir tmp && cd tmp
wget https://hyperrealm.github.io/libconfig/dist/libconfig-1.7.3.tar.gz
tar -xvzf libconfig-1.7.3.tar.gz
cd libconfig-1.7.3
```

Run the configuration script, build, and install (You may have to use `sudo` when using `make`):
```
./configure
make install
```

The static libraries and headers will be installed into `/usr/local/lib` and `/usr/local/include` respectively.
