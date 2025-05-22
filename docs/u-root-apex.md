# u-root + APEX Integration Guide

This document describes a scriptable approach for replacing Harvey's legacy `/bin` tree with a modern [u-root](https://github.com/u-root/u-root) busybox while keeping POSIX tools available via the [APEX](https://github.com/Harvey-OS/apex) project.

## 1. Build a Go toolchain for Plan 9

```
export GOROOT_BOOTSTRAP=$(go env GOROOT)
mkdir -p $HOME/go-plan9
cd $HOME/go-plan9/src
GOOS=plan9 GOARCH=amd64 ./bootstrap.bash
```

Untar the resulting `go-plan9-*-bootstrap.tbz` inside the Harvey image and bind `/usr/glenda/go/bin` over `/bin`.

## 2. Build the u-root busybox

Install u-root and gobusybox on the host:

```
go install github.com/u-root/u-root@latest
go install github.com/u-root/gobusybox@latest
```

Create a Plan 9 initramfs:

```
GOOS=plan9 GOARCH=amd64 u-root \
    -build=gbb \
    -defaultsh=/bbin/elvish \
    -o /tmp/uroot.cpio
```

Copy `uroot.cpio` into Harvey, mount it and shadow `/bin`:

```
fs/cpiofs /tmp/uroot.cpio
bind -a /n/tapefs /
bind -a /bbin /bin
```

Verify that `/bin/ls` is now provided by u-root.

## 3. Update login shell

Edit `/rc/bin/termrc` to start `/bbin/elvish` (or `/bbin/sh`). Keep the original `rc` binary in case a rollback is needed.

## 4. Add POSIX compatibility with APEX

```
 git clone https://github.com/Harvey-OS/apex
 cd apex/src
 CC=gcc HARVEY=$HOME/harvey APEX=$(dirname `pwd`) OS=linux ARCH=amd64 make
```

Copy `libap.so` and the `/cmd` binaries into a new directory `/posix`. Extend the namespace in `/boot/bootrc`:

```
bind -a /posix/bin /bin
bind -a /posix/lib /lib
```

Test with an APEX tool, for example:

```
echo 'BEGIN{print "hello"}' | awk
```

## 5. Why do this?

* **Language unification** – Both Harvey and u-root are Go-first projects.
* **Namespace friendly** – Plan 9 bindings let you overlay `/bbin` and `/posix` per process without touching the kernel.
* **Reproducible builds** – Go modules and musl simplify cross compilation.
* **Modern tooling** – Use Go's race detector, vet and fuzz support.

## 6. Caveats

* u-root binaries are larger than the original C utilities.
* APEX lacks full POSIX job control support.

This guide provides a starting point for modernising the Harvey CLI environment while retaining compatibility with traditional UNIX tools.
