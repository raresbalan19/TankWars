#include "lab_m1/tema1/tema1.h"
#include <vector>
#include <iostream>
#include "lab_m1/lab3/transform2D.h"
#include "lab_m1/lab3/object2D.h"


using namespace std;
using namespace m1;


/*
 *  To find out more about FrameStart, Update, FrameEnd
 *  and the order in which they are called, see world.cpp.
 */


tema1::tema1()
{
}


tema1::~tema1()
{
}


void tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    terrainWidth = resolution.x;
    int numPoints = resolution.x / 10;
    pointSpacing = 1;

    float amplitude1 = 60.0f, frequency1 = 0.01f;
    float amplitude2 = 40.0f, frequency2 = 0.02f;

    firstTankHealth = 100.0f;
    firstTankX = 20;
    firstTankSpeed = 100;
    firstTankTurretAngle = 0.0f;

    secondTankHealth = 100.0f;
    secondTankX = 700;
    secondTankSpeed = 100;
    secondTankTurretAngle = 0.0f;

    gravity = 200.0f;
    projectileSpeed = 300.0f;
    tankRadius = 30.0f;
    projectileRadius = 20.0f;

    for (float x = 0; x < terrainWidth; x += pointSpacing) {
        float height = amplitude1 * sin(frequency1 * x) + amplitude2 * sin(frequency2 * x) + 250; // Calculez intaltimea
        heightMap.push_back(height);  // Salvez inaltimea in vectorul heightMap
    }

    CreateTankComponents1();
    CreateTankComponents2();
    CreateRectangleConturMesh("healthBarFrame", 50.0f, 8.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    CreateRectangleMesh("healthBarFill", 50.0f, 8.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    CreateCircleMesh("projectile", 5, 10, glm::vec3(1.0f, 0.0f, 0.0f));  // Cerc roșu

    //CreateTrapezoidMesh("trapezBazaInferioara", 50, 40, 10, glm::vec3(0.42f, 0.31f, 0.24f));
    //CreateTrapezoidMesh("trapezBazaSuperioara", 70, 60, 15, glm::vec3(0.42f, 0.31f, 0.24f));
    //CreateCircleMesh("cercDeasupraTrapezelor", 20, 50, glm::vec3(0.82f, 0.71f, 0.55f));
    //CreateRectangleMesh("teava", 40, 8, glm::vec3(0.29f, 0.29f, 0.29f));
    //CreateCircleMesh("proiectil", 5, 10, glm::vec3(1.0f, 0.0f, 0.0f));
    //CreateCircleMesh("trajectory_point", 3, 10, glm::vec3(1.0f, 1.0f, 0.0f));
}


void tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.53f, 0.81f, 0.98f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void tema1::Update(float deltaTimeSeconds)
{
    float turretSpeed = 1.0f;

    for (int i = 0; i <= heightMap.size(); i++) {
        float x1, x2, y1, y2;

        if (i < heightMap.size() - 1) {
            // Coordonatele punctelor unui patrat
            x1 = i * pointSpacing;
            y1 = heightMap[i];
            x2 = (i + 1) * pointSpacing;
            y2 = heightMap[i + 1];
        }
        else {
            // Coordonatele punctelor patratului care se afla la marginea din dreapta a hartii
            float amplitude1 = 60.0f, frequency1 = 0.01f;
            float amplitude2 = 40.0f, frequency2 = 0.02f;
            x1 = (heightMap.size() - 1) * pointSpacing;
            y1 = heightMap.back();
            x2 = heightMap.size() * pointSpacing;
            y2 = amplitude1 * sin(frequency1 * x2) + amplitude2 * sin(frequency2 * x2) + 250;
        }

        glm::vec3 color(0.0f, 0.5f, 0.0f);
        RenderTerrainSquare(x1, y1, x2, y2, color);
    }

    SlideTerrain(deltaTimeSeconds);

    if (window->KeyHold(GLFW_KEY_A)) {
        firstTankX -= firstTankSpeed * deltaTimeSeconds;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        firstTankX += firstTankSpeed * deltaTimeSeconds;
    }

    if (window->KeyHold(GLFW_KEY_W)) {
        if (firstTankTurretAngle < M_PI_4 + 0.4) {
            firstTankTurretAngle += turretSpeed * deltaTimeSeconds;
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        if (firstTankTurretAngle > -M_PI_4 - 0.4) {
            firstTankTurretAngle -= turretSpeed * deltaTimeSeconds;
        }
    }

    // Gasesc punctele A si B cum e pe ocw la: Plasare/orientare tanc pe teren
    int index = firstTankX / pointSpacing;
    if (index < 0) index = 0;
    if (index >= heightMap.size() - 1) index = heightMap.size() - 2;

    float xA = index * pointSpacing;
    float yA = heightMap[index];
    float xB = (index + 1) * pointSpacing;
    float yB = heightMap[index + 1];

    float t = (firstTankX - xA) / (xB - xA);
    firstTankY = yA + t * (yB - yA);

    float vX = xB - xA;
    float vY = yB - yA;
    firstTankAngle = atan2(vY, vX);

    for (auto& projectile : projectiles) {
        if (projectile.active == true) {
            // Actualizez viteza cu gravitatia => Lansare Proiectil(formule)
            projectile.velocity.y -= gravity * deltaTimeSeconds;

            // Actualizez pozitia proiectilului
            projectile.position.x += projectile.velocity.x * deltaTimeSeconds;
            projectile.position.y += projectile.velocity.y * deltaTimeSeconds;

            if (CheckCollisionWithTerrain(projectile.position, 5.0f)) { // Pragul este 5.0f
                projectile.active = false; // Dezactivez proiectilul
                DeformTerrain(projectile.position.x, projectile.position.y, 60.0f); // Deformez terenul cu 60
            }

            glm::vec2 secondTankCenter(secondTankX, secondTankY);
            if (CheckCircleCollision(projectile.position, projectileRadius, secondTankCenter, tankRadius)) {
                secondTankHealth -= 25.0f;
                projectile.active = false;
            }

            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(projectile.position.x, projectile.position.y);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);


        }
    }

    if (firstTankHealth > 0) {
        RenderTank1(firstTankX, firstTankY, firstTankAngle, firstTankTurretAngle);
        RenderHealthBar(firstTankX, firstTankY, firstTankHealth, 100.0f);
    }

    if (window->KeyHold(GLFW_KEY_LEFT)) {
        secondTankX -= secondTankSpeed * deltaTimeSeconds;
    }

    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        secondTankX += secondTankSpeed * deltaTimeSeconds;
    }

    if (window->KeyHold(GLFW_KEY_UP)) {
        if (secondTankTurretAngle < M_PI_4 + 0.4) {
            secondTankTurretAngle += turretSpeed * deltaTimeSeconds;
        }
    }

    if (window->KeyHold(GLFW_KEY_DOWN)) {
        if (secondTankTurretAngle > -M_PI_4 - 0.4) {
            secondTankTurretAngle -= turretSpeed * deltaTimeSeconds;
        }
    }

    int indexSecondTank = secondTankX / pointSpacing;
    if (indexSecondTank < 0) indexSecondTank = 0;
    if (indexSecondTank >= heightMap.size() - 1) indexSecondTank = heightMap.size() - 2;

    float secondTankxA = indexSecondTank * pointSpacing;
    float secondTankyA = heightMap[indexSecondTank];
    float secondTankxB = (indexSecondTank + 1) * pointSpacing;
    float secondTankyB = heightMap[indexSecondTank + 1];

    float secondTankt = (secondTankX - secondTankxA) / (secondTankxB - secondTankxA);
    secondTankY = secondTankyA + secondTankt * (secondTankyB - secondTankyA);

    float secondTankvX = secondTankxB - secondTankxA;
    float secondTankvY = secondTankyB - secondTankyA;
    secondTankAngle = atan2(secondTankvY, secondTankvX);

    for (auto& projectile : projectilesSecondTank) {
        if (projectile.active == true) {

            projectile.velocity.y -= gravity * deltaTimeSeconds;
            projectile.position.x += projectile.velocity.x * deltaTimeSeconds;
            projectile.position.y += projectile.velocity.y * deltaTimeSeconds;

            if (CheckCollisionWithTerrain(projectile.position, 5.0f)) {
                projectile.active = false;
                DeformTerrain(projectile.position.x, projectile.position.y, 60.0f);
            }

            glm::vec2 tankCenter(firstTankX, firstTankY);
            if (CheckCircleCollision(projectile.position, projectileRadius, tankCenter, tankRadius)) {
                firstTankHealth -= 25.0f;
                projectile.active = false;
            }

            glm::mat3 modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(projectile.position.x, projectile.position.y);
            RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);


        }
    }

    if (secondTankHealth > 0) {
        RenderTank2(secondTankX, secondTankY, secondTankAngle, secondTankTurretAngle);
        RenderHealthBar(secondTankX, secondTankY, secondTankHealth, 100.0f);
    }

    if (firstTankHealth > 0) {
        RenderTrajectory(firstTankTurretAngle, firstTankAngle, firstTankX, firstTankY);
    }

    if (secondTankHealth > 0) {
        RenderTrajectory(secondTankTurretAngle, secondTankAngle, secondTankX, secondTankY);
    }

    //glm::mat3 modelMatrixExplicatie1 = glm::mat3(1);
    ///* il translatez la pozitia(35, 0) ca sa vad tot tancul caci baza superioara a tancului are
    // baza mica de jos 70 deci ca sa il pot vedea pe tot trebuie sa il mut cu jumatate */
    //modelMatrixExplicatie1 *= transform2D::Translate(35, 0);
    //modelMatrixExplicatie1 *= transform2D::Translate(0, 10);
    //modelMatrixExplicatie1 *= transform2D::Rotate(M_PI);
    //RenderMesh2D(meshes["trapezBazaInferioara"], shaders["VertexColor"], modelMatrixExplicatie1);
    //glm::mat3 modelMatrixExplicatie2 = glm::mat3(1);
    //modelMatrixExplicatie2 *= transform2D::Translate(35, 0);
    //modelMatrixExplicatie2 *= transform2D::Translate(0, 10);
    //RenderMesh2D(meshes["trapezBazaSuperioara"], shaders["VertexColor"], modelMatrixExplicatie2);
    //glm::mat3 modelMatrixExplicatie3 = glm::mat3(1);
    //modelMatrixExplicatie3 *= transform2D::Translate(35, 0);
    //modelMatrixExplicatie3 *= transform2D::Translate(0, 25);
    //RenderMesh2D(meshes["cercDeasupraTrapezelor"], shaders["VertexColor"], modelMatrixExplicatie3);
    //glm::mat3 modelMatrixExplicatie4 = glm::mat3(1);
    //modelMatrixExplicatie4 *= transform2D::Translate(35, 0);
    //modelMatrixExplicatie4 *= transform2D::Translate(-4, 80);
    //modelMatrixExplicatie4 *= transform2D::Rotate(3 * M_PI / 2);
    //RenderMesh2D(meshes["teava"], shaders["VertexColor"], modelMatrixExplicatie4);
    //glm::mat3 modelMatrixExplicatie5 = glm::mat3(1);
    //modelMatrixExplicatie5 *= transform2D::Translate(35, 0);
    //modelMatrixExplicatie5 *= transform2D::Translate(0, 55);
    //RenderMesh2D(meshes["proiectil"], shaders["VertexColor"], modelMatrixExplicatie5);

}


