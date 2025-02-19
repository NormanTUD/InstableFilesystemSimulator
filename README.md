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
