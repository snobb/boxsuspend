Simple program to suspend a linux box

The following options can be set before building:
```
HASLOCK - spawn lock command (see config.h)
HASXRANDR - run xrandr init after resume (see config.h)
```

The options can be either set in `config.h` or by running the following command:
```bash
make 'CFLAGS+=-DHASLOCK -DHASXRANDR'
```
