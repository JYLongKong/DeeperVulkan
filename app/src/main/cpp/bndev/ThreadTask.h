#ifndef VULKANEXBASE_THREADTASK_H
#define VULKANEXBASE_THREADTASK_H

class ThreadTask {
 public:
  ThreadTask();
  ~ThreadTask();
  void doTask();  // 执行Vulkan绘制相关任务的方法
};

#endif
