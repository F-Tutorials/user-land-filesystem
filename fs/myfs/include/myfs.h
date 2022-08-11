#ifndef _MYFS_H_
#define _MYFS_H_

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

#define MYFS_MAGIC                  /* TODO: Define by yourself */
#define MYFS_DEFAULT_PERM    0777   /* 全权限打开 */

/******************************************************************************
* SECTION: myfs.c
*******************************************************************************/
void* 			   myfs_init(struct fuse_conn_info *);
void  			   myfs_destroy(void *);
int   			   myfs_mkdir(const char *, mode_t);
int   			   myfs_getattr(const char *, struct stat *);
int   			   myfs_readdir(const char *, void *, fuse_fill_dir_t, off_t,
						                struct fuse_file_info *);
int   			   myfs_mknod(const char *, mode_t, dev_t);
int   			   myfs_write(const char *, const char *, size_t, off_t,
					                  struct fuse_file_info *);
int   			   myfs_read(const char *, char *, size_t, off_t,
					                 struct fuse_file_info *);
int   			   myfs_access(const char *, int);
int   			   myfs_unlink(const char *);
int   			   myfs_rmdir(const char *);
int   			   myfs_rename(const char *, const char *);
int   			   myfs_utimens(const char *, const struct timespec tv[2]);
int   			   myfs_truncate(const char *, off_t);
			
int   			   myfs_open(const char *, struct fuse_file_info *);
int   			   myfs_opendir(const char *, struct fuse_file_info *);

#endif  /* _myfs_H_ */