void tema1::FrameEnd()
{
}

// Am luat CreateMesh din laboratorul 2
void tema1::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);  // Generează un VAO
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);  // Generează un VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);  // Leagă bufferul VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    // TODO(student): Send vertices data into the VBO buffer

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);  // Generează un IBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);  // Leagă bufferul IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
    // TODO(student): Send indices data into the IBO buffer

    // ========================================================================
    // This section demonstrates how the GPU vertex shader program
    // receives data. It will be learned later, when GLSL shaders will be
    // introduced. For the moment, just think that each property value from
    // our vertex format needs to be sent to a certain channel, in order to
    // know how to receive it in the GLSL vertex shader.

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);
    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}

void tema1::RenderTerrainSquare(float x1, float y1, float x2, float y2, glm::vec3 color) {

    // Calculez parametrii pentru scalare cum e dat pe ocw: Descriere si desenare teren
    float scaleX = x2 - x1;
    float scaleY = max(y2, y1);

    // Creez matricea de transformare
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x1, y1);
    modelMatrix *= transform2D::Forfecare(x1, y1, x2, y2);
    modelMatrix *= transform2D::Scale(scaleX, scaleY);

    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), color),   // Stanga sus
        VertexFormat(glm::vec3(1, 0, 0), color),   // Dreapta sus
        VertexFormat(glm::vec3(0, -1, 0), color),   // Stanga jos
        VertexFormat(glm::vec3(1, -1, 0), color)    // Dreapta jos
    };

    std::vector<unsigned int> indices = {
        2, 3, 0, // Primul triunghi din stanga ca pe ocw: Descriere si desenare teren
        1, 0, 3  // Al doilea triunghi din dreapta ca pe ocw: Descriere si desenare teren

    };

    std::string meshName = "terrain_segment_" + std::to_string(x1);
    if (meshes.find(meshName) == meshes.end()) {
        CreateMesh(meshName.c_str(), vertices, indices); // Creez mesh-ul doar daca nu exista deja
    }

    RenderMesh2D(meshes[meshName], shaders["VertexColor"], modelMatrix);
}

