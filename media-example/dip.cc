#include <emscripten.h>
#include <cmath>

#define KH 3
#define KW 3

char kernel[KH][KW];
unsigned char data[921600];

extern "C" {
  EMSCRIPTEN_KEEPALIVE auto* cppGetkernelPtr() { return kernel; }
  EMSCRIPTEN_KEEPALIVE auto* cppGetDataPtr() { return data; }
  EMSCRIPTEN_KEEPALIVE void cppConvFilter(
    int width, 
    int height,
    int divisor) {
    const int half = std::floor(KH / 2);
    for (int y = half; y < height - half; ++y) {
      for (int x = half; x < width - half; ++x) {
        int px = (y * width + x) * 4;
        int r = 0, g = 0, b = 0;
        for (int cy = 0; cy < KH; ++cy) {
          for (int cx = 0; cx < KW; ++cx) {
            const int cpx = ((y + (cy - half)) * width + (x + (cx - half))) * 4;
            r += data[cpx + 0] * kernel[cy][cx];
            g += data[cpx + 1] * kernel[cy][cx];
            b += data[cpx + 2] * kernel[cy][cx];
          }
        }                 
        data[px + 0] =  ((r / divisor) > 255) ? 255 : ((r / divisor) < 0) ? 0 : r / divisor;
        data[px + 1] =  ((g / divisor) > 255) ? 255 : ((g / divisor) < 0) ? 0 : g / divisor;
        data[px + 2] =  ((b / divisor) > 255) ? 255 : ((b / divisor) < 0) ? 0 : b / divisor;
      }
    }
  }
}
