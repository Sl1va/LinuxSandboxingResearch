#!/bin/bash
sudo unshare --mount --net -fp -R rootfs  /bin/bash -c '


mount -t proc proc /proc
mount -t devtmpfs none /dev

adduser myuser --disabled-password

# Switch to the new user and run the script
su -c ./app_static myuser '