#pragma once

#include "components/simple_scene.h"
#include "components/text_renderer.h"

namespace m1
{
    class tema2 : public gfxc::SimpleScene
    {
    public:
        tema2();
        ~tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void GenerateCircleVerticesForCilindru(float y, float radius, int segments, glm::vec3 color, std::vector<VertexFormat>& vertices);
        void GenerateSideIndicesForCilindru(int segments, int offset, std::vector<unsigned int>& indices);
        void GenerateCircleIndicesForCilindru(int centerIndex, int offset, int segments, std::vector<unsigned int>& indices);
        void GenerateBaseVerticesForCon(float radius, int segments, glm::vec3 color, std::vector<VertexFormat>& vertices);
        void GenerateBaseIndicesForCon(int centerIndex, int segments, std::vector<unsigned int>& indices);
        void GenerateSideIndicesForCon(int segments, int tipIndex, std::vector<unsigned int>& indices);
        void Teren();
        void Copaci();
        void Drona(glm::vec3 position, float rotation);
        void Camera();
        bool CheckCollisionWithCilindru(glm::vec3 baseCilindru, glm::vec3 topCilindru, float radiusCilindru, glm::vec3 centerSfera, float radiusSfera);
        bool CheckCollisionWithCon(glm::vec3 baseCon, float heightCon, float baseRadiusCon, glm::vec3 centerSfera, float radiusSfera);


    protected:
        glm::vec3 positionDrona;
        float rotationDrona;
        float speed;
        float speedRotation;
        std::vector<glm::vec3> positionsCopaci;
        std::vector<float> scalesCopaci;
    };
}   // namespace m1
