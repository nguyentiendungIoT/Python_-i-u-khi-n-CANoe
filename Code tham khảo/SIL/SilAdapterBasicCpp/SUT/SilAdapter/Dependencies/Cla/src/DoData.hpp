// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "cla/IDoData.hpp"

#include "DoMember.hpp"
#include "ValueEntity.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Class representing a data member of a distributed object. */
class DoData : public DoMember, public IDoData
{
public:
  // Public methods
  // -------------------

  /*! \brief Instantiates a new data member.
   *  \param fullPath The full path of the data member, i.e. "namespace::do.dataMember".
   *  \param direction The direction of the data member.
   *  \param logger The logger instance used by the data member and child elements for logging.
   *  \param workerThreadService service to add custom tasks to be executed by a worker thread. */
  DoData(std::string fullPath,
    MemberDirection direction,
    std::shared_ptr<spdlog::logger> logger,
    std::shared_ptr<WorkerThreadService> workerThreadService);

  // -------------------
  // IDoData
  // -------------------

  std::shared_ptr<IValueEntity> GetValue() const override;

protected:
  // Protected Members
  // -------------------

  std::shared_ptr<ValueEntity> mValue;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
