#include "Planet.h"
#include <glad/glad.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

namespace {
const float G = 6.67430e-11f; // gravitational constant in SI units

const char* vertexShaderSource = "#version 330 core\n"
"layout(location = 0) in vec3 aPos;\n"
"uniform mat4 MVP;\n"
"void main(){\n"
"    gl_Position = MVP * vec4(aPos, 1.0);\n"
"}\n";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main(){\n"
"    FragColor = vec4(0.2, 0.5, 1.0, 1.0);\n"
"}\n";

GLuint compileShader(GLenum type, const char* src) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);
    int success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info[512];
        glGetShaderInfoLog(shader, 512, nullptr, info);
        printf("Shader error: %s\n", info);
    }
    return shader;
}
}

GLuint createPlanetShader() {
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

Planet::Planet(float mass, glm::vec3 position, glm::vec3 velocity)
    : m_mass(mass), m_position(position), m_velocity(velocity) {
    initGraphics();
}

void Planet::initGraphics() {
    std::vector<glm::vec3> vertices;
    const int stacks = 18, slices = 36;
    for (int i = 0; i <= stacks; ++i) {
        float V   = i / (float)stacks;
        float phi = glm::pi<float>() * V - glm::pi<float>() / 2.0f;
        for (int j = 0; j <= slices; ++j) {
            float U = j / (float)slices;
            float theta = glm::two_pi<float>() * U;
            float x = cos(phi) * cos(theta);
            float y = sin(phi);
            float z = cos(phi) * sin(theta);
            vertices.emplace_back(x, y, z);
        }
    }
    m_vertexCount = static_cast<GLsizei>(vertices.size());

    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Planet::updatePhysics(float dt, const Planet& attractor) {
    glm::vec3 r = attractor.m_position - m_position;
    float distSqr = glm::dot(r, r);
    if (distSqr < 1e-5f) return;
    float forceMag = G * attractor.m_mass / distSqr;
    glm::vec3 acc = forceMag * glm::normalize(r);
    m_velocity += acc * dt;
    m_position += m_velocity * dt;
}

void Planet::draw(GLuint shaderProgram) const {
    glBindVertexArray(m_vao);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_position);
    glm::mat4 view = glm::lookAt(glm::vec3(0,0,300), glm::vec3(0,0,0), glm::vec3(0,1,0));
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 1000.0f);
    glm::mat4 mvp = proj * view * model;
    GLuint loc = glGetUniformLocation(shaderProgram, "MVP");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mvp));

    glDrawArrays(GL_POINTS, 0, m_vertexCount);
}
