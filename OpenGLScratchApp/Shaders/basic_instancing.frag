#version 410 core													

layout(std140) uniform instancingMatrixBlock
{
    mat4[1000] instanceModels;

}block_matrix_instancing;

layout(std140) uniform instancingColorBlock
{
    vec4[1000] instancingColor;

}block_color_instancing;

in vec2 TexCoord;
flat in int InstanceID;

out vec4 color; 


																			
void main()																	
{	

	color = block_color_instancing.instancingColor[InstanceID];
	//color = vec4(1.0, 0.0, 0.0, 1.0);
	    	
	
																	    
}