/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2020 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#include "xenia/gpu/xenos.h"

#include "xenia/base/math.h"

namespace xe {
namespace gpu {
namespace xenos {

float Float20e4To32(uint32_t f24) {
  // Based on CFloat24 from d3dref9.dll and the 6e4 code from:
  // https://github.com/Microsoft/DirectXTex/blob/master/DirectXTex/DirectXTexConvert.cpp
  // 6e4 has a different exponent bias allowing [0,512) values, 20e4 allows
  // [0,2).
  f24 &= 0xFFFFFF;
  if (!f24) {
    return 0.0f;
  }
  uint32_t mantissa = f24 & 0xFFFFF;
  uint32_t exponent = f24 >> 20;
  if (!exponent) {
    // Normalize the value in the resulting float.
    // do { Exponent--; Mantissa <<= 1; } while ((Mantissa & 0x100000) == 0)
    uint32_t mantissa_lzcnt = xe::lzcnt(mantissa) - (32 - 21);
    exponent = uint32_t(1 - mantissa_lzcnt);
    mantissa = (mantissa << mantissa_lzcnt) & 0xFFFFF;
  }
  uint32_t f32 = ((exponent + 112) << 23) | (mantissa << 3);
  return *reinterpret_cast<const float*>(&f32);
}

const char* GetColorRenderTargetFormatName(ColorRenderTargetFormat format) {
  switch (format) {
    case ColorRenderTargetFormat::k_8_8_8_8:
      return "k_8_8_8_8";
    case ColorRenderTargetFormat::k_8_8_8_8_GAMMA:
      return "k_8_8_8_8_GAMMA";
    case ColorRenderTargetFormat::k_2_10_10_10:
      return "k_2_10_10_10";
    case ColorRenderTargetFormat::k_2_10_10_10_FLOAT:
      return "k_2_10_10_10_FLOAT";
    case ColorRenderTargetFormat::k_16_16:
      return "k_16_16";
    case ColorRenderTargetFormat::k_16_16_16_16:
      return "k_16_16_16_16";
    case ColorRenderTargetFormat::k_16_16_FLOAT:
      return "k_16_16_FLOAT";
    case ColorRenderTargetFormat::k_16_16_16_16_FLOAT:
      return "k_16_16_16_16_FLOAT";
    case ColorRenderTargetFormat::k_2_10_10_10_AS_10_10_10_10:
      return "k_2_10_10_10_AS_10_10_10_10";
    case ColorRenderTargetFormat::k_2_10_10_10_FLOAT_AS_16_16_16_16:
      return "k_2_10_10_10_FLOAT_AS_16_16_16_16";
    case ColorRenderTargetFormat::k_32_FLOAT:
      return "k_32_FLOAT";
    case ColorRenderTargetFormat::k_32_32_FLOAT:
      return "k_32_32_FLOAT";
    default:
      return "kUnknown";
  }
}

const char* GetDepthRenderTargetFormatName(DepthRenderTargetFormat format) {
  switch (format) {
    case DepthRenderTargetFormat::kD24S8:
      return "kD24S8";
    case DepthRenderTargetFormat::kD24FS8:
      return "kD24FS8";
    default:
      return "kUnknown";
  }
}

}  // namespace xenos
}  // namespace gpu
}  // namespace xe
