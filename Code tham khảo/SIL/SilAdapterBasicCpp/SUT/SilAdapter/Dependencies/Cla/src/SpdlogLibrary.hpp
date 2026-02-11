// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#endif

#define FMT_USE_UDL_TEMPLATE

#include "spdlog/logger.h"
#include "spdlog/sinks/stdout_sinks.h"

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif