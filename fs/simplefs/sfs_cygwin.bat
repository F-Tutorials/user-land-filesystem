gcc -o sfs-cygwin -D_FILE_OFFSET_BITS=64 -I./include ./src/sfs.c ./src/sfs_utils.c ./src/ddrivercyg.c -g cygdokanfuse2.dll