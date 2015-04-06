#include "cncocr_internal.h"



// XXX - depending on misc.h for HAL on FSim

#if CNCOCR_TG
#define CNC_ITEMS_PER_BLOCK 8
#else
#define CNC_ITEMS_PER_BLOCK 64
#endif

#define CNC_ITEM_BLOCK_FULL(block) ((block)->count == CNC_ITEMS_PER_BLOCK)
#define CNC_GETTER_GUID ((ocrGuid_t)-1)
#define CNC_GETTER_ROLE 'G'
#define CNC_PUTTER_ROLE 'P'

typedef struct {
    ocrGuid_t entry;
    ocrGuid_t bucketHead;
    ocrGuid_t firstBlock;
    ocrGuid_t oldFirstBlock;
    ocrGuid_t affinity;
    u32 firstBlockCount;
    u32 tagLength;
    u32 slot;
    ocrDbAccessMode_t mode;
    u8 checkedFirst;
    u64 role; // forcing tag[] to be 8-byte aligned (for FSim)
    u8 tag[];
} ItemCollOpParams;

typedef struct {
    bool isEvent;
    ocrGuid_t guid;
} ItemBlockEntry;

typedef struct {
    u32 count;
    ocrGuid_t next;
    ItemBlockEntry entries[CNC_ITEMS_PER_BLOCK];
    u8 tags[];
} ItemBlock;

static ocrGuid_t _itemBlockCreate(u32 tagLength, ocrGuid_t next, ItemBlock **out) {
    ocrGuid_t blockGuid;
    ItemBlock *block;
    u64 size = sizeof(ItemBlock) + (tagLength * CNC_ITEMS_PER_BLOCK);
    SIMPLE_DBCREATE(&blockGuid, (void**)&block, size);
    // XXX - should we start from the back?
    block->count = 0;
    block->next = next;
    *out = block;
    return blockGuid;
}

static ocrGuid_t _itemBlockInsert(ItemBlock *block, u8 *tag, ocrGuid_t entry, u32 tagLength) {
    ASSERT(!CNC_ITEM_BLOCK_FULL(block));
    u32 i = block->count;
    if (entry == CNC_GETTER_GUID) {
        block->entries[i].isEvent = true;
        ocrEventCreate(&block->entries[i].guid, OCR_EVENT_IDEM_T, true);
    }
    else {
        block->entries[i].isEvent = false;
        block->entries[i].guid = entry;
    }
    hal_memCopy(&block->tags[i*tagLength], tag, tagLength, 0);
    hal_fence();
    block->count += 1;
    return block->entries[i].guid;
}

static bool _cncMemCompare(const void *a, const void *b, size_t n) {
    const u8 *x = (const u8*)a;
    const u8 *y = (const u8*)b;
    size_t i;
    for (i=0; i<n; i++) {
        if (x[i] != y[i]) return 1;
    }
    return 0;
}

#define CNC_MEMCMP _cncMemCompare


/* Compare byte-by-byte for tag equality */
static bool _equals(u8 *tag1, u8 *tag2, u32 length) {
    return CNC_MEMCMP(tag1, tag2, length) == 0;
}

/* Hash function implementation. Fast and pretty good */
static u64 _hash_function(u8 *str, u32 length) {
    u64 hash = 5381;
    u64 c;
    // All tags should be composed of longs, but even if
    // they're not, part of it is just ignored for the hash.
    u64 *components = (u64*)str;
    u32 n = length / sizeof(u64);
    while (n-- > 0) {
        c = *components++;
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

static u32 _itemBlockFind(ItemBlock *block, u8 *tag, u32 tagLength, u32 startAt) {
    u32 i = startAt;
    for (; i<block->count; i++) {
        if (_equals(&block->tags[i*tagLength], tag, tagLength)) {
            return i;
        }
    }
    return CNC_ITEMS_PER_BLOCK; // not found
}

static ocrGuid_t _searchBucketEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]);

static ocrGuid_t _addToBucketEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
    // unpack
    ocrGuid_t *blockArray = depv[0].ptr;
    ItemCollOpParams *params = depv[1].ptr;
    ocrGuid_t paramsGuid = depv[1].guid;
    const u32 index = 0;
    // look up the first block the bucket
    ocrGuid_t firstBlock = blockArray[index];
    // is our first block still first?
    if (firstBlock == params->firstBlock) {
        ItemBlock *newFirst;
        blockArray[index] = _itemBlockCreate(params->tagLength, firstBlock, &newFirst);
        // XXX - repeated code, also in addToBlock
        bool isGetter = (params->role == CNC_GETTER_ROLE);
        ocrGuid_t src = isGetter ? CNC_GETTER_GUID : params->entry;
        ocrGuid_t res = _itemBlockInsert(newFirst, params->tag, src, params->tagLength);
        if (isGetter) {
            ocrAddDependence(res, params->entry, params->slot, params->mode);
        }
        // DONE! clean up.
        ocrDbDestroy(paramsGuid);
    }
    else { // someone added a new block...
        // try searching again
        params->oldFirstBlock = params->firstBlock;
        params->firstBlock = firstBlock;
        params->checkedFirst = 0;
        ocrGuid_t deps[] = { firstBlock, paramsGuid };
        ocrGuid_t searchEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _searchBucketEdt, 0, 2);
        ocrEdtCreate(&searchEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/deps,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrEdtTemplateDestroy(templGuid);
    }
    return NULL_GUID;
}

