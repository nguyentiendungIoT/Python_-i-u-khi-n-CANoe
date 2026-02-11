// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <memory>

#include "ClaMacros.hpp"
#include "IClaServiceSil.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Establish a connection with CANoe for all distributed object members
 *         get by `GetDo()`. Must be called once before data can be sent. */
CLAFUNCTIONDEF void Connect();

/*! \brief Establish a connection with CANoe for all distributed object members
 *         get by `GetDo()`. Must be called once before data can be sent.
 *  \param participantName The name of the SIL Kit participant. */
CLAFUNCTIONDEF void Connect(const std::string& participantName);

/*! \brief Establish a connection with CANoe for all distributed object members
 *         get by `GetDo()`. Must be called once before data can be sent.
 *  \param participant The Participant used to connect to CANoe. */
CLAFUNCTIONDEF void Connect(SilKit::IParticipant* participant);

/*! \brief Closes a connection with CANoe for all distributed object members
 *         get by `GetDo()`, which was previously established via `Connect()`. */
CLAFUNCTIONDEF void Disconnect();

/*! \brief Creates a new CLA service object for the SIL use case.
 *  \returns the created CLA service instance. */
CLAFUNCTIONDEF std::shared_ptr<IClaServiceSil> CreateClaService();

} // namespace Cla
} // namespace CANoe
} // namespace Vector
