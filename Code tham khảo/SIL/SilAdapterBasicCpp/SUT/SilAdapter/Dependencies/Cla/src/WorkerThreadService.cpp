// Copyright (c) Vector Informatik GmbH. All rights reserved

#include <algorithm>

#include "WorkerThreadService.hpp"

#include <iostream>

using namespace std::chrono_literals;

WorkerThreadService::WorkerThreadService(std::shared_ptr<spdlog::logger> logger) : mLogger(std::move(logger))
{
}

WorkerThreadService::~WorkerThreadService()
{
  StopWorkerThread();
}

void WorkerThreadService::PushTask(const Task& task)
{
  std::lock_guard<std::mutex> lock(mTaskQueueMutex);

  if (mTaskQueue.empty())
  {
    mLogger->debug("Initial task was added to worker thread.");
    StopWorkerThread();

    mTaskQueue.push_back(task);
    mStopWorkerThread = false;
    mWorkerThread = std::thread([this]() { ExecuteTasks(); });
  }
  else
  {
    mLogger->debug("Task was added to already running worker thread.");
    mTaskQueue.push_back(task);
  }
}

void WorkerThreadService::StopWorkerThread()
{
  mLogger->debug("Stop worker thread was called.");
  if (mWorkerThread.joinable())
  {
    mStopWorkerThread = true;
    mWorkerThread.join();
    mLogger->debug("Worker thread successfully stopped.");
  }
}

void WorkerThreadService::ExecuteTasks()
{
  while (!mStopWorkerThread)
  {
    {
      mLogger->debug("Worker thread is still executing tasks.");

      std::lock_guard<std::mutex> lock(mTaskQueueMutex);
      mTaskQueue.erase(std::remove_if(mTaskQueue.begin(),
                         mTaskQueue.end(),
                         [](const Task& task) {
                           const bool taskWasSuccessful = task();
                           return taskWasSuccessful;
                         }),
        mTaskQueue.end());

      if (mTaskQueue.empty())
      {
        mStopWorkerThread = true;
        return;
      }
    }

    std::this_thread::sleep_for(1s);
  }

  mLogger->debug("Worker thread ends because of empty task list.");

  std::lock_guard<std::mutex> lock(mTaskQueueMutex);
  mTaskQueue.clear();
}
