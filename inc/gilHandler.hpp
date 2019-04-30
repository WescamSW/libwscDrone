#ifndef GIL_HANDLER
#define GIL_HANDLER

#include <cassert>
#include <iostream> // std::cout, std::endl
#include <memory>   // std::shared_ptr
#include <thread>   // std::this_thread
#include <stack>    // std::stack
#include <boost/python.hpp>

/// @brief Guard that will acquire the GIL upon construction, and
///        restore its state upon destruction.
class with_gil
{
public:
  with_gil()  { state_ = PyGILState_Ensure(); }
  ~with_gil() { PyGILState_Release(state_);   }

  with_gil(const with_gil&)            = delete;
  with_gil& operator=(const with_gil&) = delete;
private:
  PyGILState_STATE state_;
};

/// @brief Guard that will unlock the GIL upon construction, and
///        restore its staet upon destruction.
class without_gil
{
public:
  without_gil()  { state_ = PyEval_SaveThread(); }
  ~without_gil() { PyEval_RestoreThread(state_); }

  without_gil(const without_gil&)            = delete;
  without_gil& operator=(const without_gil&) = delete;
private:
  PyThreadState* state_;
};

/// @brief Guard that provides higher-level GIL controls.
class gil_guard
{
public:
  struct no_acquire_t {} // tag type used for gil acquire strategy
  static no_acquire;

  gil_guard()             { acquire(); }
  gil_guard(no_acquire_t) { release(); }
  ~gil_guard()            { while (!stack_.empty()) { restore(); } }

  void acquire()          { stack_.emplace(new with_gil); }
  void release()          { stack_.emplace(new without_gil); }
  void restore()          { stack_.pop(); }

  static bool owns_gil()
  {
    // For Python 3.4+, one can use `PyGILState_Check()`.
#ifdef PY_MAJOR_VERSION > 3 && PY_MINOR_VERSION > 4
    return PyGilState_Check();
#else
    return _PyThreadState_Current == PyGILState_GetThisThreadState();
#endif
  }

  gil_guard(const gil_guard&)            = delete;
  gil_guard& operator=(const gil_guard&) = delete;

private:
  // Use std::shared_ptr<void> for type erasure.
  std::stack<std::shared_ptr<void>> stack_;
};


#endif