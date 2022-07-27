#include "../include/sfs.h"
#define GET_CNODE(cache, blk)           (&cache->buf[(sizeof(struct sfs_cnode_hdr) + SFS_IO_SZ()) * (blk)])    
#define GET_BMID_BY_BLK(blk)            ((blk) / (SFS_CACHE_BMND_N * UINT8_BITS))
#define GET_BMBIT_BY_BLK(blk)           ((blk) % (SFS_CACHE_BMND_N * UINT8_BITS))   
#define GET_BMLINE_BY_BLK(blk)          (GET_BMBIT_BY_BLK(blk) / UINT8_BITS)

#define IS_DIRTY(flags)                 ((flags) & SFS_FLAG_BUF_DIRTY)
#define IS_OCCUPY(flags)                ((flags) & SFS_FLAG_BUF_OCCUPY)
#define IS_CACHE_ENABLE(cache)          ((cache)->num_blk != 0)

void __sfs_try_set_bm_indicators(struct sfs_bm_node *bm_node, int bm_line) {
    int i = 0;
    int bm_line_bit = bm_line * UINT8_BITS;

    if (!(bm_node->bm[bm_line_bit] & 0xFF)) {         /* 0x00 is 0 */
        bm_clear(bm_node->bm_indicators, bm_line);
    }
    else {
        bm_set(bm_node->bm_indicators, bm_line);
    }
}

void __sfs_cache_blk_wb(struct sfs_cache *cache, int blk) {
    struct sfs_cnode *cnode;
    unsigned char *data;

    cnode = GET_CNODE(cache, blk);

    if (IS_DIRTY(cnode->hdr.flags)) {
        data = cnode->data;
        ddriver_seek(SFS_DRIVER(), cnode->hdr.phys_ofs, SEEK_SET);
        ddriver_write(SFS_DRIVER(), data, cache->sz_blk);    
    }
    cache->cache_wb += 1;
    cnode->hdr.flags = 0x00;                              /* 清除标志位 */
}

void __sfs_cache_blk_ld(struct sfs_cache *cache, int blk, int phys_offs) {
    struct sfs_cnode *cnode;
    unsigned char *data;
    
    cnode = GET_CNODE(cache, blk);
    data = cnode->data;
    
    ddriver_seek(SFS_DRIVER(), phys_offs, SEEK_SET);
    ddriver_read(SFS_DRIVER(), data, cache->sz_blk);
    
    
    cache->cache_ld += 1;
    cnode->hdr.flags = 0x00;                              /* 清除标志位 */
}

void sfs_cache_rls_blk(struct sfs_cache *cache, int blk) {
    int bm_id = 0;
    
    struct sfs_bm_node *bm_node;

    __sfs_cache_blk_wb(cache, blk);
    
    bm_id = GET_BMID_BY_BLK(blk);
    bm_node = &cache->bms[bm_id];
    bm_clear(bm_node->bm, GET_BMBIT_BY_BLK(blk));

    __sfs_try_set_bm_indicators(bm_node, GET_BMLINE_BY_BLK(blk));

    bm_node->avai++;
}

void sfs_cache_alloc_blk(struct sfs_cache *cache, int blk, int phys_offs) {
    int bm_id = 0;
    
    struct sfs_bm_node *bm_node;

    __sfs_cache_blk_ld(cache, blk, phys_offs);
    
    bm_id = GET_BMID_BY_BLK(blk);
    bm_node = &cache->bms[bm_id];
    bm_set(bm_node->bm, GET_BMBIT_BY_BLK(blk));

    __sfs_try_set_bm_indicators(bm_node, GET_BMLINE_BY_BLK(blk));

    bm_node->avai--;
}

int sfs_cache_hit_or_alloc_blk(struct sfs_cache *cache, int phys_ofs) {
    int blk = -1;
    int bm_id, line, i;
    int num_bms = cache->num_bms;
    int bm_lines = SFS_CACHE_BMND_N;
    int free_blk = -1;
    int evict_blk = -1;
    struct sfs_bm_node *bm_node;
    struct sfs_cnode   *cnode;
    
    for (bm_id = 0; bm_id < num_bms; bm_id++) {        /* 遍历所有为位图，查看Cache命中和空闲块 */   
        bm_node = &cache->bms[bm_id];
        if (bm_node->avai != SFS_CACHE_BMND_N * UINT8_BITS) {
            for (line = 0; line < bm_lines; line++) { 
                if (bm_test(bm_node->bm_indicators, line)) {
                    blk = bm_id * SFS_CACHE_BMND_N * UINT8_BITS + line * UINT8_BITS;
                    for (i = 0; i < UINT8_BITS; i++) {
                        if (bm_test(bm_node->bm, blk + i)) {
                            cnode = GET_CNODE(cache, blk + i);
                            if (cnode->hdr.phys_ofs == phys_ofs) {
                                cache->cache_hit += 1;
                                return blk + i;
                            }
                        }
                        else {
                            if (free_blk == -1) {
                                free_blk = blk + i;
                            }        
                        }
                    }
                }
                else {
                    if (free_blk == -1) {
                        blk = bm_id * SFS_CACHE_BMND_N * UINT8_BITS + line * UINT8_BITS;
                        free_blk = blk;
                    }
                }
            }
        }
        else {
            blk = bm_id * SFS_CACHE_BMND_N * UINT8_BITS;
            free_blk = blk;
        }
    }
    
    cache->cache_miss += 1;

    if (free_blk == -1 || free_blk > cache->num_blk - 1) { 
                                                      /* 驱逐缓存行 */
        evict_blk = (cache->last_evicted + 1) % cache->num_blk;
        sfs_cache_rls_blk(cache, evict_blk);
        cache->last_evicted = evict_blk;
        free_blk = evict_blk;
    }

    blk = free_blk;

    sfs_cache_alloc_blk(cache, blk, phys_ofs);

    return blk;
}