static ocrGuid_t _addToBlockEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
    // unpack
    ItemBlock *block = depv[0].ptr;
    ItemCollOpParams *params = depv[1].ptr;
    ocrGuid_t paramsGuid = depv[1].guid;
    // is it in this block?
    // XXX - repeated code (also in the searchEdt)
    u32 i = _itemBlockFind(block, params->tag, params->tagLength, 0);
    if (i < CNC_ITEMS_PER_BLOCK) { // found!
        ocrGuid_t foundEntry = block->entries[i].guid;
        if (params->role == CNC_GETTER_ROLE) { // Get
            ocrAddDependence(foundEntry, params->entry, params->slot, params->mode);
        }
        else if (block->entries[i].isEvent) { // Put
            ocrAddDependence(params->entry, foundEntry, 0, DB_DEFAULT_MODE);
        }
        // XXX - currently ignoring single assignment checks
        // DONE! clean up.
        ocrDbDestroy(paramsGuid);
    }
    // add the entry if there's still room
    else if (!CNC_ITEM_BLOCK_FULL(block)) {
        bool isGetter = (params->role == CNC_GETTER_ROLE);
        ocrGuid_t src = isGetter ? CNC_GETTER_GUID : params->entry;
        ocrGuid_t res = _itemBlockInsert(block, params->tag, src, params->tagLength);
        if (isGetter) {
            ocrAddDependence(res, params->entry, params->slot, params->mode);
        }
        // DONE! clean up.
        ocrDbDestroy(paramsGuid);
    }
    else { // the block filled up while we were searching
        // might need to add a new block to the bucket
        ocrGuid_t addEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _addToBucketEdt, 0, 2);
        ocrEdtCreate(&addEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/NULL,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrAddDependence(params->bucketHead, addEdtGuid, 0, DB_MODE_EW);
        ocrAddDependence(paramsGuid, addEdtGuid, 1, DB_DEFAULT_MODE);
        ocrEdtTemplateDestroy(templGuid);
    }
    return NULL_GUID;
}

static ocrGuid_t _searchBucketEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
    // unpack
    ItemBlock *block = depv[0].ptr;
    ocrGuid_t blockGuid = depv[0].guid;
    ItemCollOpParams *params = depv[1].ptr;
    ocrGuid_t paramsGuid = depv[1].guid;
    // record first block size (if applicable)
    if (!params->checkedFirst) {
        params->checkedFirst = 1;
        params->firstBlockCount = block->count;
    }
    // is it in this block?
    u32 i = _itemBlockFind(block, params->tag, params->tagLength, 0);
    if (i < CNC_ITEMS_PER_BLOCK) { // found!
        ocrGuid_t foundEntry = block->entries[i].guid;
        if (params->role == CNC_GETTER_ROLE) { // Get
            ocrAddDependence(foundEntry, params->entry, params->slot, params->mode);
        }
        else if (block->entries[i].isEvent) { // Put
            ocrAddDependence(params->entry, foundEntry, 0, DB_DEFAULT_MODE);
        }
        // XXX - currently ignoring single assignment checks
        // DONE! clean up.
        ocrDbDestroy(paramsGuid);
    }
    // did we reach the end of the search?
    else if (block->next == NULL_GUID || blockGuid == params->oldFirstBlock) {
        // try to go back and add it to the first block
        // XXX - should check if it was full
        ocrGuid_t addEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _addToBlockEdt, 0, 2);
        ocrEdtCreate(&addEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/NULL,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrAddDependence(params->firstBlock, addEdtGuid, 0, DB_MODE_EW);
        ocrAddDependence(paramsGuid, addEdtGuid, 1, DB_DEFAULT_MODE);
        ocrEdtTemplateDestroy(templGuid);
    }
    else { // keep looking...
        // search next
        ocrGuid_t deps[] = { block->next, paramsGuid };
        ocrGuid_t searchEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _searchBucketEdt, 0, 2);
        ocrEdtCreate(&searchEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/deps,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrEdtTemplateDestroy(templGuid);
    }
    return NULL_GUID;
}

