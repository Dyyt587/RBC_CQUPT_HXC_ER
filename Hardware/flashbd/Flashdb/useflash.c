/*
 * Copyright (c) 2020, Armink, <armink.ztl@gmail.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief basic KV samples.
 *
 * basic Key-Value Database KV feature samples
 * get and show currnet boot counts
 */

#include <flashdb.h>

#ifdef FDB_USING_KVDB

#define FDB_LOG_TAG "[sample][kvdb][basic]"


void flashdb_creat_data(fdb_kvdb_t kvdb,const char *key,int data)
{
	struct fdb_blob blob;

    FDB_INFO("==================== flashdb_creat_data_sample ====================\n");

    { /* CREATE new Key-Value */
//		int kvdb_data0 = 0;

        /* It will create new KV node when "temp" KV not in database.
         * fdb_blob_make: It's a blob make function, and it will return the blob when make finish.
         */
        fdb_kv_set_blob(kvdb, key, fdb_blob_make(&blob, &data, sizeof(data)));
        FDB_INFO("create the '%s' blob KV, value is: %d\n", key, data);
    }
}

void flashdb_get_set_data(fdb_kvdb_t kvdb,const char *key,int data)
{
    struct fdb_blob blob;
	int kvdb_data0 = 0;

    FDB_INFO("==================== flashdb_get_set_data_sample ====================\n");
	
	
    { /* GET the KV value */
        /* get the "kvdb_data" KV value */
        fdb_kv_get_blob(kvdb, key, fdb_blob_make(&blob, &kvdb_data0, sizeof(kvdb_data0)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the '%s' value is %d\n", key, kvdb_data0);
        } else {
            FDB_INFO("get the '%s' failed\n", key);
			flashdb_creat_data(kvdb,key,data);
        }
    }

    { /* CHANGE the KV value */
        /* increase the boot count */
        kvdb_data0 = data;
        /* change the "kvdb_data" KV's value */
        fdb_kv_set_blob(kvdb, key, fdb_blob_make(&blob, &kvdb_data0, sizeof(kvdb_data0)));
        FDB_INFO("set the '%s' value to %d\n", key, kvdb_data0);
    }

    FDB_INFO("===========================================================\n");
}

void flashdb_get_data(fdb_kvdb_t kvdb,const char *key)
{
    struct fdb_blob blob;
	int kvdb_data0 = 0;

    FDB_INFO("==================== flashdb_get_data_sample ====================\n");
	
    { /* GET the KV value */
        /* get the "kvdb_data" KV value */
        fdb_kv_get_blob(kvdb, key, fdb_blob_make(&blob, &kvdb_data0, sizeof(kvdb_data0)));
        /* the blob.saved.len is more than 0 when get the value successful */
        if (blob.saved.len > 0) {
            FDB_INFO("get the '%s' value is %d\n", key, kvdb_data0);
        } else {
            FDB_INFO("get the '%s' failed\n", key);
        }
    }

    FDB_INFO("===========================================================\n");
}

void flash_delete_data(fdb_kvdb_t kvdb,const char *key)
{
	fdb_kv_del(kvdb,key);
    FDB_INFO("delete the '%s' finish\n",key);
}

#endif /* FDB_USING_KVDB */



