#version 330 core
out vec4 FragColor;

// Taking input from the vertex shader
// It is necessary that the variable names are the same in both shaders

uniform vec3 u_objectColor;
uniform vec3 u_lightColor;
uniform vec3 u_lightPos;
uniform vec3 u_viewPos;

uniform float u_ambientStrength;
uniform float u_specularStrength;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    // Simple version of global illumination
    vec3 ambient = u_ambientStrength * u_lightColor;

    // When doing lighting calculations. it's advised to normalize the relevant vectors
    // Because we only care about their direction and it simplifies most calculations
    vec3 norm =  normalize(Normal);
    vec3 lightDir =  normalize(u_lightPos - FragPos); // The light's position vector relative to the fragment's position vector
    // The dot product between the normal vector and the lightDir vector
    // As the angles increase, the light gets less effect. The inverse is true as well.
    // Dot product of 1 is when the angle is really small. Dot product of 0 is when they are perpendicular
    // In the case of obtuse angles that result in negative dot product, we return 0
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // Defining specular intensity
    vec3 viewDir = normalize(u_viewPos - FragPos);
    // The reflect function expects the first vector to point from the light source to the fragment's pos
    // Currently, the lightDir vector is pointing the other way around (fragment pos towards the light source depending on the order of subtraction earlier)
    // To make sure we get the correct reflect vector, we negate the lightDir vector first
    vec3 reflectDir = reflect(-lightDir, norm); 
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32); // The 32 stands for the shininess value of the highlight
    vec3 specular = u_specularStrength * spec * u_lightColor;

    vec3 result = (ambient + diffuse + specular) * u_objectColor; // Creating ambient lighting in the scene (for our object)

    FragColor = vec4(result, 1.0);
}