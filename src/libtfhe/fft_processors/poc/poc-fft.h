#ifndef POC_FFT_H
#define POC_FFT_H

#ifdef __cplusplus
extern "C" {
#endif

void* new_fft_table(int nn);
void* new_ifft_table(int nn);
void fft_model(const void* tables, double* c);
void ifft_model(void* tables, double* c);
void fft(const void* tables, double* data);
void ifft(const void* tables, double* data);

#ifdef __cplusplus
}
#endif

#endif //POC_FFT_H
