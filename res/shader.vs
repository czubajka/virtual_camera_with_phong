#version 330 core  

	layout (location = 0) in vec3 aPosition;  		//pozycja atrybutu = 0
	layout (location = 1) in vec3 aNormal;  		//pozycja atrybutu = 1
	out vec3 Normal; 
	out vec3 FragPosition;
	
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;

	void main()  
	{  
		FragPosition = vec3(model * vec4(aPosition, 1.0));						//obliczenia oswietlenia w przestrzeni sceny (world space)
		gl_Position = projection * view * model * vec4(aPosition, 1.0); 
		Normal = mat3(transpose(inverse(model))) * aNormal;						 //normal matrix
	}