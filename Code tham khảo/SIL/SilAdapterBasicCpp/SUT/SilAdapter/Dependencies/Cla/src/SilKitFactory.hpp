// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include "ISilKitFactory.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Factory for SIL Kit objects created from free functions. */
class SilKitFactory : public ISilKitFactory
{
public:
  // -------------------
  // ISilKitFactory
  // -------------------

  std::shared_ptr<SilKit::Config::IParticipantConfiguration> ParticipantConfigurationFromFile(
    const std::string& path) override;
  std::shared_ptr<SilKit::Config::IParticipantConfiguration> ParticipantConfigurationFromString(
    const std::string& config) override;
  std::unique_ptr<SilKit::IParticipant> CreateParticipant(
    std::shared_ptr<SilKit::Config::IParticipantConfiguration> config,
    const std::string& participantName) override;
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