void tema1::CreateTrapezoidMesh(const char* name, float baseWidth, float topWidth, float height, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-baseWidth / 2, 0, 0), color),   // Stanga jos
        VertexFormat(glm::vec3(baseWidth / 2, 0, 0), color),    // Dreapta jos
        VertexFormat(glm::vec3(-topWidth / 2, height, 0), color), // Stanga sus
        VertexFormat(glm::vec3(topWidth / 2, height, 0), color)  // Dreapta sus
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,  // Primul triunghi
        1, 2, 3   // Al doilea triunghi
    };

    CreateMesh(name, vertices, indices);
}

void tema1::CreateRectangleMesh(const char* name, float width, float height, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), color),         // Stanga jos
        VertexFormat(glm::vec3(width, 0, 0), color),    // Dreapta jos
        VertexFormat(glm::vec3(0, height, 0), color),   // Stanga sus
        VertexFormat(glm::vec3(width, height, 0), color) // Dreapta sus
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        1, 2, 3
    };

    CreateMesh(name, vertices, indices);
}

void tema1::CreateRectangleConturMesh(const char* name, float width, float height, glm::vec3 color) {
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(0, 0, 0), color),         // Stanga jos
        VertexFormat(glm::vec3(width, 0, 0), color),    // Dreapta jos
        VertexFormat(glm::vec3(width, height, 0), color), // Dreapta sus
        VertexFormat(glm::vec3(0, height, 0), color)    // Stanga sus
    };

    // Indicii pentru a desena conturul dreptunghiului
    std::vector<unsigned int> indices = {
        0, 1, 1, 2, 2, 3, 3, 0,
    };

    CreateMesh(name, vertices, indices);

    // Setez modul de desenare la GL_LINES
    meshes[name]->SetDrawMode(GL_LINES);
}

