// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "SilKitFactory.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// -------------------
// ISilKitFactory
// -------------------

std::shared_ptr<SilKit::Config::IParticipantConfiguration> SilKitFactory::ParticipantConfigurationFromFile(
  const std::string& path)
{
  return SilKit::Config::ParticipantConfigurationFromFile(path);
}

std::shared_ptr<SilKit::Config::IParticipantConfiguration> SilKitFactory::ParticipantConfigurationFromString(
  const std::string& config)
{
  return SilKit::Config::ParticipantConfigurationFromString(config);
}

std::unique_ptr<SilKit::IParticipant> SilKitFactory::CreateParticipant(
  std::shared_ptr<SilKit::Config::IParticipantConfiguration> config,
  const std::string& participantName)
{
  return SilKit::CreateParticipant(config, participantName);
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
