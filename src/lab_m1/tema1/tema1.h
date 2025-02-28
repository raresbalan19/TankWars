#pragma once

#include "components/simple_scene.h"
#include <vector>
#include <format>


namespace m1
{   
    struct Projectile {
        glm::vec2 position;  // Pozitia proiectilului
        glm::vec2 velocity;  // Viteza proiectilului
        float radius;        // Raza proiectilului
        bool active;         // Este proiectilul activ?
    };

    class tema1 : public gfxc::SimpleScene
    {
    public:
        tema1();
        ~tema1();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void CreateLiniutaMesh(const char* name, float x1, float y1, float x2, float y2, glm::vec3 color);
        void CreateRectangleConturMesh(const char* name, float width, float height, glm::vec3 color);
        void CreateCircleMesh(const char* name, float radius, int numTriangles, glm::vec3 color);
        void CreateRectangleMesh(const char* name, float width, float height, glm::vec3 color);
        void CreateTrapezoidMesh(const char* name, float baseWidth, float topWidth, float height, glm::vec3 color);
        void CreateTankComponents1();
        void CreateTankComponents2();

        void RenderTerrainSquare(float x1, float y1, float x2, float y2, glm::vec3 color);             
        void RenderHealthBar(float x, float y, float healthTank, float maxHealth);       
        void RenderTank1(float x, float y, float angle, float turretAngle);
        void RenderTank2(float x, float y, float angle, float turretAngle);
        void RenderTrajectory(float turretAngle, float angle, float tankX, float tankY);

        float GetTerrainHeightAt(float x);
        bool CheckCollisionWithTerrain(const glm::vec2& projectilePos, float threshold);
        bool CheckCircleCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2);
        void DeformTerrain(float explosionX, float explosionY, float radius);
        void SlideTerrain(float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

    protected:
        //tema1

        float terrainWidth;
        float pointSpacing;
        std::vector<float> heightMap;
        float gravity;
        float projectileSpeed;
        float tankRadius;
        float projectileRadius;

        float firstTankX;
        float firstTankY; 
        float firstTankAngle;
        float firstTankSpeed;
        float firstTankTurretAngle;
        float firstTankHealth;
        std::vector<Projectile> projectiles;

        float secondTankX;
        float secondTankY;
        float secondTankAngle;
        float secondTankSpeed;
        float secondTankTurretAngle;
        float secondTankHealth;
        std::vector<Projectile> projectilesSecondTank;

    };
}
