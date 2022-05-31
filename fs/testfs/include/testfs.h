#ifndef _TESTFS_H_
#define _TESTFS_H_

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

#define TESTFS_MAGIC                  /* TODO: Define by yourself */
#define TESTFS_DEFAULT_PERM    0777   /* 全权限打开 */

/******************************************************************************
* SECTION: testfs.c
*******************************************************************************/
void* 			   testfs_init(struct fuse_conn_info *);
void  			   testfs_destroy(void *);
int   			   testfs_mkdir(const char *, mode_t);
int   			   testfs_getattr(const char *, struct stat *);
int   			   testfs_readdir(const char *, void *, fuse_fill_dir_t, off_t,
						                struct fuse_file_info *);
int   			   testfs_mknod(const char *, mode_t, dev_t);
int   			   testfs_write(const char *, const char *, size_t, off_t,
					                  struct fuse_file_info *);
int   			   testfs_read(const char *, char *, size_t, off_t,
					                 struct fuse_file_info *);
int   			   testfs_access(const char *, int);
int   			   testfs_unlink(const char *);
int   			   testfs_rmdir(const char *);
int   			   testfs_rename(const char *, const char *);
int   			   testfs_utimens(const char *, const struct timespec tv[2]);
int   			   testfs_truncate(const char *, off_t);
			
int   			   testfs_open(const char *, struct fuse_file_info *);
int   			   testfs_opendir(const char *, struct fuse_file_info *);

#endif  /* _testfs_H_ */