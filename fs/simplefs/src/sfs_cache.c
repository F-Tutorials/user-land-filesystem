#include "../include/sfs.h"

struct sfs_cache* sfs_cache_init(int io_sz, int blks) {
    struct sfs_cache   *cache;
    struct sfs_bm_node *bmn;
    int num_bms = 1;
    int i;

    cache = (struct sfs_cache *)malloc(sizeof(struct sfs_cache));
    cache->num_blk = blks;
    cache->sz_blk = io_sz;
    cache->buf = (unsigned char *)malloc(sizeof(struct sfs_cnode) * blks);
    blks = SFS_ROUND_UP(blks, (SFS_CACHE_BMND_N * UINT8_BITS));
    num_bms = blks / (SFS_CACHE_BMND_N * UINT8_BITS);
    cache->bms = (struct sfs_bm_node *)malloc(sizeof(struct sfs_bm_node) * num_bms);

    for(i = 0; i < num_bms; i++) {                    /* 清除位图 */
        bmn = &cache->bms[i];
        bmn->avai = SFS_CACHE_BMND_N * UINT8_BITS;
        memset(bmn->bm_indicators, 0, SFS_CACHE_BMID_N);
        memset(bmn->bm, 0, SFS_CACHE_BMND_N);
    }

    return cache;
}

void sfs_cache_destroy(struct sfs_cache *cache) {
    if (cache) {
        free(cache->bms);
        free(cache->buf);
        free(cache);
    }
}
