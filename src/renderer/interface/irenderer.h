// --- Renderer interface
// --------------------------------------------------
#pragma once
#include "renderer/interface/render_types.h"

class IRenderer
{
  public:
    virtual ~IRenderer() = default;

    // Lifecycle
    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    // Present the current frame (after all queueing is done).
    virtual void RenderFrame() = 0;

    // Queue draw commands
    virtual void QueueRenderRectangle(const RenderRectangle& rect) = 0;
    virtual void QueueRenderBall(const RenderBall& cmd) = 0;
};
