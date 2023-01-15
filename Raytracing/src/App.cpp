#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"
#include "Walnut/Image.h"
#include "Renderer.h"

using namespace Walnut;

class ViewportLayer : public Layer
{
public:
    void OnUIRender() override
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::Begin("Viewport");

        const uint32_t viewportWidth = ImGui::GetContentRegionAvail().x;
        const uint32_t viewportHeight = ImGui::GetContentRegionAvail().y;

        if (const auto result = m_Renderer.GetResultImage())
        {
            ImGui::Image(
                result->GetDescriptorSet(),
                ImVec2 {
                    static_cast<float>(result->GetWidth()),
                    static_cast<float>(result->GetHeight())
                },
                ImVec2(0, 1), ImVec2(1, 0)
            );
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::Begin("Render Info");

        if (ImGui::Button("Render"))
        {
            m_Renderer.OnResize(viewportWidth, viewportHeight);
            m_Renderer.Render();
        }

        ImGui::Text("Last render: %.3f ms", m_Renderer.TimeElapsed());

        ImGui::End();
    }

private:
    Renderer m_Renderer;
};

Application* Walnut::CreateApplication(int argc, char** argv)
{
    ApplicationSpecification spec;
    spec.Name = "Raytracing";

    auto* app = new Application(spec);
    app->PushLayer<ViewportLayer>();

    return app;
}