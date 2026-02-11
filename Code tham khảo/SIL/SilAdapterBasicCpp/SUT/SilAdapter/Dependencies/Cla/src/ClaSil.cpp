// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include "cla/ClaSil.hpp"

#include "ClaService.hpp"
#include "SilKitFactory.hpp"
#include "SpdlogLibrary.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

static std::shared_ptr<spdlog::logger> CreateClaLogger()
{
  auto sink = std::make_shared<spdlog::sinks::stdout_sink_mt>();
  auto logger = std::make_shared<spdlog::logger>("ClaLogger", sink);
  logger->set_level(spdlog::level::info);
  // For the current spdlog version, the 'level' strings can be empty
  // if something is logged before main() due static initialization order.
  // https://github.com/gabime/spdlog/issues/1791
  logger->set_pattern("[%l] %v"); // [level] <Message>
  return logger;
}

static IClaServiceSil& GetGlobalClaService()
{
  static ClaService s(CreateClaLogger(), std::make_unique<SilKitFactory>());
  return s;
}

CLAFUNCTIONDEF void Connect()
{
  GetGlobalClaService().Connect();
}

CLAFUNCTIONDEF void Connect(const std::string& participantName)
{
  GetGlobalClaService().Connect(participantName);
}

CLAFUNCTIONDEF void Connect(SilKit::IParticipant* participant)
{
  GetGlobalClaService().Connect(participant);
}

CLAFUNCTIONDEF void Disconnect()
{
  GetGlobalClaService().Disconnect();
}

CLAFUNCTIONDEF std::shared_ptr<IDo> GetDo(const std::string& path)
{
  return GetGlobalClaService().GetDo(path);
}

CLAFUNCTIONDEF std::shared_ptr<IClaServiceSil> CreateClaService()
{
  return std::make_shared<ClaService>(CreateClaLogger(), std::make_unique<SilKitFactory>());
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