struct sfs_cache* sfs_cache_init(int io_sz, int blks) {
    struct sfs_cache   *cache;
    struct sfs_bm_node *bm_node;
    int num_bms = 1;
    int i;

    cache = (struct sfs_cache *)malloc(sizeof(struct sfs_cache));
    cache->num_blk = blks;
    cache->sz_blk = io_sz;
    cache->buf = (unsigned char *)malloc((sizeof(struct sfs_cnode_hdr) + SFS_IO_SZ()) * blks);
    blks = SFS_ROUND_UP(blks, (SFS_CACHE_BMND_N * UINT8_BITS));
    cache->num_bms = num_bms = blks / (SFS_CACHE_BMND_N * UINT8_BITS);
    cache->bms = (struct sfs_bm_node *)malloc(sizeof(struct sfs_bm_node) * num_bms);
    cache->last_evicted = -1;
    cache->cache_hit = 0;
    cache->cache_miss = 0;
    cache->cache_wb = 0;
    cache->cache_ld = 0;

    for(i = 0; i < num_bms; i++) {                    /* 清除位图 */
        bm_node = &cache->bms[i];
        bm_node->avai = SFS_CACHE_BMND_N * UINT8_BITS;
        memset(bm_node->bm_indicators, 0, SFS_CACHE_BMID_N);
        memset(bm_node->bm, 0, SFS_CACHE_BMND_N);
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

void sfs_cache_flush(struct sfs_cache *cache) {
    int blk = -1;
    int bm_id, line, i;
    int num_bms = cache->num_bms;
    int bm_lines = SFS_CACHE_BMND_N;
    struct sfs_bm_node *bm_node;
    struct sfs_cnode   *cnode;
    
    for (bm_id = 0; bm_id < num_bms; bm_id++) {        /* 遍历所有为位图，寻找分配后的块 */   
        bm_node = &cache->bms[bm_id];
        if (bm_node->avai != SFS_CACHE_BMND_N * UINT8_BITS) {
            for (line = 0; line < bm_lines; line++) { 
                if (bm_test(bm_node->bm_indicators, line)) {
                    blk = bm_id * SFS_CACHE_BMND_N * UINT8_BITS + line * UINT8_BITS;
                    for (i = 0; i < UINT8_BITS; i++) {
                        if (bm_test(bm_node->bm, blk + i)) {
                            sfs_cache_rls_blk(cache, blk + i);
                        }
                    }
                }
            }
        }
    }
}

int sfs_try_cache_read(struct sfs_cache *cache, int offset, uint8_t *out_content, int size) {
    int blk;
    struct sfs_cnode *cnode;
    
    if (IS_CACHE_ENABLE(cache)) {
        blk = sfs_cache_hit_or_alloc_blk(cache, offset);
        cnode = GET_CNODE(cache, blk);
        
        memcpy(out_content, cnode->data, size);
        
        cnode->hdr.phys_ofs = offset;
        cnode->hdr.flags |= SFS_FLAG_BUF_OCCUPY;
    }
    else {
        ddriver_seek(SFS_DRIVER(), offset, SEEK_SET);
        ddriver_read(SFS_DRIVER(), out_content, size);  
    }
    return size;
}

int sfs_try_cache_write(struct sfs_cache *cache, int offset, uint8_t *in_content, int size) {
    int blk;
    struct sfs_cnode *cnode;

    if (IS_CACHE_ENABLE(cache)) {
        blk = sfs_cache_hit_or_alloc_blk(cache, offset);
        cnode = GET_CNODE(cache, blk);

        memcpy(cnode->data, in_content, size);
    
        cnode->hdr.phys_ofs = offset;
        cnode->hdr.flags |= SFS_FLAG_BUF_DIRTY;
    }
    else {
        ddriver_seek(SFS_DRIVER(), offset, SEEK_SET);
        ddriver_write(SFS_DRIVER(), in_content, size);
    }
    
    return size;
}

int sfs_cache_report(struct sfs_cache *cache) {
    printf("Cache hit: %d\nCache miss: %d\nCache wb: %d\nCache ld: %d\n", 
    cache->cache_hit, cache->cache_miss, cache->cache_wb, cache->cache_ld);
    return 0;
}