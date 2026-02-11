// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

namespace Vector {
namespace CANoe {
namespace TypeLib {

/*!
 * \brief Function to mark parameters or variables which are maybe unused
 *        to suppress compiler warnings. Can be replaced by [[maybe_unused]]
 *        with C++17.
 */
template <class... Args>
inline void MaybeUnused(Args&&...)
{
}

} // namespace TypeLib
} // namespace CANoe
} // namespace Vector