static ocrGuid_t _bucketHeadEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
    // unpack
    ocrGuid_t *blockArray = depv[0].ptr;
    ItemCollOpParams *params = depv[1].ptr;
    ocrGuid_t paramsGuid = depv[1].guid;
    // save bucket info for first block
    params->firstBlock = blockArray[0];
    if (params->firstBlock == NULL_GUID) { // empty bucket
        // might need to add a new block to the bucket
        ocrGuid_t addEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _addToBucketEdt, 0, 2);
        ocrEdtCreate(&addEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/NULL,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrAddDependence(params->bucketHead, addEdtGuid, 0, DB_MODE_EW);
        ocrAddDependence(paramsGuid, addEdtGuid, 1, DB_DEFAULT_MODE);
        ocrEdtTemplateDestroy(templGuid);
    }
    else { // search the bucket
        ocrGuid_t deps[] = { params->firstBlock, paramsGuid };
        ocrGuid_t searchEdtGuid, templGuid;
        ocrEdtTemplateCreate(&templGuid, _searchBucketEdt, 0, 2);
        ocrEdtCreate(&searchEdtGuid, templGuid,
            /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
            /*depc=*/EDT_PARAM_DEF, /*depv=*/deps,
            /*properties=*/EDT_PROP_NONE,
            /*affinity=*/params->affinity, /*outEvent=*/NULL);
        ocrEdtTemplateDestroy(templGuid);
    }
    return NULL_GUID;
}

static ocrGuid_t _doHashEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
    // unpack
    ocrGuid_t *blockArray = depv[0].ptr;
    ItemCollOpParams *params = depv[1].ptr;
    ocrGuid_t paramsGuid = depv[1].guid;
    // find the the bucket index
    u64 hash = _hash_function(params->tag, params->tagLength);
    u32 index = hash % CNC_TABLE_SIZE;
    // save bucket info
    params->bucketHead = blockArray[index];
    params->oldFirstBlock = NULL_GUID;
    params->checkedFirst = 0;
    // go into bucket
    ocrGuid_t deps[] = { params->bucketHead, paramsGuid };
    ocrGuid_t bucketEdtGuid, templGuid;
    ocrEdtTemplateCreate(&templGuid, _bucketHeadEdt, 0, 2);
    ocrEdtCreate(&bucketEdtGuid, templGuid,
        /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
        /*depc=*/EDT_PARAM_DEF, /*depv=*/deps,
        /*properties=*/EDT_PROP_NONE,
        /*affinity=*/params->affinity, /*outEvent=*/NULL);
    ocrEdtTemplateDestroy(templGuid);
    return NULL_GUID;
}

static void _itemCollUpdate(ocrGuid_t coll, u8 *tag, u32 tagLength, u8 role, ocrGuid_t entry, u32 slot, ocrDbAccessMode_t mode) {
    // Build datablock to hold search parameters
    ocrGuid_t paramsGuid;
    ItemCollOpParams *params;
    SIMPLE_DBCREATE(&paramsGuid, (void**)&params, sizeof(ItemCollOpParams)+tagLength);
    params->entry = entry;
    params->tagLength = tagLength;
    params->role = role;
    params->slot = slot;
    params->mode = mode;
    hal_memCopy(params->tag, tag, tagLength, 0);
    // affinity
    // TODO - affinitizing each bucket with node, and running all the
    // lookup stuff on that node would probably be cheaper.
    params->affinity = NULL_GUID;
    // edt
    ocrGuid_t hashEdtGuid, templGuid;
    ocrEdtTemplateCreate(&templGuid, _doHashEdt, 0, 2);
    ocrEdtCreate(&hashEdtGuid, templGuid,
        /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
        /*depc=*/EDT_PARAM_DEF, /*depv=*/NULL,
        /*properties=*/EDT_PROP_NONE,
        /*affinity=*/params->affinity, /*outEvent=*/NULL);
    ocrAddDependence(coll, hashEdtGuid, 0, DB_MODE_RO);
    ocrAddDependence(paramsGuid, hashEdtGuid, 1, DB_DEFAULT_MODE);
    ocrEdtTemplateDestroy(templGuid);
}

/* Putting an item into the hashmap */
bool _cncPut(ocrGuid_t item, unsigned char *tag, int tagLength, cncItemCollection_t collection, bool isSingleAssignment) {
    _itemCollUpdate(collection, tag, tagLength, CNC_PUTTER_ROLE, item, 0, DB_DEFAULT_MODE);
    return true;
}

/* Get GUID from an item tag */
void _cncGet(unsigned char *tag, int tagLength, ocrGuid_t destination, u32 slot, ocrDbAccessMode_t mode, cncItemCollection_t collection) {
    _itemCollUpdate(collection, tag, tagLength, CNC_GETTER_ROLE, destination, slot, mode);
}

