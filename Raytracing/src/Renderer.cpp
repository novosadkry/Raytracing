#include "Renderer.h"
#include "Walnut/Timer.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/geometric.hpp>

std::shared_ptr<Walnut::Image> Renderer::GetResultImage() const
{
    return m_Image;
}

void Renderer::OnResize(uint32_t width, uint32_t height)
{
    if (!m_Image)
    {
        m_Image = std::make_shared<Walnut::Image>(width, height, Walnut::ImageFormat::RGBA);
        m_ImageData = std::vector<uint32_t>(width * height);
    }

    if (width != m_Image->GetWidth() || height != m_Image->GetHeight())
    {
        m_Image->Resize(width, height);
        m_ImageData.resize(width * height);
    }
}

void Renderer::Render()
{
    Walnut::Timer timer;

    for (size_t y = 0; y < m_Image->GetHeight(); y++)
    for (size_t x = 0; x < m_Image->GetWidth(); x++)
    {
        const glm::vec2 coord = {
            x / (float)m_Image->GetWidth(),
            y / (float)m_Image->GetHeight()
        };

        auto color = PerPixel(coord);

        uint8_t r = (uint8_t)(color.r * 255.0f);
        uint8_t g = (uint8_t)(color.g * 255.0f);
        uint8_t b = (uint8_t)(color.b * 255.0f);

        auto result = 0xFF000000 | b << 16 | g << 8 | r;

        m_ImageData[x + y * m_Image->GetWidth()] = result;
    }

    m_Image->SetData(m_ImageData.data());
    m_TimeRenderElapsed = timer.Elapsed();
}

glm::vec3 Renderer::PerPixel(glm::vec2 coord)
{
    coord = coord * 2.0f - 1.0f;

    auto rayOrigin = glm::vec3(0, 0, 2.0f);
    auto rayDirection = glm::vec3(coord.x, coord.y, -1.0f);
    auto sphereCenter = glm::vec3(0, 0, -1.0f);
    auto sphereRadius = 1.0f;

    {
        auto oc = rayOrigin - sphereCenter;
        float a = length2(rayDirection);
        float b = 2 * dot(oc, rayDirection);
        float c = length2(oc) - sphereRadius * sphereRadius;

        float d = b * b - 4 * a * c;

        if (d >= 0)
        {
            float t = (-b - sqrt(d)) / (2.0f * a);
            auto n = normalize(rayOrigin + rayDirection * t - sphereCenter);
            return 0.5f * glm::vec3(n.x + 1, n.y + 1, n.z + 1);
        }
    }

    float t = (coord.y + 1) * 0.5f;
    return (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + t * glm::vec3(0.5, 0.7, 1.0);
}
