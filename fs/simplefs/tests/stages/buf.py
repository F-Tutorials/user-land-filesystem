import os
import sys
import time

def process(loop, cblks):
    abs_path = sys.path[0]
    mnt_point = os.path.join(abs_path, '../mnt')
    bin_path = os.path.join(abs_path, '../../build/sfs-fuse')
    f_path = os.path.join(mnt_point, 'test')

    work_set_sz = 16 * 512                                # 8KB
    iter_sz = 512                                         # 512B
    num_iters = work_set_sz // iter_sz

    os.system("echo 3 > /proc/sys/vm/drop_caches")
    os.system("rm -rf {} && mkdir {}".format(mnt_point, mnt_point))
    os.system("ddriver -r")
    os.system("{} --device=/root/ddriver --cache-blks={} {}".format(bin_path, cblks, mnt_point))
    
    tot_sz = loop * work_set_sz     # B
    start = time.time()
    with open(f_path, 'w+') as f:
        content = 'a' * work_set_sz
        for i in range(loop):
            f.seek(0)
            for j in range(num_iters):
                # print("Iter: " + str(j))
                f.write(content[j * iter_sz : (j + 1) * iter_sz])

        f.seek(0)
        data = f.read(work_set_sz)
        if data != content:
            print('buf: read data is not equal to written data')
            sys.exit(1)

    os.system('umount {}'.format(mnt_point))
    end = time.time()
    print('Time: {}ms BW: {}MB/s'.format(1000 * (end - start), tot_sz / 1024 / 1024 / (end - start)))

""" 
loop = 2
process(loop)
512 BLKs Cache
=================================
m:sfs_cache_hit_or_alloc_blk: 0
m:sfs_cache_hit_or_alloc_blk: 512
m:sfs_cache_hit_or_alloc_blk: 1024
h:sfs_cache_hit_or_alloc_blk: 1024
h:sfs_cache_hit_or_alloc_blk: 1024
m:sfs_cache_hit_or_alloc_blk: 10240
m:sfs_cache_hit_or_alloc_blk: 10752
m:sfs_cache_hit_or_alloc_blk: 11264
m:sfs_cache_hit_or_alloc_blk: 11776
m:sfs_cache_hit_or_alloc_blk: 12288
m:sfs_cache_hit_or_alloc_blk: 12800
m:sfs_cache_hit_or_alloc_blk: 13312
m:sfs_cache_hit_or_alloc_blk: 13824
h:sfs_cache_hit_or_alloc_blk: 10240
h:sfs_cache_hit_or_alloc_blk: 10752
h:sfs_cache_hit_or_alloc_blk: 11264
h:sfs_cache_hit_or_alloc_blk: 11776
h:sfs_cache_hit_or_alloc_blk: 12288
h:sfs_cache_hit_or_alloc_blk: 12800
h:sfs_cache_hit_or_alloc_blk: 13312
h:sfs_cache_hit_or_alloc_blk: 13824
m:sfs_cache_hit_or_alloc_blk: 14336
m:sfs_cache_hit_or_alloc_blk: 14848
m:sfs_cache_hit_or_alloc_blk: 15360
m:sfs_cache_hit_or_alloc_blk: 15872
m:sfs_cache_hit_or_alloc_blk: 16384
m:sfs_cache_hit_or_alloc_blk: 16896
m:sfs_cache_hit_or_alloc_blk: 17408
m:sfs_cache_hit_or_alloc_blk: 17920
h:sfs_cache_hit_or_alloc_blk: 14336
h:sfs_cache_hit_or_alloc_blk: 14848
h:sfs_cache_hit_or_alloc_blk: 15360
h:sfs_cache_hit_or_alloc_blk: 15872
h:sfs_cache_hit_or_alloc_blk: 16384
h:sfs_cache_hit_or_alloc_blk: 16896
h:sfs_cache_hit_or_alloc_blk: 17408
h:sfs_cache_hit_or_alloc_blk: 17920
h:sfs_cache_hit_or_alloc_blk: 10240
h:sfs_cache_hit_or_alloc_blk: 10752
h:sfs_cache_hit_or_alloc_blk: 11264
h:sfs_cache_hit_or_alloc_blk: 11776
h:sfs_cache_hit_or_alloc_blk: 12288
h:sfs_cache_hit_or_alloc_blk: 12800
h:sfs_cache_hit_or_alloc_blk: 13312
h:sfs_cache_hit_or_alloc_blk: 13824
h:sfs_cache_hit_or_alloc_blk: 10240
h:sfs_cache_hit_or_alloc_blk: 10752
h:sfs_cache_hit_or_alloc_blk: 11264
h:sfs_cache_hit_or_alloc_blk: 11776
h:sfs_cache_hit_or_alloc_blk: 12288
h:sfs_cache_hit_or_alloc_blk: 12800
h:sfs_cache_hit_or_alloc_blk: 13312
h:sfs_cache_hit_or_alloc_blk: 13824
h:sfs_cache_hit_or_alloc_blk: 14336
h:sfs_cache_hit_or_alloc_blk: 14848
h:sfs_cache_hit_or_alloc_blk: 15360
h:sfs_cache_hit_or_alloc_blk: 15872
h:sfs_cache_hit_or_alloc_blk: 16384
h:sfs_cache_hit_or_alloc_blk: 16896
h:sfs_cache_hit_or_alloc_blk: 17408
h:sfs_cache_hit_or_alloc_blk: 17920
h:sfs_cache_hit_or_alloc_blk: 14336
h:sfs_cache_hit_or_alloc_blk: 14848
h:sfs_cache_hit_or_alloc_blk: 15360
h:sfs_cache_hit_or_alloc_blk: 15872
h:sfs_cache_hit_or_alloc_blk: 16384
h:sfs_cache_hit_or_alloc_blk: 16896
h:sfs_cache_hit_or_alloc_blk: 17408
h:sfs_cache_hit_or_alloc_blk: 17920
h:sfs_cache_hit_or_alloc_blk: 1024
h:sfs_cache_hit_or_alloc_blk: 1024
m:sfs_cache_hit_or_alloc_blk: 1536
h:sfs_cache_hit_or_alloc_blk: 1536
m:sfs_cache_hit_or_alloc_blk: 9728
h:sfs_cache_hit_or_alloc_blk: 9728
h:sfs_cache_hit_or_alloc_blk: 0
h:sfs_cache_hit_or_alloc_blk: 0
h:sfs_cache_hit_or_alloc_blk: 512
h:sfs_cache_hit_or_alloc_blk: 512

Cache hit: 58
Cache miss: 21
Cache wb: 8         
Cache ld: 21

=================================
"""

loop = 1000000
print("Cache Blks: 512")
process(loop, 512) # => 101.79038122398883MB/s
print("Cache Blks: 0")
process(loop, 0)   # => 84.38797875936444MB/s
