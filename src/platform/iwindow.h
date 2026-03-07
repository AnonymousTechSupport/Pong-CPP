// --- Window interface
// ----------------------------------------------------

#pragma once

class IWindow
{
  public:
    virtual ~IWindow() = default;

    // Lifecycle
    virtual bool Create() = 0;
    virtual void Shutdown() = 0;

    // Event handling
    virtual bool ProcessEvent() = 0;

    // State
    virtual bool IsRunning() const = 0;
    virtual int GetWidth() const = 0;
    virtual int GetHeight() const = 0;

    // Native handle (opaque)
    virtual void* GetNativeWindow() const = 0;
};
