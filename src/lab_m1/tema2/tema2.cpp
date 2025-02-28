#include "lab_m1/tema2/tema2.h"
#include "lab_m1/tema2/transform3D.h"
#include <vector>
#include <string>
#include <iostream>


using namespace std;
using namespace m1;

tema2::tema2() {}

tema2::~tema2() {}

void tema2::Init()
{
    {
        int numRows = 100.0f;
        int numCols = 100.0f;
        float stepSize = 1.0f;
        glm::vec3 vertexColor = glm::vec3(0.5f, 0.5f, 0.5f);
        std::vector<VertexFormat> meshVertices;
        std::vector<unsigned int> meshIndices;

        for (int row = 0; row <= numRows; row++) {
            for (int col = 0; col <= numCols; col++) {
                glm::vec3 position(col * stepSize, 0.0f, row * stepSize);
                meshVertices.emplace_back(position, vertexColor);
            }
        }

        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                unsigned int idxTopLeft = row * (numCols + 1) + col;
                unsigned int idxTopRight = idxTopLeft + 1;
                unsigned int idxBottomLeft = (row + 1) * (numCols + 1) + col;
                unsigned int idxBottomRight = idxBottomLeft + 1;

                meshIndices.insert(meshIndices.end(), { idxTopLeft, idxBottomLeft, idxTopRight });
                meshIndices.insert(meshIndices.end(), { idxTopRight, idxBottomLeft, idxBottomRight });
            }
        }

        Mesh* terenMesh = new Mesh("teren");
        terenMesh->InitFromData(meshVertices, meshIndices);
        meshes["teren"] = terenMesh;
    }

    float sideCub = 0.3f;

    {
        float bodyLength = 2.0f;
        float bodyWidth = sideCub;
        float bodyHeight = sideCub;

        glm::vec3 centerParalelipiped = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 dimensionsParalelipiped = glm::vec3(bodyLength, bodyHeight, bodyWidth);

        float xMin = centerParalelipiped.x - dimensionsParalelipiped.x / 2.0f;
        float xMax = centerParalelipiped.x + dimensionsParalelipiped.x / 2.0f;
        float yMin = centerParalelipiped.y - dimensionsParalelipiped.y / 2.0f;
        float yMax = centerParalelipiped.y + dimensionsParalelipiped.y / 2.0f;
        float zMin = centerParalelipiped.z - dimensionsParalelipiped.z / 2.0f;
        float zMax = centerParalelipiped.z + dimensionsParalelipiped.z / 2.0f;

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(xMin, yMin, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMin, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMax, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMax, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMin, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMin, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMax, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMax, zMax), glm::vec3(0.5f, 0.5f, 0.5f))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0,
            0, 3, 7, 7, 4, 0,
            1, 2, 6, 6, 5, 1
        };

        Mesh* paralelipipedMesh = new Mesh("paralelipiped");
        paralelipipedMesh->InitFromData(vertices, indices);
        meshes["paralelipiped"] = paralelipipedMesh;
    }

    {
        glm::vec3 centerCub = glm::vec3(0.0f, 0.0f, 0.0f);

        float xMin = centerCub.x - sideCub / 2.0f;
        float xMax = centerCub.x + sideCub / 2.0f;
        float yMin = centerCub.y - sideCub / 2.0f;
        float yMax = centerCub.y + sideCub / 2.0f;
        float zMin = centerCub.z - sideCub / 2.0f;
        float zMax = centerCub.z + sideCub / 2.0f;

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(xMin, yMin, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMin, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMax, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMax, zMin), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMin, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMin, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMax, yMax, zMax), glm::vec3(0.5f, 0.5f, 0.5f)),
            VertexFormat(glm::vec3(xMin, yMax, zMax), glm::vec3(0.5f, 0.5f, 0.5f))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0,
            0, 3, 7, 7, 4, 0,
            1, 2, 6, 6, 5, 1
        };

        Mesh* cubMesh = new Mesh("cub");
        cubMesh->InitFromData(vertices, indices);
        meshes["cub"] = cubMesh;
    }

    {
        glm::vec3 centerElice = glm::vec3(0.0f, 0.0f, 0.0f);
        glm::vec3 dimensionsElice = glm::vec3(1.0f, 0.1f, 0.1f);

        float xMin = centerElice.x - dimensionsElice.x / 2.0f;
        float xMax = centerElice.x + dimensionsElice.x / 2.0f;
        float yMin = centerElice.y - dimensionsElice.y / 2.0f;
        float yMax = centerElice.y + dimensionsElice.y / 2.0f;
        float zMin = centerElice.z - dimensionsElice.z / 2.0f;
        float zMax = centerElice.z + dimensionsElice.z / 2.0f;

        std::vector<VertexFormat> vertices = {
            VertexFormat(glm::vec3(xMin, yMin, zMin), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMax, yMin, zMin), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMax, yMax, zMin), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMin, yMax, zMin), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMin, yMin, zMax), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMax, yMin, zMax), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMax, yMax, zMax), glm::vec3(0.0f, 0.0f, 0.0f)),
            VertexFormat(glm::vec3(xMin, yMax, zMax), glm::vec3(0.0f, 0.0f, 0.0f))
        };

        std::vector<unsigned int> indices = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            3, 2, 6, 6, 7, 3,
            0, 1, 5, 5, 4, 0,
            0, 3, 7, 7, 4, 0,
            1, 2, 6, 6, 5, 1
        };

        Mesh* eliceMesh = new Mesh("paralelipipedElice");
        eliceMesh->InitFromData(vertices, indices);
        meshes["paralelipipedElice"] = eliceMesh;
    }

    {
        int segmentsCilindru = 20;
        float radiusCilindru = 0.1f;
        float heightCilindru = 1.0f;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        GenerateCircleVerticesForCilindru(0.0f, radiusCilindru, segmentsCilindru, glm::vec3(0.6f, 0.3f, 0.0f), vertices);
        GenerateCircleVerticesForCilindru(heightCilindru, radiusCilindru, segmentsCilindru, glm::vec3(0.6f, 0.3f, 0.0f), vertices);

        int bottomCenter = vertices.size();
        vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.6f, 0.3f, 0.0f));

        int topCenter = vertices.size();
        vertices.emplace_back(glm::vec3(0.0f, heightCilindru, 0.0f), glm::vec3(0.6f, 0.3f, 0.0f));

        GenerateSideIndicesForCilindru(segmentsCilindru, 0, indices);
        GenerateCircleIndicesForCilindru(bottomCenter, 0, segmentsCilindru, indices);
        GenerateCircleIndicesForCilindru(topCenter, segmentsCilindru + 1, segmentsCilindru, indices);

        Mesh* cilindruMesh = new Mesh("cilindru");
        cilindruMesh->InitFromData(vertices, indices);
        meshes["cilindru"] = cilindruMesh;
    }

    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        GenerateBaseVerticesForCon(0.5f, 20.0f, glm::vec3(0.0f, 0.6f, 0.0f), vertices);

        int tipIndex = vertices.size();
        vertices.emplace_back(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.6f, 0.0f));

        int centerIndex = vertices.size();
        vertices.emplace_back(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.6f, 0.0f));

        GenerateSideIndicesForCon(20.0f, tipIndex, indices);
        GenerateBaseIndicesForCon(centerIndex, 20.0f, indices);

        Mesh* conMesh = new Mesh("con");
        conMesh->InitFromData(vertices, indices);
        meshes["con"] = conMesh;
    }

    float x1Teren = 0.0f;
    float x2Teren = 100.0f;
    float z1Teren = 0.0f;
    float z2Teren = 100.0f;

    for (int i = 0; i < 100; i++) {
        float x = x1Teren + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (x2Teren - x1Teren)));
        float z = z1Teren + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (z2Teren - z1Teren)));
        float scale = 1.5f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX));
        positionsCopaci.push_back(glm::vec3(x, 1.0f, z));
        scalesCopaci.push_back(scale);
    }

    positionDrona = glm::vec3(0.0f, 10.0f, 0.0f);
    speed = 15.0f;
    rotationDrona = 0.0f;
    speedRotation = glm::radians(60.0f);

    auto camera = GetSceneCamera();
    camera->SetPerspective(80.0f, window->props.aspectRatio, 0.05f, 200.0f);
    camera->SetPosition(glm::vec3(0.0f, 10.0f, 50.0f));
    camera->Update();
    GetCameraInput()->SetActive(false);

    Shader* shader = new Shader("TerenShader");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;
}

