class Sphere
{
        public:
    GLuint sphere_vbo[4];//={-1,-1,-1,-1};
    GLuint sphere_vao[4];//={-1,-1,-1,-1};

unsigned int texture_ava = 0;
unsigned int map;

unsigned int stack=100;
unsigned int sector=100;

GLfloat pos[101*101*3];
GLfloat tex_coord[101*101*2];

unsigned int indi[100*100*6];

Sphere(int radius)
    {
        unsigned int k=0,l=0,s,t;
        for(int i=0;i<=stack;i++)
        {
            for(int j=0;j<=sector;j++)
            {
                pos[k++]=cos(M_PI*i/stack);                             //position of z
                pos[k++]=sin(M_PI*i/stack)*sin(2*M_PI*j/sector);          //position of y
                pos[k++]=sin(M_PI*i/stack)*cos(2*M_PI*j/sector);          //position of x

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
                indi[k++]=(i+1)*(sector+1)+j+1;
                indi[k++]=i*(sector+1)+j+1;
            }
        }




        // [VAO/VBO stuff]
        GLuint i;
        glGenVertexArrays(4,sphere_vao);
        glGenBuffers(4,sphere_vbo);
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

        i=2; // indices
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,sphere_vbo[i]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indi),indi,GL_STATIC_DRAW);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,3,GL_UNSIGNED_INT,GL_FALSE,0,0);


    /*
        i=3; // color
        glBindBuffer(GL_ARRAY_BUFFER,sphere_vbo[i]);
        glBufferData(GL_ARRAY_BUFFER,sizeof(sphere_col),sphere_col,GL_STATIC_DRAW);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i,3,GL_FLOAT,GL_FALSE,0,0);
    */
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
        }

    void texture(unsigned int m)
        {   
            map=m;
            texture_ava=1;
        }
    

    void sphere_draw()
        {
    //    glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        //glEnable(GL_LIGHTING);
        //glEnable(GL_LIGHT0);
    
        glBindVertexArray(sphere_vao[0]);
    //  glDrawArrays(GL_POINTS,0,sizeof(sphere_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
        
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, map);

        glDrawElements(GL_TRIANGLES,sizeof(indi)/sizeof(GLuint),GL_UNSIGNED_INT,0);    // indices (choose just one line not both !!!)
        glBindVertexArray(0);
        }
    
    void draw()
        {
    //    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    //    float aspect=1;;
   //     glMatrixMode(GL_PROJECTION);
   //     glLoadIdentity();
    //  gluPerspective(60.0/aspect,aspect,0.1,100.0);
  //      glMatrixMode(GL_TEXTURE);
  //      glLoadIdentity();
  //      glMatrixMode(GL_MODELVIEW);
 //       glLoadIdentity();
 //       glTranslatef(0.0,0.0,-10.0);
    
        glEnable(GL_DEPTH_TEST);
//        glDisable(GL_TEXTURE_2D);
        sphere_draw();
    
        }
            void exit()
        {
        glDeleteVertexArrays(4,sphere_vao);
        glDeleteBuffers(4,sphere_vbo);
        }
        
};