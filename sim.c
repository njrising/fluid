// lid driven cavity flow simulation
// using fdm to generate realtime fluid simulation
// author - Nathan Rising
// last revision - 01/23/2021 

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
GLuint vao;

// initialize velocity and pressure mesh
float u[600][600] = {};
float v[600][600] = {};
float p[600][600] = {};


const GLchar *fragment_shader = "\
#version 120\n\
varying vec4 fragmentColor;\
void main(){\
    gl_FragColor = fragmentColor;\
}\
";



const GLchar *vertex_shader = "\
#version 120\n\
attribute vec4 in_color;\
varying vec4 fragmentColor;\
void main(){\
    gl_Position = vec4(0.0,0.0,0.0,1.0);\
    fragmentColor = in_color;\
}\
";

void init(void) 
{
//initialize glfw
glfwInit();
glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,1.0);
//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,2);
//glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_ANY_PROFILE);
glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);

window = glfwCreateWindow(600,600,"A Most Excellent Window",0,0);
if(!window){glfwTerminate();std::cout<<"Could not create window \n";}

glfwMakeContextCurrent(window);

// initialize GLEW
glewExperimental = GL_TRUE;
if(glewInit() != GLEW_OK){std::cout<<"Could initialize GLEW \n";};

glViewport(0,0,600,600);

// print some graphics information
std::cout<<"Vendor: "<<glGetString(GL_VENDOR)<<std::endl;
std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<std::endl;
std::cout<<"Version: "<<glGetString(GL_VERSION)<<std::endl;
std::cout<<"Shading language: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<std::endl;
// create and compile vertex shader
GLuint vertexshader = glCreateShader(GL_VERTEX_SHADER);
glShaderSource(vertexshader,1,&vertex_shader,0);
glCompileShader(vertexshader);
GLint s_compile;
GLchar infolog[300];
glGetShaderiv(vertexshader,GL_COMPILE_STATUS,&s_compile);
if(!s_compile)
{
    glGetShaderInfoLog(vertexshader,300,0,infolog);
    std::cout<<"vertex shader compilation failed\n";
    std::cout<<infolog<<'\n';
}
// create and compile fragment shader
GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
glShaderSource(fragmentshader,1,&fragment_shader,0);
glCompileShader(fragmentshader);
glGetShaderiv(fragmentshader,GL_COMPILE_STATUS,&s_compile);
if(!s_compile){
    glGetShaderInfoLog(fragmentshader,300,0,infolog);
    std::cout<<"fragment shader compilation failed\n";
    std::cout<<infolog<<"\n";
}
// link shaders
GLuint shaderProgram = glCreateProgram();
glAttachShader(shaderProgram,vertexshader);
glAttachShader(shaderProgram,fragmentshader);
glLinkProgram(shaderProgram);
glGetProgramiv(shaderProgram,GL_LINK_STATUS,&s_compile);
if(!s_compile)
{
    glGetProgramInfoLog(shaderProgram,300,0,infolog);
    std::cout<<"linking shaders failed\n";
    std::cout<<infolog<<'\n';
}

glUseProgram(shaderProgram);
glDeleteShader(vertexshader);
glDeleteShader(fragmentshader);
// generate buffer
glGenVertexArrays(1,&vao);
glBindVertexArray(vao);


}

int main(int argv,char* argc[]){
// initialize environment
init();

// set boundary conditions x velocity to zero
for(int i = 0;i<600;++i)
    u[599][i] = 1.0;



while(!glfwWindowShouldClose(window))
{
    glfwPollEvents();
    
    glClearColor(0.0,0.0,0.1,1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwSwapBuffers(window);
}
glfwTerminate();

//std::cout<<"Hello Nathan!"<<std::endl;
//std::cout<<argv<<std::endl;

return 0;
}










