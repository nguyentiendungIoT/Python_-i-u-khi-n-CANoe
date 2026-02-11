// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <array>

#include "DistributedObject.hpp"
#include "TypeLib/DoConsumedData.hpp"
#include "TypeLib/DoConsumedEvent.hpp"
#include "TypeLib/DoConsumedField.hpp"
#include "TypeLib/DoConsumedMethod.hpp"
#include "TypeLib/DoProvidedData.hpp"
#include "TypeLib/DoProvidedEvent.hpp"
#include "TypeLib/DoProvidedField.hpp"
#include "TypeLib/DoProvidedMethod.hpp"
#include "TypeLib/EmbeddedMember.hpp"

#include "cla/IClaService.hpp"

namespace Vector {
namespace CANoe {
namespace TypeLib {

// Forward Declarations
class Accessor;

template <class T, size_t N> using EmbeddedObjectArray = std::array<DistributedObject<T>, N>;
template <class T, size_t N> using DistributedObjectArray = std::array<DistributedObject<T>, N>;

/*! Abstract base class for all distributed objects and interfaces. */
class DistributedObjectBase
{
protected:
  // Aliases
  // -------------------

  using _Get = Get;
  using _Set = Set<true, false>;
  using _SetNoReturn = Set<false, false>;
  using _SetOneWay = Set<false, true>;
  using _Notify = Notify;

  // Protected Methods
  // -------------------

  DistributedObjectBase() = default;
  virtual ~DistributedObjectBase() = default;

  friend class Accessor; // To initialize / reset

  /*! \brief Initializes the distributed object.
   *  \param claService The CLA service to get the distributed object.
   *  \param doPath The full path of the distributed object.
   *  \param subMemberPath The sub-member path if the DO is not the root DO, e.g. an embedded member. */
  virtual void _Init(Cla::IClaService* claService, const std::string& doPath, const std::string& subMemberPath);

  /*! \brief Waits for the first transmission from CANoe for all members on which the blocking was enabled. */
  virtual void _WaitForFirstTransmission();

  // Protected Members
  // -------------------

  std::shared_ptr<Cla::IDo> mDo; ///< Handle to distributed object of the CLA.
};

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