void tema1::CreateCircleMesh(const char* name, float radius, int numTriangles, glm::vec3 color) {
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Adaugă vârful central al cercului (centrul)
    vertices.push_back(VertexFormat(glm::vec3(0.0f, 0.0f, 0.0f), color)); // Centrul cercului

    // Calculează vârfurile de pe marginea cercului
    for (int i = 0; i <= numTriangles; ++i) {
        float theta = 2.0f * M_PI * i / numTriangles; // Unghiul fiecărui vârf
        float x = radius * cos(theta);  // Coordonata X
        float y = radius * sin(theta);  // Coordonata Y

        // Adaugă vârful pe marginea cercului cu aceeași culoare
        vertices.push_back(VertexFormat(glm::vec3(x, y, 0.0f), color));

        // Adaugă indicii pentru triunghiurile care formează cercul
        if (i > 0) {
            indices.push_back(0);     // Centrul cercului
            indices.push_back(i);     // Vârful curent
            indices.push_back(i + 1); // Vârful următor
        }
    }

    // Fixarea ultimului triunghi (închide cercul)
    indices.push_back(0);           // Centrul cercului
    indices.push_back(numTriangles); // Ultimul vârf
    indices.push_back(1);           // Primul vârf de pe margine

    // Creează mesh-ul cercului
    CreateMesh(name, vertices, indices);
}

