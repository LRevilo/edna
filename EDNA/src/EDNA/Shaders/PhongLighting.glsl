vec4 Phong_CalculateLighting(vec3 lightPos, vec3 lightColor, vec3 viewPos, vec3 fragPos, vec3 fragNormal, vec3 fragColor, float shadow) 
{

    vec3 normal = normalize(fragNormal);

    // ambient
    vec3 ambient = 0.15 * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    


    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * fragColor;  

    return vec4(lighting,1.0);
}
    


