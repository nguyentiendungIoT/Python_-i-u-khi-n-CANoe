// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include <utility>

#include "cla/ClaError.hpp"

#include "Do.hpp"
#include "DoRegistry.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

// Public Methods
// ------------------

DoRegistry::DoRegistry(std::shared_ptr<spdlog::logger> logger,
  std::shared_ptr<WorkerThreadService> workerThreadService) :
  mLogger(std::move(logger)),
  mWorkerThreadService(std::move(workerThreadService))
{
}

std::shared_ptr<IDo> DoRegistry::GetDo(const std::string& path)
{
  if (path.empty())
  {
    throw ClaError("Cannot get new DO with an empty path.");
  }

  auto&& it = mDoInstances.find(path);
  if (it != mDoInstances.end())
  {
    return it->second;
  }

  if (mParticipant)
  {
    throw ClaError("Cannot get new DO while the CLA service is connected.");
  }

  auto newInstance = std::make_shared<Do>(path, mLogger, mWorkerThreadService);

  mDoInstances[path] = newInstance;
  mLogger->debug(fmt::format("Created DO '{}'", path)); // Explicit fmt::format because of VS2017
  return newInstance;
}

void DoRegistry::Connect(SilKit::IParticipant* participant)
{
  mParticipant = participant;
  for (auto& doInstance : mDoInstances)
  {
    doInstance.second->Connect(mParticipant);
  }
}

void DoRegistry::Disconnect()
{
  for (auto& doInstance : mDoInstances)
  {
    doInstance.second->Disconnect();
  }
  mParticipant = nullptr;
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