void tema1::CreateTankComponents1() {
    // Creez baza tancului
    CreateTrapezoidMesh("tankLowerBase1", 50, 40, 10, glm::vec3(0.455, 0.392, 0.306));

    // Creez corpul superior al tancului
    CreateTrapezoidMesh("tankUpperBase1", 70, 60, 15, glm::vec3(0.8, 0.686, 0.533));

    // Creez cercul pentru turela
    CreateCircleMesh("tankTurret1", 20, 50, glm::vec3(0.8, 0.686, 0.533));

    // Creez dreptunghiul pentru teava
    CreateRectangleMesh("tankBarrel1", 40, 8, glm::vec3(0.29f, 0.29f, 0.29f));
}

void tema1::CreateTankComponents2() {
    // Creez baza tancului
    CreateTrapezoidMesh("tankLowerBase2", 50, 40, 10, glm::vec3(0.2, 0.4, 0.));

    // Creez corpul superior al tancului
    CreateTrapezoidMesh("tankUpperBase2", 70, 60, 15, glm::vec3(0.349, 0.702, 0.));

    // Creez cercul pentru turela
    CreateCircleMesh("tankTurret2", 20, 50, glm::vec3(0.349, 0.702, 0.));

    // Creez dreptunghiul pentru teava
    CreateRectangleMesh("tankBarrel2", 40, 8, glm::vec3(0.29f, 0.29f, 0.29f));
}

void tema1::CreateLiniutaMesh(const char* name, float x1, float y1, float x2, float y2, glm::vec3 color) {
    // Definesc cele doua puncte care formeaza segmentul
    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(x1, y1, 0.0f), color),
        VertexFormat(glm::vec3(x2, y2, 0.0f), color)
    };

    // Indicii care definesc o linie intre cele doua puncte
    std::vector<unsigned int> indices = {
        0, 1
    };

    CreateMesh(name, vertices, indices);

    // Setez modul de desenare ca linie
    meshes[name]->SetDrawMode(GL_LINES);
}

void tema1::RenderTank1(float x, float y, float angle, float turretAngle) {
    // Desenez baza tancului
    glm::mat3 modelMatrix1 = glm::mat3(1);
    modelMatrix1 *= transform2D::Translate(x, y);
    modelMatrix1 *= transform2D::Rotate(angle);
    modelMatrix1 *= transform2D::Translate(0, 10);
    modelMatrix1 *= transform2D::Rotate(M_PI);
    RenderMesh2D(meshes["tankLowerBase1"], shaders["VertexColor"], modelMatrix1);

    // Desenez corpul superior al tancului
    glm::mat3 modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= transform2D::Translate(x, y);
    modelMatrix2 *= transform2D::Rotate(angle);
    modelMatrix2 *= transform2D::Translate(0, 10);
    RenderMesh2D(meshes["tankUpperBase1"], shaders["VertexColor"], modelMatrix2);

    // Desenez turela
    glm::mat3 modelMatrix3 = glm::mat3(1);
    modelMatrix3 *= transform2D::Translate(x, y);
    modelMatrix3 *= transform2D::Rotate(angle);
    modelMatrix3 *= transform2D::Translate(0, 25);
    RenderMesh2D(meshes["tankTurret1"], shaders["VertexColor"], modelMatrix3);

    // Desenez teava tunului
    glm::mat3 modelMatrix4 = glm::mat3(1);
    modelMatrix4 *= transform2D::Translate(x, y);
    modelMatrix4 *= transform2D::Rotate(angle);
    modelMatrix4 *= transform2D::Translate(0, 20);
    modelMatrix4 *= transform2D::Rotate(turretAngle);
    modelMatrix4 *= transform2D::Translate(-4, 60);
    modelMatrix4 *= transform2D::Rotate(3 * M_PI / 2);
    RenderMesh2D(meshes["tankBarrel1"], shaders["VertexColor"], modelMatrix4);
}

