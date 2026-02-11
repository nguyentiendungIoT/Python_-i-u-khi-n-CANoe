// This code was generated automatically. Changes to this file may
// cause incorrect behavior and will be lost if the code is regenerated.

#pragma once

// ----------------------------------------
// TypeLib Generated Headers
// ----------------------------------------

#include "TypeLib/RoomTemperatureControl.hpp"

#ifdef SIL_ADAPTER_INCLUDE_SIL_KIT_API
#include "silkit/SilKit.hpp"
#endif

namespace Vector {
namespace CANoe {
namespace SilAdapter {

/*!
 * \brief Connects the SIL Adapter with CANoe.
 *        Must be called once before the SIL Adapter can be used.
 */
void Connect();

#ifdef SIL_ADAPTER_INCLUDE_SIL_KIT_API
 /*!
 * \brief Connects the SIL Adapter with CANoe.
 *        Must be called once before the SIL Adapter can be used.
 * \param participant The SIL Kit participant used to initialize the SIL Adapter.
 */
void Connect(SilKit::IParticipant* participant);
#endif

/*!
 * \brief Disconnects the SIL Adapter from CANoe.
 */
void Disconnect();

} // namespace SilAdapter
} // namespace CANoe
} // namespace Vector
