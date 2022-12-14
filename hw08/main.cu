#include <cstdio>
#include <cuda_runtime.h>
#include "CudaAllocator.h"
#include "helper_cuda.h"
#include <cmath>
#include <vector>
// #include <thrust/device_vector.h>  // 如果想用 thrust 也是没问题的
#include <thrust/universal_vector.h>


__global__ void fill_sin(int *arr, int n) {
    for(int i=blockDim.x * blockIdx.x + threadIdx.x; i<n; i+=blockDim.x * gridDim.x)
    {
        arr[i]=sinf(i);
    }
}

__global__ void filter_positive(int *counter, int *res, int const *arr, int n) {
    for(int i=blockDim.x * blockIdx.x + threadIdx.x; i<n; i+=blockDim.x * gridDim.x)
    {
        if(arr[i]>=0)
        {
            int loc = atomicAdd(counter,1);
            res[loc] = n;
        }
    }
}

__host__ int main() {
    constexpr int n = 1<<24;
    // std::vector<int, CudaAllocator<int>> arr(n); //why error
    // std::vector<int, CudaAllocator<int>> res(n);
    // std::vector<int, CudaAllocator<int>> counter(1);

    thrust::universal_vector<int> arr(n); //OK
    thrust::universal_vector<int> res(n);
    thrust::universal_vector<int> counter(1);
    fill_sin<<<64, 512>>>(&*arr.data(), n);
    filter_positive<<<64, 512>>>(&*counter.data(), &*res.data(), &*arr.data(), n);

    // int * arr,*res,*counter;   //OK
    // checkCudaErrors(cudaMallocManaged(&arr,sizeof(int)*n));
    // checkCudaErrors(cudaMallocManaged(&res,sizeof(int)*n));
    // checkCudaErrors(cudaMallocManaged(&counter,sizeof(int)));
    // fill_sin<<<64, 512>>>(arr, n);
    // filter_positive<<<64, 512>>>(counter, res, arr, n);

    checkCudaErrors(cudaDeviceSynchronize());

    if (counter[0] <= n / 50) {
        printf("Result too short! %d <= %d\n", counter[0], n / 50);
        return -1;
    }
    for (int i = 0; i < counter[0]; i++) {
        if (res[i] < 0) {
            printf("Wrong At %d: %f < 0\n", i, res[i]);
            return -1;  // 突然想起了ICPC有一年队名叫“蓝翔WA掘机”的，笑不活了:)
        }
    }

    printf("All Correct!\n");  // 还有个队名叫“AC自动机”的，和隔壁“WAWA大哭”对标是吧:)
    return 0;
}