void tema1::RenderTank2(float x, float y, float angle, float turretAngle) {
    // Desenez baza tancului
    glm::mat3 modelMatrix1 = glm::mat3(1);
    modelMatrix1 *= transform2D::Translate(x, y);
    modelMatrix1 *= transform2D::Rotate(angle);
    modelMatrix1 *= transform2D::Translate(0, 10);
    modelMatrix1 *= transform2D::Rotate(M_PI);
    RenderMesh2D(meshes["tankLowerBase2"], shaders["VertexColor"], modelMatrix1);

    // Desenez corpul superior al tancului
    glm::mat3 modelMatrix2 = glm::mat3(1);
    modelMatrix2 *= transform2D::Translate(x, y);
    modelMatrix2 *= transform2D::Rotate(angle);
    modelMatrix2 *= transform2D::Translate(0, 10);
    RenderMesh2D(meshes["tankUpperBase2"], shaders["VertexColor"], modelMatrix2);

    // Desenez turela
    glm::mat3 modelMatrix3 = glm::mat3(1);
    modelMatrix3 *= transform2D::Translate(x, y);
    modelMatrix3 *= transform2D::Rotate(angle);
    modelMatrix3 *= transform2D::Translate(0, 25);
    RenderMesh2D(meshes["tankTurret2"], shaders["VertexColor"], modelMatrix3);

    // Desenez teava tunului
    glm::mat3 modelMatrix4 = glm::mat3(1);
    modelMatrix4 *= transform2D::Translate(x, y);
    modelMatrix4 *= transform2D::Rotate(angle);
    modelMatrix4 *= transform2D::Translate(0, 20);
    modelMatrix4 *= transform2D::Rotate(turretAngle);
    modelMatrix4 *= transform2D::Translate(-4, 60);
    modelMatrix4 *= transform2D::Rotate(3 * M_PI / 2);
    RenderMesh2D(meshes["tankBarrel2"], shaders["VertexColor"], modelMatrix4);
}

void tema1::RenderHealthBar(float x, float y, float healthTank, float maxHealth) {
    float barWidth = 50.0f; // Lungimea barei de viata
    float healthRatio = healthTank / maxHealth;
    glLineWidth(1.0f);

    // Bordura barei
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x - barWidth / 2, y + 100); // Pozitionez bara deasupra tancului
    RenderMesh2D(meshes["healthBarFrame"], shaders["VertexColor"], modelMatrix);

    //Bara umpluta (in functie de viata tancului)
    modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(x - barWidth / 2, y + 100);
    modelMatrix *= transform2D::Scale(healthRatio, 1.0f); // Scade bara proportional cu sanatatea
    RenderMesh2D(meshes["healthBarFill"], shaders["VertexColor"], modelMatrix);
}

