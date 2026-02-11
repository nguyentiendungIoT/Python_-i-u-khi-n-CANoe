// Copyright (c) Vector Informatik GmbH. All rights reserved.

#include <chrono>
#include <cstdlib> //getenv

using namespace std::chrono_literals;

#include "cla/ClaError.hpp"

#include "ClaService.hpp"
#include "DoInstContainer.hpp"
#include "SilKitFactory.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

static void OptionallySetFromEnvVar(std::string& target, const char* varName)
{
  auto* varValue = ::getenv(varName);
  if (varValue)
  {
    target.assign(varValue);
  }
}

// Public Methods
// ------------------

ClaService::ClaService(std::shared_ptr<spdlog::logger> logger, std::unique_ptr<ISilKitFactory> silKitFactory) :
  mLogger(std::move(logger)),
  mSilKitFactory(std::move(silKitFactory)),
  mWorkerThreadService(std::make_shared<WorkerThreadService>(mLogger)),
  mDoRegistry(mLogger, mWorkerThreadService)
{
#ifdef CLA_ENABLE_DEBUG_LOGGING
  mLogger->set_level(spdlog::level::trace);
#endif
}

ClaService::~ClaService()
{
  mWorkerThreadService->StopWorkerThread();
}

void ClaService::Connect()
{
  Connect("SilAdapter");
}

void ClaService::Connect(const std::string& participantName)
{
  const auto config = CreateParticipantConfiguration();
  CreateParticipant(config, participantName);
  Connect(mParticipantManaged.get());
}

void ClaService::Connect(SilKit::IParticipant* participant)
{
  assert(!mParticipantHandle && "CLA service is already connected!");
  mParticipantHandle = participant;

  mDoRegistry.Connect(participant);

  if (mLifecycle)
  {
    mLifecycleState = mLifecycle->StartLifecycle();
  }

  mLogger->debug("CLA service connected.");
}

void ClaService::Disconnect()
{
  assert(mParticipantHandle && "CLA service is not connected!");

  if (mLifecycle)
  {
    mLifecycle->Stop("Disconnect");
    mLifecycleState.wait();
    mLifecycle = nullptr;
  }

  mDoRegistry.Disconnect();
  mParticipantManaged.reset();
  mParticipantHandle = nullptr;
  mWorkerThreadService->StopWorkerThread();

  mLogger->debug("CLA service disconnected.");
}
std::shared_ptr<IDo> ClaService::GetDo(const std::string& path)
{
  return mDoRegistry.GetDo(path);
}
std::shared_ptr<IDoInstContainer> ClaService::GetDoInstContainer(const std::string& path)
{
  if (mDoInstContainerMap.count(path) > 0)
  {
    return mDoInstContainerMap[path];
  }

  mDoInstContainerMap[path] = std::make_shared<DoInstContainer>(path, mDoRegistry);
  return mDoInstContainerMap[path];
}

// Private Methods
// ------------------

ParticipantConfiguration ClaService::CreateParticipantConfiguration() const
{
  std::string configPath;
  OptionallySetFromEnvVar(configPath, "SILKIT_CONFIG_PATH");

  try
  {
    if (!configPath.empty())
    {
      return ParticipantConfiguration{configPath, mSilKitFactory->ParticipantConfigurationFromFile(configPath)};
    }
  }
  catch (const SilKit::ConfigurationError& e)
  {
    const auto error = "Error creating participant configuration from file '" + configPath + "': " + e.what();
    mLogger->error(error);
    throw ClaError(error);
  }

  return ParticipantConfiguration{"", mSilKitFactory->ParticipantConfigurationFromString("")};
}
void ClaService::CreateParticipant(const ParticipantConfiguration& config, const std::string& participantName)
{
  auto retry = 0;
  auto wasWarned = false;
  while (true)
  {
    try
    {
      mParticipantManaged = mSilKitFactory->CreateParticipant(config.participantConfiguration, participantName);
      break;
    }
    // Currently a std::runtime_error is thrown instead of a SilKit::SilKitError if the registry couldn't be reached.
    // But cannot catch std::runtime_error, because SilKit::SilKitError is derived from std::exception directly.
    // If the SilKit switches to SilKitError it would disable this retry mechanism.
    catch (const std::exception& e)
    {
      auto usedConfigMsg =
        config.configPath.empty()
          ? "Used default SIL Kit Configuration."
          : fmt::format("Used configuration file set by environment variable SILKIT_CONFIG_PATH: {0}.",
              config.configPath);

      const auto msg =
        fmt::format("Couldn't connect because SIL Kit participant creation failed: {0}. {1} Retrying...",
          e.what(),
          usedConfigMsg);

      if (++retry == 5)
      {
        // We print that once after 10 seconds.
        mLogger->warn(msg);
        wasWarned = true;
      }
      else
      {
        mLogger->debug(msg);
      }
    }

    std::this_thread::sleep_for(2s);
  }
  if (wasWarned)
  {
    mLogger->info("Connected successfully after retrying.");
  }

  SilKit::Services::Orchestration::LifecycleConfiguration lifecycleConfiguration;
  lifecycleConfiguration.operationMode = SilKit::Services::Orchestration::OperationMode::Autonomous;
  mLifecycle = mParticipantManaged->CreateLifecycleService(lifecycleConfiguration);
}

} // namespace Cla
} // namespace CANoe
} // namespace Vector
