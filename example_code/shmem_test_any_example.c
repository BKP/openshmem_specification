#include <shmem.h>
#include <stdlib.h>

#define N 100

int main(void)
{
  int total_sum = 0;

  shmem_init();
  int mype = shmem_my_pe();
  int npes = shmem_n_pes();

  int my_data = malloc(N * sizeof(int));
  int *all_data = shmem_malloc(N * npes * sizeof(int));

  int *flags = shmem_calloc(npes, sizeof(int));
  int *status = calloc(npes, sizeof(int));

  for (int i = 0; i < N; i++)
      my_data[i] = mype*N + i;

  for (int i = 0; i < npes; i++)
      shmem_put_nbi(&all_data[mype*N], my_data, N, i);

  shmem_fence();

  for (int i = 0; i < npes; i++)
      shmem_p(&flags[mype], 1, i);
  
  int ncompleted = 0, zero_processed = 0;

  while (ncompleted < npes) {
      int completed_idx = shmem_test_any(flags, npes, status, SHMEM_CMP_NE, 0); 
      if (completed_idx == 0 && status[0] == 1 && !zero_processed) {
          ncompleted++;
          zero_processed = 1;
          for (int j = 0; j < N; j++)
              total_sum += all_data[completed_idx * N + j];
      } else  {
          ncompleted++;
          for (int j = 0; j < N; j++)
              total_sum += all_data[completed_idx * N + j];
      }
  }

  shmem_finalize();
  return 0;
}
