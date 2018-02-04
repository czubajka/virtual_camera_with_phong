 #version 330 core  

	out vec4 FragColor;  
	in vec3 Normal; 
	in vec3 FragPosition;
	
	struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
	};

	struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	};

	uniform Material material;
	uniform Light light;
	uniform vec3 cameraPosition;

	void main()  
	{  
		//ambient
		 vec3 ambient = material.ambient * light.ambient;

		 //diffuse
		 vec3 norm = normalize(Normal);
		 vec3 lightDir = normalize(light.position - FragPosition);
		 float diff = max(dot(norm, lightDir), 0.0);			//max w przypadku kata>90 , ktory spowoduje ujemny wynik, wybiera wyzsza wartosc
		 vec3 diffuse = (diff * material.diffuse) * light.diffuse;

		 //specular
		 vec3 viewDir = normalize(cameraPosition - FragPosition);
		 vec3 reflectDir = reflect(-lightDir, norm);
		 float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		 vec3 specular = light.specular * (spec * material.specular);

		 float r = 1/(sqrt(pow((light.position.x - FragPosition.x),2) + pow((light.position.y - FragPosition.y),2) + pow((light.position.y - FragPosition.y),2)) + 0.5);	//wspolczynnik tlumienia z odlegloscia

		 vec3 result = ambient + diffuse + (specular * r);
		 FragColor = vec4(result, 1.0);  
	}