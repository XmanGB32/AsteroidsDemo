#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <cmath>
#include <iostream>

const int WIDTH = 800;
const int HEIGHT = 600;

// Basic vertex shader source
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 projection;
void main() {
    gl_Position = projection * model * vec4(aPos, 1.0);
}
)";

// Basic fragment shader source
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0); // White
}
)";

// Structure for game objects
struct Object {
    glm::vec2 position;
    glm::vec2 velocity;
    float rotation = 0.0f;
    float radius = 20.0f; // For collision
};

// Bullets
struct Bullet {
    glm::vec2 position;
    glm::vec2 velocity;
    float life = 2.0f; // Seconds
};

// Global variables
Object ship = { {WIDTH / 2.0f, HEIGHT / 2.0f}, {0.0f, 0.0f} };
std::vector<Object> asteroids = {
    { {100.0f, 100.0f}, {0.5f, 0.3f}, 0.0f, 30.0f },
    { {700.0f, 500.0f}, {-0.4f, -0.6f}, 0.0f, 40.0f }
};
std::vector<Bullet> bullets;
float thrust = 0.0f;
float turn = 0.0f;
float deltaTime = 0.0f;

void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);

    thrust = 0.0f; // Reset thrust
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { // Add down arrow for thrust
        thrust = 5.0f;
    }

    turn = 0.0f; // Reset turn
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) { // Add left arrow for turn left
        turn += 200.0f;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) { // Add right arrow for turn right
        turn += -200.0f;
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        static float lastShot = 0.0f;
        float time = glfwGetTime();
        if (time - lastShot > 0.2f) {
            glm::vec2 dir = { -std::sin(glm::radians(ship.rotation)), std::cos(glm::radians(ship.rotation)) };
            bullets.push_back({ ship.position, dir * 5.0f + ship.velocity });
            lastShot = time;
        }
    }
}

// Check circle collision
bool collide(const Object& a, const Object& b) {
    glm::vec2 diff = a.position - b.position;
    float dist = glm::length(diff);
    return dist < a.radius + b.radius;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Asteroids Demo", NULL, NULL);
    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Compile shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Ortho projection
    glm::mat4 projection = glm::ortho(0.0f, (float)WIDTH, 0.0f, (float)HEIGHT, -1.0f, 1.0f);

    // VBO for lines (ship and asteroids are drawn with lines)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 100, NULL, GL_DYNAMIC_DRAW); // Buffer for dynamic vertices
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glLineWidth(2.0f);

    float lastFrame = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Update ship
        ship.rotation += turn * deltaTime;
        glm::vec2 dir = { -std::sin(glm::radians(ship.rotation)), std::cos(glm::radians(ship.rotation)) };
        ship.velocity += dir * thrust * deltaTime;
        ship.velocity *= 0.99f; // Friction
        ship.position += ship.velocity;
        if (ship.position.x < 0) ship.position.x += WIDTH;
        if (ship.position.x > WIDTH) ship.position.x -= WIDTH;
        if (ship.position.y < 0) ship.position.y += HEIGHT;
        if (ship.position.y > HEIGHT) ship.position.y -= HEIGHT;

        // Update asteroids
        for (auto& ast : asteroids) {
            ast.position += ast.velocity;
            if (ast.position.x < 0) ast.position.x += WIDTH;
            if (ast.position.x > WIDTH) ast.position.x -= WIDTH;
            if (ast.position.y < 0) ast.position.y += HEIGHT;
            if (ast.position.y > HEIGHT) ast.position.y -= HEIGHT;

            // Collision with ship
            if (collide(ship, ast)) {
                ship.position = { WIDTH / 2.0f, HEIGHT / 2.0f }; // Reset
                ship.velocity = { 0.0f, 0.0f };
            }
        }

        // Update bullets
        for (auto it = bullets.begin(); it != bullets.end(); ) {
            it->position += it->velocity;
            it->life -= deltaTime;
            if (it->life <= 0.0f) {
                it = bullets.erase(it);
            }
            else {
                // Wrap bullets
                if (it->position.x < 0) it->position.x += WIDTH;
                if (it->position.x > WIDTH) it->position.x -= WIDTH;
                if (it->position.y < 0) it->position.y += HEIGHT;
                if (it->position.y > HEIGHT) it->position.y -= HEIGHT;

                // Collision with asteroids
                for (auto astIt = asteroids.begin(); astIt != asteroids.end(); ) {
                    if (collide({ it->position, {0,0}, 0, 2.0f }, *astIt)) {
                        astIt = asteroids.erase(astIt);
                        it = bullets.erase(it);
                        break;
                    }
                    else {
                        ++astIt;
                    }
                }
                if (it != bullets.end()) ++it;
            }
        }

        // Render
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        // Draw ship (triangle)
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(ship.position.x, ship.position.y, 0.0f));
        model = glm::rotate(model, glm::radians(ship.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        float shipVerts[] = {
            0.0f, 20.0f, 0.0f,
            -10.0f, -10.0f, 0.0f,
            10.0f, -10.0f, 0.0f
        };
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(shipVerts), shipVerts);
        glDrawArrays(GL_LINE_LOOP, 0, 3);

        // Draw asteroids (rough circles with lines)
        for (const auto& ast : asteroids) {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(ast.position.x, ast.position.y, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            std::vector<float> astVerts;
            for (int i = 0; i < 10; ++i) {
                float angle = i * 36.0f;
                astVerts.push_back(ast.radius * std::cos(glm::radians(angle)));
                astVerts.push_back(ast.radius * std::sin(glm::radians(angle)));
                astVerts.push_back(0.0f);
            }
            glBufferSubData(GL_ARRAY_BUFFER, 0, astVerts.size() * sizeof(float), astVerts.data());
            glDrawArrays(GL_LINE_LOOP, 0, 10);
        }

        // Draw bullets (points)
        glPointSize(5.0f);
        for (const auto& bullet : bullets) {
            model = glm::translate(glm::mat4(1.0f), glm::vec3(bullet.position.x, bullet.position.y, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
            float bulletVert[] = { 0.0f, 0.0f, 0.0f };
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bulletVert), bulletVert);
            glDrawArrays(GL_POINTS, 0, 1);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}