void tema2::GenerateCircleVerticesForCilindru(float y, float radius, int segments, glm::vec3 color, std::vector<VertexFormat>& vertices) 
{
    for (int i = 0; i <= segments; i++) {
        float alfa = i * 2 * glm::pi<float>() / segments;
        float x = radius * cos(alfa);
        float z = radius * sin(alfa);
        vertices.emplace_back(glm::vec3(x, y, z), color);
    }
}

void tema2::GenerateSideIndicesForCilindru(int segments, int offset, std::vector<unsigned int>& indices) 
{
    for (int i = 0; i < segments; i++) {
        int bottom1 = offset + i;
        int bottom2 = offset + (i + 1) % segments;
        int top1 = offset + i + segments + 1;
        int top2 = offset + (i + 1) % segments + segments + 1;

        indices.push_back(bottom1);
        indices.push_back(bottom2);
        indices.push_back(top1);

        indices.push_back(top1);
        indices.push_back(bottom2);
        indices.push_back(top2);
    }
}

void tema2::GenerateCircleIndicesForCilindru(int centerIndex, int offset, int segments, std::vector<unsigned int>& indices) 
{
    for (int i = 0; i < segments; i++) {
        int v1 = offset + i;
        int v2 = offset + (i + 1) % segments;

        indices.push_back(centerIndex);
        indices.push_back(v1);
        indices.push_back(v2);
    }
}

