// Simple Color Shader - Vertex Shader
#version 410 core                                            
                                                             
in vec4 position;                                            
                                                             
out VS_OUT                                                   
{                                                            
    vec4 color;                                              
} vs_out;                                                    

// Uniforms:                                                             
uniform mat4 world_matrix;                                     
uniform mat4 view_matrix;
uniform mat4 proj_matrix;                                    
uniform vec4 color_vector;
                                                             
void main(void)                                              
{
    // Remember: We use ROW major, but OpenGL GLSL multiplies in reverse order!
    gl_Position = proj_matrix * view_matrix * world_matrix * position;        

	// Color is specified by user
    vs_out.color = color_vector;
}                                                            