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
glm::vec3 earthPos(1.0f, 1.0f, 1.0f);

Shader lampShader("Shader/light_vs.glsl","Shader/light_fs.glsl");
Sphere sphere(2);
Sphere sun(1);

unsigned int loadTexture(const char *path);
unsigned int Map= loadTexture("Texture/earth.jpg");
unsigned int sun_tex=loadTexture("Texture/sun.jpg");

sphere.texture(Map);
sun.texture(sun_tex);

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
          
      lampShader.use();

      lampShader.setMat4("model", model);
      lampShader.setMat4("projection", projection);
      lampShader.setMat4("view", view);

      sphere.draw();


      model = glm::mat4(1.0f);
      model = glm::translate(model, sunPos);
      model = glm::scale(model, glm::vec3(0.5f));

      lampShader.setMat4("model", model);


      sun.draw();


      glfwSwapBuffers(window);
      glfwPollEvents();
    }
  sphere.exit();
  sun.exit();

  glfwTerminate();
  return 0;
}