void tema1::RenderTrajectory(float turretAngle, float angle, float tankX, float tankY) {
    glLineWidth(4.0f);
    // Pozitia initiala a proiectilului care este in teava
    float xLocal = 0.0f;
    float yLocal = 55.0f;

    // Rotesc punctul folosind unghiul turelei si adun la y 20 cum fac efectiv ca in RenderTank pentru teava la Rotate si Translate(0, 20)
    float xRotatedTurret = xLocal * cos(turretAngle) - yLocal * sin(turretAngle);
    float yRotatedTurret = xLocal * sin(turretAngle) + yLocal * cos(turretAngle) + 20;

    // Aplic dupa rotatia tancului precum am facut in RenderTank la teava
    float xRotatedTank = xRotatedTurret * cos(angle) - yRotatedTurret * sin(angle);
    float yRotatedTank = xRotatedTurret * sin(angle) + yRotatedTurret * cos(angle);

    float xGlobal = xRotatedTank + tankX;
    float yGlobal = yRotatedTank + tankY;

    glm::vec2 startPosition = glm::vec2(xGlobal, yGlobal);
    glm::vec2 velocity = glm::vec2(
        /* fac asa vectorul de deplasare ca eu trebuie sa ma rotesc dupa unghiul 
        tevii, dar si al tancului, iar eu am adaugat 90 de grade ca initial la mine
        unghiul turelei nu este la 0 grade, l-am intors la 270 de grade si ca sa ajung
        la 360 ca sa fiu la pozitia initiala trebuie sa mai adun 90 de grade */
        cos(turretAngle + angle + M_PI_2) * projectileSpeed,
        sin(turretAngle + angle + M_PI_2) * projectileSpeed
    );

    float timeStep = 0.1f; // Pasul de simulare
    float currentTime = 0.0f;
    glm::vec2 gravityEffect = glm::vec2(0, -gravity);

    // Stochez punctele traiectoriei intr un vector
    std::vector<glm::vec2> trajectoryPoints;

    // Salvez puncte de-a lungul traiectoriei
    for (int i = 0; i < 50; ++i) {
        // formula de mai jos este cea de pe ocw de la Lansare Proiectil(ultimele 2)
        glm::vec2 position = startPosition + velocity * currentTime + 0.5f * gravityEffect * currentTime * currentTime;
        trajectoryPoints.push_back(position);
        currentTime += timeStep;

        // Opresc daca punctul cade sub sol
        if (position.y < 0) break;
    }

    // Desenez linii intre punctele consecutive
    for (size_t i = 0; i < trajectoryPoints.size() - 1; ++i) {
        glm::vec2 start = trajectoryPoints[i];
        glm::vec2 end = trajectoryPoints[i + 1];

        std::string lineName = "trajectory_line_" + std::to_string(i);

        CreateLiniutaMesh(lineName.c_str(), start.x, start.y, end.x, end.y, glm::vec3(1.0f, 1.0f, 1.0f));

        RenderMesh2D(meshes[lineName], shaders["VertexColor"], glm::mat3(1));
    }
}

float tema1::GetTerrainHeightAt(float x) {
    int index = x / pointSpacing; // Calculez indexul în heightMap
    if (index < 0 || index >= heightMap.size() - 1) {
        return 0.0f; // Daca indexul este in afara domeniului, returnez 0 => poate fi solul
    }

    /* mai jos am aplicat formulele de pe ocw de la: Detectie coliziune proiectil-teren */

    // Punctele adiacente
    float x1 = index * pointSpacing;
    float x2 = (index + 1) * pointSpacing;
    float y1 = heightMap[index];
    float y2 = heightMap[index + 1];

    // Coeficientul de interpolare
    float t = (x - x1) / (x2 - x1);

    // Interpolarea liniara
    return y1 * (1 - t) + y2 * t;
}

bool tema1::CheckCollisionWithTerrain(const glm::vec2& projectilePos, float threshold) {
    float terrainHeight = GetTerrainHeightAt(projectilePos.x); // Inaltimea terenului la pozitia X a proiectilului
    return (projectilePos.y - terrainHeight) <= threshold; // Verific daca proiectilul este aproape de teren cu ajutorul pragului
}

bool tema1::CheckCircleCollision(glm::vec2 center1, float radius1, glm::vec2 center2, float radius2) {
    float distance = glm::length(center1 - center2); // Distanta reala intre centrul proiectilului si al tancului
    return distance <= (radius1 + radius2); // Coliziune daca distanta este mai mica decat suma razelor
}

void tema1::DeformTerrain(float explosionX, float explosionY, float radius) {
    for (int i = 0; i <= heightMap.size(); ++i) {
        float x = i * pointSpacing; // Coordonata X a punctului din heightMap
        float distanceX = x - explosionX; // Distanta fata de centru pe axa X

        // Verific daca punctul este in raza cercului si folosesc abs ca sa verific si in dreapta si in stanga
        if (abs(distanceX) <= radius) {

            /* ecuatia unui cerc in forma standard este (x−h)2+(y−k)2=r2, unde
            (h,k) este centrul, iar r este raza cercului, perechea (x,y) reprezinta
            orice punct de pe cerc.*/

            // Calculez inaltimea maximă afectata de cerc conform a ceea ce am scris mai sus
            float distanceY = sqrt(radius * radius - distanceX * distanceX);

            /* explosionY - distanceY reprezinta limita inferioara a deformarii pe axa Y, iar
            daca punctul curent al terenului este mai sus decat aceasta limita, il ajustam */ 
            if (heightMap[i] > explosionY - distanceY) {
                heightMap[i] = explosionY - distanceY;
            }
        }
    }
}

