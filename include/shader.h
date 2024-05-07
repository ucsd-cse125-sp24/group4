#pragma once

// Shader from learnOpenGL: https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h
// https://learnopengl.com/Getting-started/Shaders

#include "glad/glad.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
    unsigned int ID;
    // constructor generates the shader on the fly
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();


    // activate the shader
    // ------------------------------------------------------------------------
    void activate();

    // utility uniform functions
    // ------------------------------------------------------------------------
    void set_bool(const std::string& name, bool value) const;
    void set_int(const std::string& name, int value) const;
    void set_float(const std::string& name, float value) const;
    void set_vec3(const std::string& name, const float* value   ) const;
    void set_mat4(const std::string& name, const float* value) const;

private:
    // utility function for checking shader compilation/linking errors.
    void check_compile_errors(unsigned int shader, std::string type);
};