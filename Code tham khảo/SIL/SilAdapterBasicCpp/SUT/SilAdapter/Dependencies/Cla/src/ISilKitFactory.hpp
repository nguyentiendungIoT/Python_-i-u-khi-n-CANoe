// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "silkit/SilKit.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Factory for SIL Kit objects created from free functions. */
class ISilKitFactory
{
public:
  // Public Methods
  // ------------------

  virtual ~ISilKitFactory() = default;

  /*! \brief Creates a SIL Kit configuration from a file.
   *  \param path The path to the configuration file.
   *  \returns The SIL Kit configuration. */
  virtual std::shared_ptr<SilKit::Config::IParticipantConfiguration> ParticipantConfigurationFromFile(
    const std::string& path) = 0;

  /*! \brief Creates a SIL Kit configuration from a string.
   *  \param config The configuration string.
   *  \returns The SIL Kit configuration. */
  virtual std::shared_ptr<SilKit::Config::IParticipantConfiguration> ParticipantConfigurationFromString(
    const std::string& config) = 0;

  /*! \brief Creates a SIL Kit participant.
   *  \param config The participant configuration.
   *  \param participantName The name of the participant.
   *  \returns The created participant. */
  virtual std::unique_ptr<SilKit::IParticipant> CreateParticipant(
    std::shared_ptr<SilKit::Config::IParticipantConfiguration> config,
    const std::string& participantName) = 0;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
