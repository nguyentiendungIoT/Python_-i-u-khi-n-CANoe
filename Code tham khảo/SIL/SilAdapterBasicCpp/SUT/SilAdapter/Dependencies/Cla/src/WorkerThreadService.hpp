// Copyright (c) Vector Informatik GmbH. All rights reserved.

#pragma once

#include <atomic>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

#include "SpdlogLibrary.hpp"

/*
 * Task that will be executed as long as it returns false.
 */
using Task = std::function<bool()>;

/*! \brief Class offering the execution of tasks in a worker thread. */
class WorkerThreadService
{
public:
  // Public methods
  // -------------------

  WorkerThreadService(std::shared_ptr<spdlog::logger> logger);
  WorkerThreadService(const WorkerThreadService&) = delete;
  WorkerThreadService(const WorkerThreadService&&) = delete;
  void operator=(const WorkerThreadService&) = delete;
  void operator=(const WorkerThreadService&&) = delete;
  ~WorkerThreadService();

  /*! \brief Adds a new task to perform until the task returns true.
   *  \param task The task to be executed. */
  void PushTask(const Task& task);

  /*! \brief Stops worker thread.*/
  void StopWorkerThread();

private:
  // Private members
  // -------------------

  std::thread mWorkerThread;
  std::vector<Task> mTaskQueue;
  std::atomic<bool> mStopWorkerThread{false};
  std::mutex mTaskQueueMutex;
  std::shared_ptr<spdlog::logger> mLogger;

  // Private methods
  // -------------------

  void ExecuteTasks();
};
