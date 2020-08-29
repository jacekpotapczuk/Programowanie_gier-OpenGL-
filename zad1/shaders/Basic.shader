#shader vertex
#version 150

in vec3 position;
in vec3 vertexColor;

uniform mat4 u_model_matrix;

uniform mat4 u_rotate_x_matrix;
uniform mat4 u_rotate_y_matrix;
uniform mat4 u_rotate_z_matrix;

uniform mat4 u_view_matrix;
uniform mat4 u_projection_matrix;

out vec3 fragmentColor;

void main()
{
	gl_Position = u_projection_matrix * u_view_matrix * u_model_matrix * u_rotate_z_matrix * u_rotate_y_matrix * u_rotate_x_matrix * vec4(position, 1.0f);

	fragmentColor = vertexColor;
};



#shader fragment
#version 150 core

in vec3 fragmentColor;

out vec4 out_color;

uniform vec4 u_color;

void main()
{
	//out_color = u_color;
	out_color = vec4(fragmentColor, 1.0);
};