/* Put a singleton item */
bool _cncPutSingleton(ocrGuid_t item, ocrGuid_t collection, bool isSingleAssignment) {
    ocrEventSatisfy(collection, item);
    return true;
}

/* Get GUID for singleton item */
void _cncGetSingleton(ocrGuid_t destination, u32 slot, ocrDbAccessMode_t mode, ocrGuid_t collection) {
    ocrAddDependence(collection, destination, slot, mode);
}

static ocrGuid_t _shutdownEdt(u32 paramc, u64 paramv[], u32 depc, ocrEdtDep_t depv[]) {
ocrShutdown();
    return NULL_GUID;
}

void cncAutomaticShutdown(ocrGuid_t doneEvent) {
    ocrGuid_t shutdownEdtGuid, templGuid;
    ocrEdtTemplateCreate(&templGuid, _shutdownEdt, 0, 1);
    ocrEdtCreate(&shutdownEdtGuid, templGuid,
        /*paramc=*/EDT_PARAM_DEF, /*paramv=*/NULL,
        /*depc=*/EDT_PARAM_DEF, /*depv=*/&doneEvent,
        /*properties=*/EDT_PROP_NONE,
        /*affinity=*/NULL_GUID, /*outEvent=*/NULL);
    ocrEdtTemplateDestroy(templGuid);
}

void *_cncRangedInputAlloc(u32 n, u32 dims[], size_t itemSize, ocrEdtDep_t *out) {
    u32 i, j, k;
    ///////////////////////////////////////
    // Figure out how much memory we need
    ///////////////////////////////////////
    u32 sum = 0, product = 1;
    for (i=0; i<n-1; i++) {
        product *= dims[i];
        sum += sizeof(void*) * product;
    }
    product *= dims[i];
    sum += itemSize * product;
    ///////////////////////////////////////
    // Allocate a datablock
    ///////////////////////////////////////
    ocrEdtDep_t block;
    SIMPLE_DBCREATE(&block.guid, &block.ptr, sum);
    void *dataStart = block.ptr;
    ///////////////////////////////////////
    // Set up the internal pointers
    ///////////////////////////////////////
    if (n > 1) {
        u32 prevDim = 1, currDim = 1, nextDim = dims[0];
        // Set up the pointers-to-pointers
        void **ptrs = block.ptr;
        void **current = ptrs;
        void **tail = ptrs + nextDim; // make room for first array
        for (i=1; i<n-1; i++) {
            // Slide the window
            prevDim = currDim;
            currDim = nextDim;
            nextDim = dims[i];
            // One array for each parent
            for (j=0; j<prevDim; j++) {
                // Initialize each current pointer
                for (k=0; k<currDim; k++) {
                    *current = (void*)tail;
                    tail += nextDim; // Make room for new array
                    ++current;
                }
            }
        }
        // Save start of actual data's memory
        dataStart = tail;
        // Set up the pointers-to-data
        u8 **itemCurrent = (u8**)current;
        u8 *itemTail = (u8*)tail;
        // Slide the window
        prevDim = currDim;
        currDim = nextDim;
        nextDim = dims[i];
        // One array for each last-level parent
        for (j=0; j<prevDim; j++) {
            // Initialize each current pointer
            for (k=0; k<currDim; k++) {
                *itemCurrent = itemTail;
                itemTail += itemSize * nextDim; // Make room for new items
                ++itemCurrent;
            }
        }
        ASSERT(itemTail == ((u8*)ptrs + sum));
    }
    ///////////////////////////////////////
    // Return the initialized datablock
    ///////////////////////////////////////
    *out = block;
    return dataStart;
}

void *cncMalloc(size_t count) {
    ocrGuid_t handle;
    void *data;
    // add space for our meta-data
    count += _CNC_ITEM_META_SIZE;
    // allocate datablock
    // XXX - do I need to check for busy (and do a retry)?
    ocrDbCreate(&handle, &data, count, DB_PROP_NONE, NULL_GUID, NO_ALLOC);
    ASSERT(data && "ERROR: out of memory");
    // store guid
    *(ocrGuid_t*)data = handle;
    // set cookie
    #ifdef CNC_DEBUG
    u8 *bytes = data;
    u64 *cookie = (u64*)&bytes[sizeof(ocrGuid_t)];
    *cookie = _CNC_ITEM_COOKIE;
    #endif
    // return offset user pointer
    return _cncItemDataPtr(data);
}

void cncFree(void *itemPtr) {
    if (itemPtr) {
        _cncItemCheckCookie(itemPtr);
        ocrDbDestroy(_cncItemGuid(itemPtr));
    }
}