void tema2::GenerateBaseVerticesForCon(float radius, int segments, glm::vec3 color, std::vector<VertexFormat>& vertices) 
{
    for (int i = 0; i < segments; i++) {
        float angle = i * 2 * glm::pi<float>() / segments;
        float x = radius * cos(angle);
        float z = radius * sin(angle);
        vertices.emplace_back(glm::vec3(x, 0, z), color);
    }
}

void tema2::GenerateBaseIndicesForCon(int centerIndex, int segments, std::vector<unsigned int>& indices) 
{
    for (int i = 0; i < segments; i++) {
        int nextIndex = (i + 1) % segments;
        indices.push_back(centerIndex);
        indices.push_back(i);
        indices.push_back(nextIndex);
    }
}

void tema2::GenerateSideIndicesForCon(int segments, int tipIndex, std::vector<unsigned int>& indices) 
{
    for (int i = 0; i < segments; i++) {
        int nextIndex = (i + 1) % segments;
        indices.push_back(i);
        indices.push_back(nextIndex);
        indices.push_back(tipIndex);
    }
}

void tema2::Update(float deltaTimeSeconds)
{
    Camera();
    Teren();
    Copaci();
    Drona(positionDrona, rotationDrona);
}

void tema2::Teren()
{
    Shader* shader = shaders["TerenShader"];
    shader->Use();

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = transform3D::Translate(-100, 0, -100);
    modelMatrix *= transform3D::Scale(5, 1, 5);

    glUniformMatrix4fv(glGetUniformLocation(shader->program, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    RenderMesh(meshes["teren"], shader, modelMatrix);
}

void tema2::Copaci() 
{
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    for (int i = 0; i < positionsCopaci.size(); i++) {
        glm::vec3 position = positionsCopaci[i];
        float scale = scalesCopaci[i];

        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y, position.z);
        modelMatrix *= transform3D::Scale(scale, scale, scale);
        RenderMesh(meshes["cilindru"], shader, modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + 1.0f * scale, position.z);
        modelMatrix *= transform3D::Scale(1.0f * scale, 1.5f * scale, 1.0f * scale);
        RenderMesh(meshes["con"], shader, modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix = transform3D::Translate(position.x, position.y + 2.0f * scale, position.z);
        modelMatrix *= transform3D::Scale(0.5f * scale, 1.5f * scale, 0.5f * scale);
        RenderMesh(meshes["con"], shader, modelMatrix);
    }
}

void tema2::Drona(glm::vec3 position, float rotation)
{
    Shader* shader = shaders["VertexColor"];
    shader->Use();

    float angle = Engine::GetElapsedTime() * 5.0f;

    glm::mat4 baseTransform = glm::mat4(1);
    baseTransform = transform3D::Translate(position.x, position.y, position.z);
    baseTransform *= transform3D::RotateOY(rotation);
    baseTransform *= transform3D::RotateOY(glm::radians(45.0f));

    glm::mat4 modelMatrix = baseTransform;
    modelMatrix *= transform3D::Scale(1.15f, 0.3f, 1.0f);
    RenderMesh(meshes["paralelipiped"], shader, modelMatrix);

    modelMatrix = baseTransform;
    modelMatrix *= transform3D::RotateOY(glm::radians(90.0f));
    modelMatrix *= transform3D::Scale(1.15f, 0.3f, 1.0f);
    RenderMesh(meshes["paralelipiped"], shader, modelMatrix);

    modelMatrix = baseTransform;
    modelMatrix *= transform3D::Translate(1.0f, 0.2f, 0.0f);
    RenderMesh(meshes["cub"], shader, modelMatrix);

    modelMatrix *= transform3D::Translate(0.0f, 0.2f, 0.0f);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);
    RenderMesh(meshes["paralelipipedElice"], shader, modelMatrix);

    modelMatrix = baseTransform;
    modelMatrix *= transform3D::Translate(-1, 0.2f, 0.0f);
    RenderMesh(meshes["cub"], shader, modelMatrix);

    modelMatrix *= transform3D::Translate(0.0f, 0.2f, 0.0f);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);
    RenderMesh(meshes["paralelipipedElice"], shader, modelMatrix);

    modelMatrix = baseTransform;
    modelMatrix *= transform3D::Translate(0.0f, 0.2f, 1.0f);
    RenderMesh(meshes["cub"], shader, modelMatrix);

    modelMatrix *= transform3D::Translate(0.0f, 0.2f, 0.0f);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);
    RenderMesh(meshes["paralelipipedElice"], shader, modelMatrix);

    modelMatrix = baseTransform;
    modelMatrix *= transform3D::Translate(0.0f, 0.2f, -1.0f);
    RenderMesh(meshes["cub"], shader, modelMatrix);

    modelMatrix *= transform3D::Translate(0.0f, 0.2f, 0.0f);
    modelMatrix *= transform3D::RotateOY(angle);
    modelMatrix *= transform3D::Scale(1.0f, 1.0f, 1.0f);
    RenderMesh(meshes["paralelipipedElice"], shader, modelMatrix);
}

