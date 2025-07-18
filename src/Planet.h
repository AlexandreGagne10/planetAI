#ifndef PLANET_H
#define PLANET_H

#include <glm/glm.hpp>
#include <vector>

GLuint createPlanetShader();

class Planet {
public:
    Planet(float mass, glm::vec3 position, glm::vec3 velocity);

    void updatePhysics(float dt, const Planet& attractor);
    void draw(GLuint shaderProgram) const;

private:
    float m_mass;
    glm::vec3 m_position;
    glm::vec3 m_velocity;
    GLuint m_vao = 0;
    GLsizei m_vertexCount = 0;

    void initGraphics();
};

#endif // PLANET_H
