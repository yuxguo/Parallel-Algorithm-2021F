#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define output_tensor(n, h, w, c) output_tensor[n * HO * WO * CO + h * WO * CO + w * CO + c]
#define input_tensor(n, h, w, c) input_tensor[n * HI * WI * CI + h * WI * CI + w * CI + c]
#define kernel(h, w, ci, co) kernel[h * KW * CI * CO + w * CI * CO + ci * CO + co]

int main(void)
{
    // NCHW x HWIO = NCHW
    int N = 32, H = 224, W = 224, KH = 7, KW = 7, CI = 64, CO = 256, SH = 3, SW = 3, PH = 1, PW = 1;
    // int N = 8, H = 7, W = 7, KH = 3, KW = 3, CI = 16, CO = 16, SH = 1, SW = 1, PH = 1, PW = 1;

    int HI = (H + 2 * PH);
    int WI = (W + 2 * PW);
    int HO = ((H + 2 * PH - KH) / SH + 1);
    int WO = ((W + 2 * PW - KW) / SW + 1);
    float *input_tensor, *kernel, *output_tensor;
    srand48(time(NULL));
    clock_t start, end;

    input_tensor = (float *)malloc(N * HI * WI * CI * sizeof(float));
    kernel = (float *)malloc(KH * KW * CI * CO * sizeof(float));
    output_tensor = (float *)malloc(N * HO * WO * CO * sizeof(float));

#pragma omp parallel for
    for (int i = 0; i < N * HI * WI * CI; ++i)
    {
        input_tensor[i] = drand48();
    }
#pragma omp parallel for
    for (int i = 0; i < KH * KW * CI * CO; ++i)
    {
        kernel[i] = drand48();
    }
    printf("Start computing: ");

    start = clock();
    for (int n = 0; n < N; ++n)
    {
        for (int ho = 0; ho < HO; ++ho)
        {
            for (int wo = 0; wo < WO; ++wo)
            {
#pragma omp parallel for
                for (int co = 0; co < CO; ++co)
                {
                    output_tensor(n, ho, wo, co) = 0.0f;
                    for (int kh = 0; kh < KH; ++kh)
                    {
                        for (int kw = 0; kw < KW; ++kw)
                        {
                            for (int ci = 0; ci < CI; ++ci)
                            {
                                output_tensor(n, ho, wo, co) += kernel(kh, kw, ci, co) * input_tensor(n, (ho * SH + kh), (wo * SW + kw), co);
                            }
                        }
                    }
                }
            }
        }
    }
    end = clock();

    printf("time:%.3f ms\n", (double)(end - start) / CLOCKS_PER_SEC * 1e3);

    return 0;
}