# GLSL Minifier

**GLSL Minifier** is a simple program to remove comments and **minimize** shader code (remove whitespace and summarize code).

## Building

Building *shader-minify* can be done using GNU Make:

```
make
```

## Install

To install *shader-minify*, run the following target:

```
make install PREFIX=(prefix)
```

The variable `PREFIX` defaults to `/usr/local`.

## Uninstall

To uninstall *shader-minify*, run the following target using the same prefix as specified in the install process:

```
make uninstall PREFIX=(prefix)
```

## Acknowledgement

This project is based on the work of Max Base - [C-Minifier](https://github.com/BaseMax/C-Minifier).
