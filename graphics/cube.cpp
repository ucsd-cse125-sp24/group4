#include "../include/cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
	// Model matrix - Maybe decouple from cube?
	model = glm::mat4(1.0f);

	// Color of the cube
	color = glm::vec3(1.0f, 0.95f, 0.1f);

    // Specify vertex positions
    positions = {
        // Front
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

        // Back
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

        // Top
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Bottom
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

        // Left
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

        // Right
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z) };

    // Specify normals
    normals = {
        // Front
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),
        glm::vec3(0, 0, 1),

        // Back
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),

        // Top
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 1, 0),

        // Bottom
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),
        glm::vec3(0, -1, 0),

        // Left
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),
        glm::vec3(-1, 0, 0),

        // Right
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0),
        glm::vec3(1, 0, 0) };

     // Specify indices - Triangles
     indices = {
         0, 1, 2, 0, 2, 3,        // Front
         4, 5, 6, 4, 6, 7,        // Back
         8, 9, 10, 8, 10, 11,     // Top
         12, 13, 14, 12, 14, 15,  // Bottom
         16, 17, 18, 16, 18, 19,  // Left
         20, 21, 22, 20, 22, 23,  // Right
     };


     // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
     glGenVertexArrays(1, &VAO);
     glGenBuffers(1, &VBO_positions);
     glGenBuffers(1, &VBO_normals);

     // Bind to the VAO.
     glBindVertexArray(VAO);

     // Bind to the first VBO - We will use it to store the vertices
     glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* positions.size(), positions.data(), GL_STATIC_DRAW);
     glEnableVertexAttribArray(0);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

     // Bind to the second VBO - We will use it to store the normals
     glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
     glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3)* normals.size(), normals.data(), GL_STATIC_DRAW);
     glEnableVertexAttribArray(1);
     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

     // Generate EBO, bind the EBO to the bound VAO and send the data
     glGenBuffers(1, &EBO);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)* indices.size(), indices.data(), GL_STATIC_DRAW);

     // Unbind the VBOs.
     glBindBuffer(GL_ARRAY_BUFFER, 0);
     glBindVertexArray(0);
}

Cube::~Cube() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::draw(const glm::mat4& viewProjMtx, Shader* shader) {
    // Activate the shader program
    //shader->activate();

    // get the locations and send the uniforms to the shader
    print_world();
    shader->set_mat4("viewProj", (float*)&viewProjMtx);
    shader->set_mat4("model", (float*)&model);
    shader->set_vec3("DiffuseColor", &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // Draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, (int)indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    //glUseProgram(0);
}

void Cube::print_world() {
	std::cout << "Cube world matrix: " << std::endl;
	for (int i = 0; i < 4; i++) {
		std::cout << model[i][0] << " " << model[i][1] << " " << model[i][2] << " " << model[i][3] << std::endl;
	}
}