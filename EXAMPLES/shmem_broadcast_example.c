for (i=0; i < _SHMEM_BCAST_SYNC_SIZE; i++) {
        pSync[i] = _SHMEM_SYNC_VALUE;
}
shmem_barrier_all(); /* Wait for all PEs to initialize pSync */
shmem_broadcast64(target, source, nlong, 0, 4, 0, 4, pSync);


