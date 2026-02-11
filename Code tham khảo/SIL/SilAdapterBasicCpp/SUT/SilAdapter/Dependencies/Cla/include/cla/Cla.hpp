// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "ClaMacros.hpp"
#include "IClaService.hpp"

namespace Vector {
  namespace CANoe {
    namespace Cla {
#ifdef _CANOECLA
      CLAFUNCTIONDEF std::shared_ptr<IClaService> CreateClaService();
#endif
    } // namespace Cla
  } // namespace CANoe
} // namespace Vector