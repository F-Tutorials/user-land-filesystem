#ifndef _FS_H_
#define _FS_H_

#define FUSE_USE_VERSION 26
#include "stdio.h"
#include "stdlib.h"
#include <unistd.h>
#include "fcntl.h"
#include "string.h"
#include "fuse.h"
#include <stddef.h>
#include "ddriver.h"
#include "errno.h"
#include "types.h"

#define FS_MAGIC                  /* TODO: Define by yourself */
#define FS_DEFAULT_PERM    0777   /* 全权限打开 */

/******************************************************************************
* SECTION: fs.c
*******************************************************************************/
void* 			   fs_init(struct fuse_conn_info *);
void  			   fs_destroy(void *);
int   			   fs_mkdir(const char *, mode_t);
int   			   fs_getattr(const char *, struct stat *);
int   			   fs_readdir(const char *, void *, fuse_fill_dir_t, off_t,
						                struct fuse_file_info *);
int   			   fs_mknod(const char *, mode_t, dev_t);
int   			   fs_write(const char *, const char *, size_t, off_t,
					                  struct fuse_file_info *);
int   			   fs_read(const char *, char *, size_t, off_t,
					                 struct fuse_file_info *);
int   			   fs_access(const char *, int);
int   			   fs_unlink(const char *);
int   			   fs_rmdir(const char *);
int   			   fs_rename(const char *, const char *);
int   			   fs_utimens(const char *, const struct timespec tv[2]);
int   			   fs_truncate(const char *, off_t);
			
int   			   fs_open(const char *, struct fuse_file_info *);
int   			   fs_opendir(const char *, struct fuse_file_info *);

#endif  /* _fs_H_ */