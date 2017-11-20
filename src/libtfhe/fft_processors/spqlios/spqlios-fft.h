#ifndef SPQLIOS_FFT_H
#define SPQLIOS_FFT_H

#ifdef __cplusplus
extern "C" {
#endif

void *new_fft_table(int32_t nn);
double *fft_table_get_buffer(const void *tables);
void *new_ifft_table(int32_t nn);
double *ifft_table_get_buffer(const void *tables);
void fft_model(const void *tables);
void ifft_model(void *tables);
void fft(const void *tables, double *data);
void ifft(const void *tables, double *data);

#ifdef __cplusplus
}
#endif

#endif //SPQLIOS_FFT_H
