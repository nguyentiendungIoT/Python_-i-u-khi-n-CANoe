/*
 * This file contains the application's entry point.
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <numeric>
#include <sstream>
#include <fstream>
#include <iomanip>

// ---------- INSTRUMENTATION ----------
#include "SilAdapter/SilAdapter.hpp"
// -------------------------------------

size_t getMsgSize()
{
    size_t msgSize = PerformanceMeasurement::PingPong.Pong.GetValue().size();

    if (msgSize < 1) {
        throw std::runtime_error("Invalid message size.");
    }

    return msgSize;
}

void processChunkData(const std::chrono::nanoseconds duration, const std::vector<std::chrono::nanoseconds>& measuredRoundtrips,
                      const uint32_t messageCount, uint32_t chunkCount, double& meanLatency, double& meanThroughput)
{
    // compute (averaged) latency [us] for given chunk
    std::vector<double> measuredLatency(measuredRoundtrips.size());
    std::transform(measuredRoundtrips.begin(), measuredRoundtrips.end(), measuredLatency.begin(),
        [](auto d) {
            return static_cast<double>(d.count() / 1e3 * 0.5); // roundtrip to latency (factor 1/2) and ns to us (factor 1e3)
        });
    const double latency = std::accumulate(measuredLatency.begin(), measuredLatency.end(), 0.0) / measuredLatency.size();

    // compute throughput [MiB/s] for given chunk
    const double durationSeconds = duration.count() / 1e9;
    const double msgSizeMiB = getMsgSize() / 1024.0 / 1024.0;    
    const double throughput = 2.0 * messageCount * msgSizeMiB / durationSeconds; // consider both directions (factor 2)

    // update mean via exponential smoothing
    if (chunkCount == 0) {
        meanLatency = latency;
        meanThroughput = throughput;
    }
    else {
        const double weight = 0.95;
        meanLatency = weight * meanLatency + (1.0 - weight) * latency;
        meanThroughput = weight * meanThroughput + (1.0 - weight) * throughput;
    }

    // transmit data to CANoe
    PerformanceMeasurement::PerformanceData data{};
    data.Latency = latency;
    data.LatencyMean = meanLatency;
    data.Throughput = throughput;
    data.ThroughputMean = meanThroughput;

    PerformanceMeasurement::Result.Performance.SetValue(data);
}

int main(int argc, char* argv[])
{
    Vector::CANoe::SilAdapter::Connect();

    std::cout << "Starting Ping-Pong between SUT and CANoe." << std::endl;

    std::vector<std::chrono::nanoseconds> measuredRoundtrips;
    std::chrono::steady_clock::time_point startTimestamp{};
    std::chrono::steady_clock::time_point sendTime{};

    const uint32_t chunkSize{ 50 }; // at most 60 DO changes per time unit allowed in CANoe

    uint32_t sendCount{ 0 };
    uint32_t chunkCount{ 0 };

    double meanLatency{ 0.0 };
    double meanThroughput{ 0.0 };

    // initial ping (42 is just a random choice, could be any other integer)    
    PerformanceMeasurement::PingPong.Ping.SetValue(std::vector<uint8_t>{42});

    // reset objects related to timing on CANoe restart
    PerformanceMeasurement::MeasurementState.IsMeasurementRunning.RegisterOnUpdateHandler(
        [&sendCount, &measuredRoundtrips, &chunkCount]() {
            if(PerformanceMeasurement::MeasurementState.IsMeasurementRunning.GetValue() == true) {
                sendCount = 0;
                measuredRoundtrips.clear();
                chunkCount = 0;
            }
        }
    );

    // ping-pong mechanism
    auto handler = PerformanceMeasurement::PingPong.Pong.RegisterOnUpdateHandler(
        [&]() {
            // time measurement
            if (sendCount == 0)
            {
                startTimestamp = std::chrono::high_resolution_clock::now(); // start runtime measurement (total duration)
            }
            else
            {
                measuredRoundtrips.push_back(std::chrono::high_resolution_clock::now() - sendTime);
            }
            sendTime = std::chrono::high_resolution_clock::now(); // start runtime measurement (current round trip)

            // send ping
            PerformanceMeasurement::PingPong.Ping.SetValue(PerformanceMeasurement::PingPong.Pong.GetValue());

            sendCount++;

            // finalize current chunk
            if (sendCount >= chunkSize + 1) // terminate after last ping has been sent
            {
                auto duration = std::chrono::high_resolution_clock::now() - startTimestamp;

                // compute latency/throughput and send data to CANoe
                processChunkData(duration, measuredRoundtrips, chunkSize, chunkCount, meanLatency, meanThroughput);

                // reset variables (prepare for new chunk)
                sendCount = 0;
                measuredRoundtrips.clear();
                chunkCount++;

                std::this_thread::sleep_for(std::chrono::milliseconds(10)); // DOs are not allowed to change at high frequency
            }            
        });

    std::cout << "Press enter to stop the process..." << std::endl;
    std::cin.ignore();

    std::cout << "Runtime measurement terminated." << std::endl;

    Vector::CANoe::SilAdapter::Disconnect();

    return 0;
}