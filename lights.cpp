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

glm::vec3 sunPos(-10.0f, -10.0f, -10.0f);
glm::vec3 sunDir(10.0f, 10.0f, 10.0f);

glm::vec3 earthPos(1.0f, 1.0f, 1.0f);

Shader earthShader("Shader/earth_vs.glsl","Shader/earth_fs.glsl");
Shader sunShader("Shader/sun_vs.glsl","Shader/sun_fs.glsl");

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

earthShader.use();
earthShader.setInt("material.diffuse", 0);
earthShader.setInt("material.specular", 1);
earthShader.setInt("material.norm", 2);
earthShader.setFloat("material.shininess", 0.0f);


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
      
      model = glm::mat4(1.0f);
      model = glm::translate(model, earthPos);
          
      earthShader.use();

      earthShader.setMat4("model", model);
      earthShader.setMat4("projection", projection);
      earthShader.setMat4("view", view);
      earthShader.setVec3("direction", sunDir);
      earthShader.setVec3("cpos", camera.Position);
      earthShader.setVec3("ligpos", camera.Position);

      // directional light
      earthShader.setInt("dirLight.status",dirLight);
      earthShader.setVec3("dirLight.direction", sunDir);
      earthShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
      earthShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
      earthShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


      // spotLight
      earthShader.setInt("spotLight.status", spotLight);
      earthShader.setVec3("spotLight.position", camera.Position);
      earthShader.setVec3("spotLight.direction", camera.Front);
      earthShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
      earthShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
      earthShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
      earthShader.setFloat("spotLight.constant", 1.0f);
      earthShader.setFloat("spotLight.linear", 0.09);
      earthShader.setFloat("spotLight.quadratic", 0.032);
      earthShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      earthShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     
      
      earthShader.setVec3("viewPos", camera.Position);

      earthShader.setInt("blinn", blinn);
      earthShader.setInt("np", np);

      earth.draw();

      sunShader.use();

      model = glm::mat4(1.0f);
      model = glm::translate(model, sunPos);
      model = glm::scale(model, glm::vec3(0.5f));

      sunShader.setMat4("model", model);

      sunShader.setMat4("projection", projection);
      sunShader.setMat4("view", view);

      sun.draw();


      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  earth.exit();
  sun.exit();

  glfwTerminate();
  return 0;
}