void tema2::FrameStart()
{
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema2::Camera()
{
    glm::vec3 offsetFromDrona(0, 3, -2);
    glm::vec3 forwardDrona(sin(rotationDrona), 0.0f, cos(rotationDrona));
    glm::vec3 cameraPosition = positionDrona + forwardDrona * glm::length(glm::vec2(offsetFromDrona.z, offsetFromDrona.y));
    cameraPosition.y = cameraPosition.y + 2;

    auto activeCamera = GetSceneCamera();
    activeCamera->SetPosition(cameraPosition);

    glm::vec3 directionToTarget = glm::normalize(positionDrona - cameraPosition);
    activeCamera->SetRotation(glm::vec3(
        atan2(directionToTarget.y, glm::length(glm::vec2(directionToTarget.x, directionToTarget.z))), 
        -atan2(directionToTarget.x, -directionToTarget.z), 0.0f));
    activeCamera->Update();
}

void tema2::FrameEnd() {}

bool tema2::CheckCollisionWithCilindru(glm::vec3 baseCilindru, glm::vec3 topCilindru, float radiusCilindru, glm::vec3 centerSfera, float radiusSfera) 
{
    glm::vec3 axisCilindru = topCilindru - baseCilindru;
    float axisLength = glm::length(axisCilindru);
    axisCilindru = glm::normalize(axisCilindru);

    glm::vec3 baseToSfera = centerSfera - baseCilindru;
    float projectionLength = glm::dot(baseToSfera, axisCilindru);
    if (projectionLength < 0 || projectionLength > axisLength) return false;

    glm::vec3 closestPoint = baseCilindru + projectionLength * axisCilindru;
    float distance = glm::length(centerSfera - closestPoint);
    return distance < (radiusSfera + radiusCilindru);
}

bool tema2::CheckCollisionWithCon(glm::vec3 baseCon, float heightCon, float baseRadiusCon, glm::vec3 centerSfera, float radiusSfera) 
{
    glm::vec3 tipCon = baseCon + glm::vec3(0.0f, heightCon, 0.0f);
    glm::vec3 baseToSfera = centerSfera - baseCon;
    glm::vec3 baseToTip = tipCon - baseCon;

    float heightSquared = glm::length2(baseToTip);
    float projection = glm::dot(baseToSfera, baseToTip) / heightSquared;
    if (projection < 0 || projection > 1) return false;

    glm::vec3 closestPoint = baseCon + projection * baseToTip;
    float baseRadiusAtProjection = baseRadiusCon * (1.0f - projection);
    float distance = glm::length(centerSfera - closestPoint);
    return distance < (radiusSfera + baseRadiusAtProjection);
}

void tema2::OnInputUpdate(float deltaTime, int mods) 
{

    glm::vec3 forward = glm::vec3(sin(rotationDrona), 0.0f, cos(rotationDrona));
    glm::vec3 right = glm::vec3(cos(rotationDrona), 0.0f, -sin(rotationDrona));
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

    glm::vec3 newPosition = positionDrona;
    float radiusDrona = 1.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        newPosition -= forward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        newPosition += forward * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        newPosition -= right * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        newPosition += right * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        newPosition += up * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        newPosition -= up * speed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        rotationDrona += speedRotation * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        rotationDrona -= speedRotation * deltaTime;
    }

    bool move = true;

    for (int i = 0; i < positionsCopaci.size(); i++) {
        float scale = scalesCopaci[i];

        glm::vec3 baseCilindru = positionsCopaci[i];
        glm::vec3 topCilindru = baseCilindru + glm::vec3(0.0f, 1.2f * scale, 0.0f);
        float radiusCilindru = 0.1f * scale;

        if (CheckCollisionWithCilindru(baseCilindru, topCilindru, radiusCilindru, newPosition, radiusDrona)) {
            move = false;
        }

        glm::vec3 baseCon = topCilindru;
        float heightCon = 1.0f * scale;
        float baseRadiusCon = 0.5f * scale;

        if (CheckCollisionWithCon(baseCon, heightCon, baseRadiusCon, newPosition, radiusDrona)) {
            move = false;
        }

        glm::vec3 secondBaseCon = topCilindru + glm::vec3(0.0f, heightCon, 0.0f);
        float secondHeightCon = 1.5f * scale;
        float secondBaseRadiusCon = 0.5f * scale * 0.5f;

        if (CheckCollisionWithCon(secondBaseCon, secondHeightCon, secondBaseRadiusCon, newPosition, radiusDrona)) {
            move = false;
        }
    }

    if (newPosition.y < 2.0f) {
        move = false;
    }

    if (move) {
        positionDrona = newPosition;
    }
}

void tema2::OnKeyPress(int key, int mods) {}

void tema2::OnKeyRelease(int key, int mods) {}

void tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {}

void tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void tema2::OnWindowResize(int width, int height) {}
