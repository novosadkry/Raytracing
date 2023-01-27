#include "Renderer.h"
#include "Walnut/Timer.h"

#include <glm/vec2.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/geometric.hpp>

static uint32_t ConvertToRGBA(const glm::vec4& color)
{
    uint8_t r = (uint8_t)(color.r * 255.0f);
    uint8_t g = (uint8_t)(color.g * 255.0f);
    uint8_t b = (uint8_t)(color.b * 255.0f);
    uint8_t a = (uint8_t)(color.a * 255.0f);

    return a << 24 | b << 16 | g << 8 | r;
}

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
        glm::vec2 coord(
            x / (float)m_Image->GetWidth(),
            y / (float)m_Image->GetHeight()
        );

        coord = coord * 2.0f - 1.0f;

        glm::vec4 color = PerPixel(coord);
        uint32_t result = ConvertToRGBA(color);

        m_ImageData[x + y * m_Image->GetWidth()] = result;
    }

    m_Image->SetData(m_ImageData.data());
    m_TimeRenderElapsed = timer.Elapsed();
}

glm::vec4 Renderer::PerPixel(glm::vec2 coord)
{
    glm::vec3 rayOrigin(0, 0, 2);
    glm::vec3 rayDirection(coord.x, coord.y, -1);
    glm::vec3 sphereCenter(0, 0, 0);
    glm::vec3 lightDir = normalize(glm::vec3(-1, -1, -1));
    float sphereRadius = 1.0f;

    glm::vec3 oc = rayOrigin - sphereCenter;
    float a = dot(rayDirection, rayDirection);
    float b = 2 * dot(oc, rayDirection);
    float c = dot(oc, oc) - sphereRadius * sphereRadius;

    float d = b * b - 4 * a * c;

    if (d < 0.0f) 
    {
        float t = (coord.y + 1) * 0.5f;

        glm::vec3 bg = (1.0f - t) * glm::vec3(1.0, 1.0, 1.0) + 
                               t  * glm::vec3(0.5, 0.7, 1.0);

        return { bg, 1.0 };
    }

    float tNear = (-b - sqrt(d)) / (2.0f * a);
    glm::vec3 hit = rayOrigin + rayDirection * tNear;
    glm::vec3 hitNormal = normalize(hit - sphereCenter);

    float lightCos = dot(hitNormal, -lightDir);
    glm::vec3 sphereColor = 0.5f * hitNormal + 0.5f;

    return { sphereColor, glm::max(lightCos, 0.0f) };
}
