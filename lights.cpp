#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/camera.h"
#include "include/shader.h"
#include "include/head.hpp"
#include "include/sphere.cpp"


int main(int argc, char** argv)
{
  GLFWwindow* window;

  glfwSetErrorCallback(opengl::error_callback);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT , "Lights", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window,key_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

//Here beings the game
opengl::initGL();

float skyVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

glm::vec3 sunPos(-90.0f,0.0f,0.0f);
glm::vec3 sunDir(90.0f,0.0f,0.0f);

glm::vec3 earthPos(1.0f, 1.0f, 1.0f);

Shader earthShader("Shader/earth_vs.glsl","Shader/earth_fs.glsl");
Shader sunShader("Shader/sun_vs.glsl","Shader/sun_fs.glsl");
Shader earth1("Shader/texturedDiffuse.vert","Shader/texturedDiffuse.frag");

Sphere earth(2);
Sphere sun(1);

unsigned int loadTexture(const char *path);

unsigned int earth_tex= loadTexture("Texture/earth.jpg");
//unsigned int earth_spec_map=loadTexture("Texture/EarthSpec.png");
//unsigned int earth_norm_map=loadTexture("Texture/earthNormal.png");
unsigned int sun_tex=loadTexture("Texture/sun.jpg");

earth.texture(earth_tex);
//earth.specular_map(earth_spec_map);
//earth.normal_map(earth_norm_map);
sun.texture(sun_tex);

const glm::vec4 white(1);
const glm::vec4 black(0);
const glm::vec4 ambient( 0.1f, 0.1f, 0.1f, 1.0f );

unsigned int skyVAO, skyVBO;
glGenVertexArrays(1, &skyVAO);
glGenBuffers(1, &skyVBO);
glBindVertexArray(skyVAO);
glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), &skyVertices, GL_STATIC_DRAW);
glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

std::vector<std::string> faces=
{
  "Texture/skybox/right.png",
  "Texture/skybox/left.png",
  "Texture/skybox/top.png",
  "Texture/skybox/bottom.png",
  "Texture/skybox/front.png",
  "Texture/skybox/back.png"
};

unsigned int cubemapTexture = loadCubemap(faces);  

Shader skyShader("Shader/sky_vs.glsl","Shader/sky_fs.glsl");


  while (glfwWindowShouldClose(window) == 0)
    { 
      //
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;

      processInput(window);
      glfwSetScrollCallback(window, scroll_callback); 
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

      //Matrices to be used
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      glm::mat4 model = glm::mat4(1.0f);

      sunShader.use();

      model = glm::mat4(1.0f);
      model=  glm::translate(model,sunPos);

      sunShader.setMat4("model", model);
      sunShader.setMat4("projection", projection);
      sunShader.setMat4("view", view);

      sun.draw();
      
      model = glm::mat4(1.0f);
      model = glm::translate(model, earthPos);
//      glm::mat4 modelMatrix = glm::vec3(90,0,0);


      earth1.use();

      earth1.setMat4("model", model);
      earth1.setMat4("projection", projection);
      earth1.setMat4("view", view);


      glm::vec4 eyePosW = glm::vec4(camera.Position, 1 );
      earth1.setVec4("EyePosW",eyePosW);

      earth1.setVec4("LightPosW",glm::vec4(sunPos,1));

      earth1.setVec4("LightColor",white);

      earth1.setVec4("MaterialEmissive",black);
      earth1.setVec4("MaterialDiffuse",white);
      earth1.setVec4("MaterialSpecular",white);
      earth1.setFloat("MaterialShininess",50.0f);

      earth.draw();

      glDepthFunc(GL_LEQUAL);
      skyShader.use();
      view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
      skyShader.setMat4("view", view);
      skyShader.setMat4("projection", projection);
      glBindVertexArray(skyVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth function back to default

      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  earth.exit();
  sun.exit();

  glfwTerminate();
  return 0;
}
