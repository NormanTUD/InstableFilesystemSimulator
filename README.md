# What is this?

This allows you to simulate a very unstable file system, where everything fails, delays are huge and so n.

## Compile it

```bash
gcc -shared -o unstable_fs.so -fPIC unstable_fs.c -ldl
```

## Use it

```bash
LD_PRELOAD=./unstable_fs.so python3
```
