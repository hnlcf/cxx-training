// Copyright (c) 2022 Changfeng Lou. All rights reserved.

#pragma once

#include <cstdint>

namespace mini_std {
char *
encodeVarint32(char *dst, uint32_t val);
}
