#include "silkit/SilKit.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/can/string_utils.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/orchestration/ILifecycleService.hpp"
#include "silkit/services/orchestration/string_utils.hpp"

#include <atomic>
#include <chrono>
#include <thread>

using namespace std::chrono_literals;
using namespace SilKit::Services::Can;

namespace {

const uint32_t _maxEngineSpeed = 3400;
const uint32_t _speedChangeRateUp = 50;
const uint32_t _speedChangeRateDown = 200;

std::atomic<uint32_t> _currentEngineSpeed = 0;
bool _isAccelerating = true;

void UpdateEngineValue()
{
    // Update if speed increases or decreases
    if (_currentEngineSpeed == 0)
    {
        _isAccelerating = true;
    }
    if (_currentEngineSpeed == _maxEngineSpeed)
    {
        _isAccelerating = false;
    }

    // Update speed
    if (_isAccelerating)
    {
        _currentEngineSpeed += _speedChangeRateUp;
    }
    else
    {
        _currentEngineSpeed -= _speedChangeRateDown;
    }
}

void SendEngineSpeed(SilKit::Services::Can::ICanController* canController, uint32_t engineSpeed)
{
    std::array<uint8_t, 4> payload{};
    for (unsigned byteIndex = 0; byteIndex != payload.size(); ++byteIndex)
    {
        payload[byteIndex] = static_cast<uint8_t>(_currentEngineSpeed >> (byteIndex * 8u)) & UINT8_C(0xFF);
    }

    CanFrame frame{};
    frame.canId = 0x123;
    frame.dlc = 4;
    frame.dataField = SilKit::Util::MakeSpan(payload);

    canController->SendFrame(frame);
}

using clock = std::chrono::steady_clock;

auto SleepAndReturnElapsed(typename clock::duration duration) -> typename clock::duration
{
    const auto startTime = clock::now();
    std::this_thread::sleep_for(duration);
    return clock::now() - startTime;
}

const auto participantConfigurationString = R"raw(
{
  "Logging": {
    "Sinks": [
      {
        "Type": "Stdout",
        "Level": "Warn"
      }
    ]
  }
}
)raw";

constexpr auto title = R"raw(

     _____             _
    | ____|_ __   __ _(_)_ __   ___
    |  _| | '_ \ / _` | | '_ \ / _ \
    | |___| | | | (_| | | | | |  __/
    |_____|_| |_|\__, |_|_| |_|\___|
                 |___/

)raw";

} // namespace

int main(int argc, char** argv)
{
    if (argc < 2)
    {
        std::cerr << "Missing arguments! Start demo with: " << argv[0] << " <ParticipantName> [registryUri] [--async]"
                  << std::endl;
        return -1;
    }

    if (argc > 4)
    {
        std::cerr << "Too many arguments! Start demo with: " << argv[0] << "  <ParticipantName> [registryUri] [--async]"
                  << std::endl;
        return -1;
    }

    try
    {
        std::string participantName(argv[1]);

        std::string registryUri = "silkit://localhost:8500";

        bool runSync = true;

        std::vector<std::string> args;
        std::copy((argv + 2), (argv + argc), std::back_inserter(args));

        for (auto arg : args)
        {
            if (arg == "--async")
            {
                runSync = false;
            }
            else
            {
                registryUri = arg;
            }
        }

        auto ignitionInfo = false;

        const auto config = SilKit::Config::ParticipantConfigurationFromString(participantConfigurationString);
        auto participant = SilKit::CreateParticipant(config, participantName, registryUri);
        const auto logger = participant->GetLogger();

        std::atomic<bool> isIgnitionStatusOn = false;

        auto* canController = participant->CreateCanController("CAN_Engine", "easy");
        canController->AddFrameHandler([&](auto controller, CanFrameEvent canFrameEvent) {
            if (canFrameEvent.frame.canId == 0x67)
            {
                // Received IgnitionInfo message
                const bool desiredIsIgnitionStatusOn = (canFrameEvent.frame.dataField[0] == 0) ? false : true;
                const bool ignitionStatusWasOn = isIgnitionStatusOn.exchange(desiredIsIgnitionStatusOn);

                if (desiredIsIgnitionStatusOn != ignitionStatusWasOn)
                {
                    std::cout << "Ignition is now " << (isIgnitionStatusOn ? "ON" : "OFF") << std::endl;
                }
            }
        });

        constexpr auto simPeriod = 50ms;

        std::cout << title << std::endl;

        if (runSync)
        {
            std::cout << "Running with lifecycle and time synchronization ..." << std::endl;

            SilKit::Services::Orchestration::LifecycleConfiguration lifecycleConfiguration{};
            lifecycleConfiguration.operationMode = SilKit::Services::Orchestration::OperationMode::Coordinated;
            auto lifecycleService = participant->CreateLifecycleService(lifecycleConfiguration);
            auto timeSyncService = lifecycleService->CreateTimeSyncService();

            // state callbacks
            // Set an Init Handler
            lifecycleService->SetCommunicationReadyHandler([&]() {
                std::cout << "This participant is now initializing... " << std::endl;
                canController->Start();
            });

            // Set a Stop Handler
            lifecycleService->SetStopHandler([]() {
                std::cout << "This participant is now stopping ..." << std::endl;
            });

            // Set a Shutdown Handler
            lifecycleService->SetShutdownHandler([]() {
                std::cout << "This participant is now shutting down ..." << std::endl;
            });

            // participant behavior
            timeSyncService->SetSimulationStepHandler(
                [&](std::chrono::nanoseconds now, auto /*duration*/) {
                    if (isIgnitionStatusOn && (now % 100ms) == 0ms)
                    {
                        UpdateEngineValue();
                        SendEngineSpeed(canController, _currentEngineSpeed);
                    }
                },
                simPeriod);

            auto finalState = lifecycleService->StartLifecycle();
            std::cout << "Simulation stopped. Final State: " << finalState.get() << std::endl;
        }
        else
        {
            std::cout << "Running without lifecycle and time synchronization ..." << std::endl;

            canController->Start();

            std::atomic<bool> sendLoopRunning{true};

            auto senderThread = std::thread{[&isIgnitionStatusOn, &sendLoopRunning, canController, simPeriod, logger] {
                using clock = std::chrono::steady_clock;

                constexpr auto sendTick = 100ms;

                logger->Info("Starting loop to send current engine speed ...");

                clock::duration elapsedSend{};
                while (sendLoopRunning)
                {
                    elapsedSend += SleepAndReturnElapsed(simPeriod);

                    if (elapsedSend >= sendTick)
                    {
                        elapsedSend = {};

                        if (isIgnitionStatusOn)
                        {
                            UpdateEngineValue();
                            SendEngineSpeed(canController, _currentEngineSpeed);
                        }
                    }
                }

                logger->Info("Loop to send current engine speed finished");
            }};

            std::cout << "Press enter to stop the process..." << std::endl;
            std::cin.ignore();

            sendLoopRunning = false;
            senderThread.join();
        }
    }
    catch (const SilKit::ConfigurationError& error)
    {
        std::cerr << "Invalid configuration: " << error.what() << std::endl;
        std::cout << "Press enter to stop the process..." << std::endl;
        std::cin.ignore();
        return -2;
    }
    catch (const std::exception& error)
    {
        std::cerr << "Something went wrong: " << error.what() << std::endl;
        std::cout << "Press enter to stop the process..." << std::endl;
        std::cin.ignore();
        return -3;
    }
}
