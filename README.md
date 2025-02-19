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

## Example output

```
LD_PRELOAD=./unstable_fs.so python3
object address  : 0x7f51ee579bc0
object refcount : 1
object type     : 0x9ea4e0
object type name: OSError
object repr     : OSError(9, 'Bad file descriptor')
lost sys.stderr
```

```
LD_PRELOAD=./unstable_fs.so python3
Python 3.13.2 (main, Feb  5 2025, 01:23:35) [GCC 14.2.0] on linux
Type "help", "copyright", "credits" or "license" for more information.
Failed calling sys.__interactivehook__
object address  : 0x7fe094615a40
object refcount : 3
object type     : 0x9ea4e0
object type name: OSError
object repr     : OSError(9, 'Bad file descriptor')
lost sys.stderr
object address  : 0x7fe094615e40
object refcount : 3
object type     : 0x9ea4e0
object type name: OSError
object repr     : OSError(9, 'Bad file descriptor')
lost sys.stderr
```

```
Error processing line 1 of /usr/lib/python3/dist-packages/distutils-precedence.pth:
Traceback (most recent call last):
object address  : 0x7f41d977f1c0
object refcount : 1
object type     : 0x9ea4e0
object type name: OSError
object repr     : OSError(5, 'Input/output error')
lost sys.stderr
```