void tema1::SlideTerrain(float deltaTimeSeconds) {
    float threshold = 15.0f;       // Pragul pentru diferenta de inaltime
    float transferRate = 100.0f;    // Rata de transfer de înălțime

    for (size_t i = 0; i < heightMap.size() - 1; ++i) {
        float diff = heightMap[i] - heightMap[i + 1];

        if (abs(diff) > threshold) { // Verific daca diferenta depaseste pragul
            float transfer = transferRate * deltaTimeSeconds; // Calculez transferul pe baza ratei si deltaTime
            if (diff > 0) { // Daca punctul curent este mai inalt decat urmatorul
                heightMap[i] -= transfer;      // Scad inaltimea punctului curent
                heightMap[i + 1] += transfer;  // Adaug la urmatorul punct
            }
            else { // Daca urmatorul punct este mai inalt
                heightMap[i] += transfer;      // Adaug inaltimea punctului curent
                heightMap[i + 1] -= transfer;  // Scad de la urmatorul punct
            }
        }
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see input_controller.h.
 */


void tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void tema1::OnKeyPress(int key, int mods)
{   
    /* Aici am facut la fel ca la RenderTrajectory, doar ca acolo mai trebuia sa imi unesc punctele
    si sa mai fac niste operatii pe care o sa le fac in Update aici */

    if (key == GLFW_KEY_SPACE && firstTankHealth > 0) {
        Projectile newProjectile;

        float xLocal = 0.0f;
        float yLocal = 55.0f;

        float xRotatedTurret = xLocal * cos(firstTankTurretAngle) - yLocal * sin(firstTankTurretAngle);
        float yRotatedTurret = xLocal * sin(firstTankTurretAngle) + yLocal * cos(firstTankTurretAngle) + 20;

        float xRotatedTank = xRotatedTurret * cos(firstTankAngle) - yRotatedTurret * sin(firstTankAngle);
        float yRotatedTank = xRotatedTurret * sin(firstTankAngle) + yRotatedTurret * cos(firstTankAngle);

        float xGlobal = xRotatedTank + firstTankX;
        float yGlobal = yRotatedTank + firstTankY;

        newProjectile.position = glm::vec2(xGlobal, yGlobal);

        float combinedAngle = firstTankAngle + firstTankTurretAngle;

        float speed = 300.0f;
        newProjectile.velocity = glm::vec2(
            cos(combinedAngle + M_PI_2) * speed,
            sin(combinedAngle + M_PI_2) * speed
        );

        newProjectile.active = true;
        projectiles.push_back(newProjectile);
    }

    if (key == GLFW_KEY_ENTER && secondTankHealth > 0) {
        Projectile newProjectile;

        float xLocal = 0.0f;
        float yLocal = 55.0f;

        float xRotatedTurret = xLocal * cos(secondTankTurretAngle) - yLocal * sin(secondTankTurretAngle);
        float yRotatedTurret = xLocal * sin(secondTankTurretAngle) + yLocal * cos(secondTankTurretAngle) + 20;

        float xRotatedTank = xRotatedTurret * cos(secondTankAngle) - yRotatedTurret * sin(secondTankAngle);
        float yRotatedTank = xRotatedTurret * sin(secondTankAngle) + yRotatedTurret * cos(secondTankAngle);

        float xGlobal = xRotatedTank + secondTankX;
        float yGlobal = yRotatedTank + secondTankY;

        newProjectile.position = glm::vec2(xGlobal, yGlobal);

        float combinedAngle = secondTankAngle + secondTankTurretAngle;

        float speed = 300.0f;
        newProjectile.velocity = glm::vec2(
            cos(combinedAngle + M_PI_2) * speed,
            sin(combinedAngle + M_PI_2) * speed
        );

        newProjectile.active = true;
        projectilesSecondTank.push_back(newProjectile);
    }
}


void tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void tema1::OnWindowResize(int width, int height)
{
}