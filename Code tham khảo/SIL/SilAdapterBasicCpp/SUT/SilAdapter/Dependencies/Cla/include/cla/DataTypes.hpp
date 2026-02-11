// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <cstdint>

namespace Vector {
namespace CANoe {
namespace Cla {

using Time = int64_t; // Placeholder
using CallbackHandle = int32_t;
using ContainerSize = uint32_t;

enum class MemberType
{
  DataMember,
  Method,
  Event,
  Field,
};

enum class MemberDirection
{
  Consumed,
  Provided,
  Internal
};

enum class ValueState
{
  Offline,
  Measured
};

enum class ValueRepresentation
{
  Phys,
  Raw,
  Impl
};

enum class ValueUpdateMode
{
  OnChange,
  OnUpdate
};

enum class CallState
{
  Initial,
  Called,
  Returned, //only on consumer side if response has been received
  Discarded //only on consumer side if call has been discarded (e.g. due to a timeout)
};

/*! \brief Enumeration representing the `TxTrigger` attribute for provided data, event and field member,
 *         which specifies when a provided value is transmitted. */
enum class TxTrigger
{
  OnUpdate, //!< The provided value is transmitted when it is updated.
  OnChange  //!< The provided value is transmitted when it is changed.
};

#ifdef _CANOECLA
enum class BlueprintResult
{
  OK,
  AttributeNotFound,
  NetworkNotFound,
  MemberNotFound,
  ObjectOfWrongInterface,
};

enum class DynamicDOErrorCode
{
  Success,
  InvalidDescription,
  UnknownDO,
  NameNotUnique,
  InvalidDOName,
  InvalidPath,
  InvalidInterface,
  InvalidNamespace,
  NamespaceIsNotUserExtensible,
  InvalidAttribute,
  NotInOfflineMode,
};

enum class AttributeDefinitionType
{
  UInt64,
  Int64,
  Double,
  String,
};
#endif

} // namespace Cla
} // namespace CANoe
} // namespace Vector
