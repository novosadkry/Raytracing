#pragma once

#include "Walnut/Image.h"

#include <memory>
#include <vector>
#include <glm/fwd.hpp>

class Renderer
{
public:
    void Render();
    void OnResize(uint32_t width, uint32_t height);
    glm::vec4 PerPixel(glm::vec2 coord);

    std::shared_ptr<Walnut::Image> GetResultImage() const;

    float TimeElapsed() const
    {
        return m_TimeRenderElapsed;
    }

private:
    std::shared_ptr<Walnut::Image> m_Image;
    std::vector<uint32_t> m_ImageData;

    float m_TimeRenderElapsed;
};
