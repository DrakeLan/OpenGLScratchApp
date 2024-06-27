#version 410 core                                            

layout(std140) uniform globalMatrixBlock
{
    mat4 projection;
    mat4 view;
}block_matrices;


layout (location = 0) in vec3 Pos;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec3 tangent;

out vec2 TexCoord;
out vec3 PositionWS;
out vec3 NormalWS;
out vec3 TangentWS;
out vec3 BiTangengt;
out mat3 TBN;

uniform mat4 model;                                                      
                                                                            
void main()                                                                  
{         
    TexCoord = uv;
    PositionWS = (model * vec4(Pos, 1.0)).xyz;
    NormalWS = normalize(mat3(transpose(inverse(model))) * normal);
    TangentWS = normalize(mat3(transpose(inverse(model))) * tangent);
    BiTangengt = normalize(cross(NormalWS, TangentWS));

    TBN = mat3(TangentWS, BiTangengt, NormalWS);

    gl_Position = block_matrices.projection * block_matrices.view * vec4(PositionWS, 1.0);
}