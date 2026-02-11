// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <map>

#include "cla/IClaServiceSil.hpp"

#include "DoRegistry.hpp"
#include "ISilKitFactory.hpp"
#include "SpdlogLibrary.hpp"
#include "WorkerThreadService.hpp"

namespace Vector {
namespace CANoe {
namespace Cla {

/*! \brief Struct holding information about the used SIL Kit configuration. */
struct ParticipantConfiguration
{
  std::string configPath;
  std::shared_ptr<SilKit::Config::IParticipantConfiguration> participantConfiguration;
};

/*! \brief Class representing the entry point for all CLA functionality. */
class ClaService : public IClaServiceSil
{
public:
  // Public Methods
  // ------------------

  /*! \brief Instantiates a new CLA service.
   *  \param logger The logger used for logging by the CLA service and all subsystems.
   *  \param silKitFactory The factory for the SIL Kit objects. */
  ClaService(std::shared_ptr<spdlog::logger> logger, std::unique_ptr<ISilKitFactory> silKitFactory);
  ~ClaService() override;

  // -------------------
  // IClaService
  // -------------------

  void Connect() override;
  void Connect(const std::string& participantName) override;
  void Connect(SilKit::IParticipant* participant) override;
  void Disconnect() override;
  std::shared_ptr<IDo> GetDo(const std::string& path) override;
  std::shared_ptr<IDoInstContainer> GetDoInstContainer(const std::string& path) override;

private:
  // Private Members
  // ------------------

  std::shared_ptr<spdlog::logger> mLogger;
  std::unique_ptr<ISilKitFactory> mSilKitFactory;
  SilKit::IParticipant* mParticipantHandle = nullptr;
  SilKit::Services::Orchestration::ILifecycleService* mLifecycle = nullptr;
  std::future<SilKit::Services::Orchestration::ParticipantState> mLifecycleState;
  std::unique_ptr<SilKit::IParticipant> mParticipantManaged = nullptr;
  std::shared_ptr<WorkerThreadService> mWorkerThreadService;
  DoRegistry mDoRegistry;
  std::map<std::string, std::shared_ptr<IDoInstContainer>> mDoInstContainerMap;

  // Private methods
  // ------------------

  ParticipantConfiguration CreateParticipantConfiguration() const;
  void CreateParticipant(const ParticipantConfiguration& config,
    const std::string& participantName);
};

} // namespace Cla
} // namespace CANoe
} // namespace Vector
