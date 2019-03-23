# cocos2d-x blend examples
A project for learning how blend modes works in cocos2d-x

[![License: Apache2](https://img.shields.io/badge/license-Apache%202-blue.svg)](/LICENSE)

## info

This project has been created for teaching some friends about how blend modes works in cocos2d-x

# requirements

- C++ compiler for your platform (clang, gcc, etc.)
- C++ IDE (XCode, Visual Studio Express, etc.)
- python

## building

Init submodules

```bash
$ git submodule update --init
```

Update cocos2d-x files

```bash
$ cd cocos2d
$ git submodule update --init
$ python download-deps.py
```

Now you could open the project folder for the platform you like to use:

- proj.ios_mac
- proj.win32


## running the example

Use the following keys to test the example

- Cursors : Move the ball
- Shift : Increase opactiy
- Ctrl : Decrase opacity
- Space : Change blend mode
- 1 to 3 : Change the ball image
- 0 : Change the ball color
- Enter : Create a fireworks emitter
- Delete : Create a fire emitter
