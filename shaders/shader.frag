#version 330 core

in vec3 fragNormal;  // Input from the vertex shader

// Uniforms for lighting
uniform vec3 AmbientColor = vec3(0.2); // Ambient light color
uniform vec3 LightDirection = normalize(vec3(0, -1, 2)); // Direction of the light source
uniform vec3 LightColor = vec3(1); // Light color (white)
uniform vec3 DiffuseColor; // Diffuse color of the material

out vec4 fragColor; // Output color of the fragment

void main()
{
    // Normalize the fragment normal
    vec3 norm = normalize(fragNormal);
    
    // Compute the diffuse light intensity
    float diff = max(dot(LightDirection, norm), 0.0);
    
    // Compute the irradiance (sum of ambient and diffuse lighting)
    vec3 irradiance = AmbientColor + LightColor * diff;

    // Compute the diffuse reflectance
    vec3 reflectance = irradiance * DiffuseColor;

    // Apply gamma correction (assumes gamma 2.2)
    vec3 gammaCorrectedReflectance = sqrt(reflectance);

    // Clamp the final color to avoid values greater than 1
    gammaCorrectedReflectance = clamp(gammaCorrectedReflectance, 0.0, 1.0);
    
    // Set the fragment color
    fragColor = vec4(gammaCorrectedReflectance, 1.0);
}
