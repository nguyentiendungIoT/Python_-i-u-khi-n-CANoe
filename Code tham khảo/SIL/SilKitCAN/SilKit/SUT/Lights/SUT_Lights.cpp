#include "silkit/SilKit.hpp"
#include "silkit/services/can/all.hpp"
#include "silkit/services/can/string_utils.hpp"
#include "silkit/services/logging/ILogger.hpp"
#include "silkit/services/orchestration/ILifecycleService.hpp"
#include "silkit/services/orchestration/string_utils.hpp"

using namespace std::chrono_literals;
using namespace SilKit::Services::Can;

namespace {

enum class State : uint8_t
{
    On,
    Off
};

const uint8_t _flashLightBit = 1 << 0;
const uint8_t _headLightBit = 1 << 1;

std::atomic<State> _currentFlashLightState{State::Off};
std::atomic<State> _currentHeadLightState{State::Off};

std::atomic<bool> _isFlashLightOn{false};

void UpdateLightState()
{
    if (_currentFlashLightState == State::On)
    {
        _isFlashLightOn = !_isFlashLightOn;
    }
}

void SendLightState(SilKit::Services::Can::ICanController* canController, State flashLightState, State headLightState)
{
    uint8_t lightValue = 0x0;
    if (_currentHeadLightState == State::On)
    {
        lightValue |= _headLightBit;
    }
    if (_currentFlashLightState == State::On)
    {
        if (_isFlashLightOn)
        {
            lightValue |= _flashLightBit;
        }
    }

    std::array<uint8_t, 1> payload{lightValue};

    CanFrame frame{};
    frame.canId = 0x321;
    frame.dlc = 1;
    frame.dataField = SilKit::Util::MakeSpan(payload);

    canController->SendFrame(frame);
}

using clock = std::chrono::steady_clock;
using clock_duration = typename clock::duration;

auto SleepAndReturnElapsed(clock_duration duration) -> clock_duration
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

     _     _       _     _
    | |   (_) __ _| |__ | |_ ___
    | |   | |/ _` | '_ \| __/ __|
    | |___| | (_| | | | | |_\__ \
    |_____|_|\__, |_| |_|\__|___/
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

        const auto config = SilKit::Config::ParticipantConfigurationFromString(participantConfigurationString);
        auto participant = SilKit::CreateParticipant(config, participantName, registryUri);
        const auto logger = participant->GetLogger();

        auto* canController = participant->CreateCanController("CAN_Engine", "easy");
        canController->AddFrameHandler([](auto controller, CanFrameEvent canFrameEvent) {
            if (canFrameEvent.frame.canId == 0x76)
            {
                // Received light control update

                const auto newFlashLightState =
                    (canFrameEvent.frame.dataField[0] & _flashLightBit) ? State::On : State::Off;
                const auto oldFlashLightState = _currentFlashLightState.exchange(newFlashLightState);

                const auto newHeadLightState =
                    (canFrameEvent.frame.dataField[0] & _headLightBit) ? State::On : State::Off;
                const auto oldHeadLightState = _currentHeadLightState.exchange(newHeadLightState);

                if (oldFlashLightState != newFlashLightState)
                {
                    std::cout << "Hazard flasher is now " << (newFlashLightState == State::On ? "ON" : "OFF")
                              << std::endl;
                }

                if (oldHeadLightState != newHeadLightState)
                {
                    std::cout << "Head light is now " << (newHeadLightState == State::On ? "ON" : "OFF") << std::endl;
                }
            }
        });

        constexpr auto simPeriod = 10ms;
        constexpr auto flashPeriod = 500ms;

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
                [flashPeriod, canController](auto now, auto /*duration*/) {
                    if ((now % flashPeriod) == 0ms)
                    {
                        UpdateLightState();
                    }

                    SendLightState(canController, _currentFlashLightState, _currentHeadLightState);
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

            auto sendThread = std::thread{[&sendLoopRunning, canController, simPeriod, logger, flashPeriod] {
                logger->Info("Starting loop to send current light state ...");

                clock_duration elapsedFlash{};
                while (sendLoopRunning)
                {
                    elapsedFlash += SleepAndReturnElapsed(simPeriod);

                    if (elapsedFlash >= flashPeriod)
                    {
                        elapsedFlash = {};

                        UpdateLightState();
                    }

                    SendLightState(canController, _currentFlashLightState, _currentHeadLightState);
                }

                logger->Info("Loop to send current light state finished");
            }};

            std::cout << "Press enter to stop the process..." << std::endl;
            std::cin.ignore();

            sendLoopRunning = false;
            sendThread.join();
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
