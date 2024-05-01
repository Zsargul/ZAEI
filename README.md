![ZAEI Logo](img/logo.png)

**<h2 align="center">Zsargul's Artix Environment Initializer</h2>**

ZAEI (pronounced "zey"), is a tool inspired by [Luke Smith's LARBS](https://github.com/LukeSmithxyz/LARBS), for automatically downloading and/or configuring my packages, graphical display, and other configurations that I use on my Artix install. While inspired by Luke Smith's script, I felt like writing my own utility and fine-tuning it to my own needs and specific setup. I did however choose to write this one in C instead of bash. While bash would likely be more efficient and more lightweight, I like C and didn't want to just copy Luke Smith's scripts. This probably doesn't matter as I'll likely be the only one using this. If you're looking for something to use yourself, I recommend using Luke Smith's script cause they're probably better and more easily customisable for general use cases.

### Building from source

**Note:** The libconfig version that was statically linked into the release binary was `1.7.3`. If you're worried
about newer versions having problems, you can follow the instructions under ["Building libconfig"](#building-libconfig)

#### Building libconfig

If you want to compile this from source, you will also need to build `libconfig`, a statically linked dependency of ZAEI. The complete build instructions can be found [here](https://github.com/hyperrealm/libconfig/blob/master/INSTALL) in their git repository, but I will include the specific steps I took below for the sake of brevity:

```
mkdir tmp && cd tmp
wget https://hyperrealm.github.io/libconfig/dist/libconfig-1.7.3.tar.gz
tar -xvzf libconfig-1.7.3.tar.gz
cd libconfig-1.7.3
./configure
make
make install

```
