class Sphere
{
    public:
    GLuint sphere_vbo[5];
    GLuint sphere_vao[5];

	unsigned int stack=100;
	unsigned int sector=100;

	GLfloat pos[101*101*3];
	GLfloat normal[101*101*3];
	GLfloat tex_coord[101*101*2];
	GLfloat tangent[101*101*3];

	unsigned int indi[100*100*6];

	Sphere(int radius)
    	{
        	unsigned int k=0,l=0,m=0,yu=0,s,t;
        	for(int i=0;i<=stack;i++)
        	{
            	for(int j=0;j<=sector;j++)
            	{
                	pos[k++]=radius*cos(M_PI*i/stack);                               //position of z
                	pos[k++]=radius*sin(M_PI*i/stack)*sin(2*M_PI*j/sector);          //position of y
                	pos[k++]=radius*sin(M_PI*i/stack)*cos(2*M_PI*j/sector);          //position of x

                	normal[m++]=cos(M_PI*i/stack);                               //position of z
                	normal[m++]=sin(M_PI*i/stack)*sin(2*M_PI*j/sector);          //position of y
                	normal[m++]=sin(M_PI*i/stack)*cos(2*M_PI*j/sector);          //position of x

                	tangent[yu++]=(-1)*sin(M_PI*i/stack);                           //position of z
                	tangent[yu++]=cos(M_PI*i/stack)*sin(2*M_PI*j/sector);          //position of y
              		tangent[yu++]=cos(M_PI*i/stack)*cos(2*M_PI*j/sector);          //position of x

                	tex_coord[l++]= (float)j / sector;
                	tex_coord[l++]= (float)i / stack;
            	}
        	}
        
        	k=0;

        	for(int i=0;i<stack;i++)
        	{
            	for(int j=0;j<sector;j++)
            	{
            	    indi[k++]=i*(sector+1)+j;
            	    indi[k++]=i*(sector+1)+j+1;
            	    indi[k++]=(i+1)*(sector+1)+j;
            	    indi[k++]=(i+1)*(sector+1)+j;
                    indi[k++]=i*(sector+1)+j+1;
            	    indi[k++]=(i+1)*(sector+1)+j+1;
            	}
        	}

        	// [VAO/VBO stuff]
        	GLuint i;
        	glGenVertexArrays(5,sphere_vao);
        	glGenBuffers(5,sphere_vbo);
        	glBindVertexArray(sphere_vao[0]);

        	i=0; // vertex
        	glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
        	glBufferData(GL_ARRAY_BUFFER,sizeof(pos),pos,GL_STATIC_DRAW);
        	glEnableVertexAttribArray(i);
        	glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);

        	i=1; // texture coordinates
        	glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
        	glBufferData(GL_ARRAY_BUFFER,sizeof(tex_coord),tex_coord,GL_STATIC_DRAW);
        	glEnableVertexAttribArray(i);
        	glVertexAttribPointer(i,2,GL_FLOAT,GL_FALSE,0,0);

        	i=2; // normal
        	glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
        	glBufferData(GL_ARRAY_BUFFER,sizeof(normal),normal,GL_STATIC_DRAW);
        	glEnableVertexAttribArray(i);
        	glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);


        	i=3; // tangents
        	glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
        	glBufferData(GL_ARRAY_BUFFER,sizeof(tangent),tangent,GL_STATIC_DRAW);
        	glEnableVertexAttribArray(i);
        	glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);


        	i=4; // indices
        	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphere_vbo[i]);
        	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indi),indi,GL_STATIC_DRAW);
        	glEnableVertexAttribArray(i);
        	glVertexAttribPointer(i,3,GL_UNSIGNED_INT,GL_FALSE,0,0);

        	glBindVertexArray(0);
        	glBindBuffer(GL_ARRAY_BUFFER,0);
        	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        	glDisableVertexAttribArray(0);
        	glDisableVertexAttribArray(1);
        	glDisableVertexAttribArray(2);
        	glDisableVertexAttribArray(3);
			glDisableVertexAttribArray(4);
        }

    void sphere_draw()
        {
        	glEnable(GL_CULL_FACE);
        	glFrontFace(GL_CCW);
    
        	glBindVertexArray(sphere_vao[0]);

			glDrawElements(GL_TRIANGLES,sizeof(indi)/sizeof(GLuint),GL_UNSIGNED_INT,0);
        	glBindVertexArray(0);
        }
    
    void draw()
        {   
        	glEnable(GL_DEPTH_TEST);
        	sphere_draw();
        }
    
    void exit()
        {
        	glDeleteVertexArrays(5,sphere_vao);
        	glDeleteBuffers(5,sphere_vbo);
        }
};