// lid driven cavity flow simulation
// using fdm to generate realtime fluid simulation
// author - Nathan Rising
// last revision - 01/23/2021 
// Testing...


#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* window;
GLuint vao;


//screen resolution set at 600 X 600;
float scr_w = 600;
float scr_h = 600;
// box dimensions
float len_x = 1.0;
float len_y = 1.0;
// grid spacing
float dx = len_x/(scr_w - 1.0);
float dy = len_y/(scr_h - 1.0);
// initialize velocity and pressure
float u[600][600] = {};
float v[600][600] = {};
float p[600][600] = {};
// node x and y ordinates
float x[600][600] = {};
float y[600][600] = {};
// time increment
float dt = 0.1;

// fluid properties pressure and viscosity
float rho = 1.0;
float vis = 0.1;


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
for(int i = 0;i<50;++i)
    u[49][i] = 1.0;

int var_test = 5;
while(!glfwWindowShouldClose(window))
{
    glfwPollEvents();
    
    //-----------Start Simulation    
    // testing...
    while(var_test != 0){ 
    var_test--;
    std::cout<<var_test<<'\n';
   
    for(int i = 1;i < 50;++i)
    {
        for(int j = 1;j < 50;++j)
        {
            float t1 = u[i][j] - u[i][j]*(u[i][j] - u[i-1][j]) * (dt/dx); 
            float t2 = -v[i][j]*(u[i][j] - u[i][j-1]) * (dt/dy); 
            float t3 = -(1.0/rho)*(p[i+1][j] - p[i-1][j]) * (dt/(2*dx));
            float t4 = vis * ( (u[i+1][j] - 2*u[i][j] + u[i-1][j])*(dt/(dx*dx)) ); 
            float t5 = vis * ( (u[i][j+1] - 2*u[i][j] + u[i][j-1])*(dt/(dy*dy)) );
            u[i][j] + t1 + t2 + t3 + t4 + t5;
            
            float s1 = v[i][j] - u[i][j]*(v[i][j] - v[i-1][j]) * (dt/dx); 
            float s2 = -v[i][j]*(v[i][j] - v[i][j-1]) * (dt/dy); 
            float s3 = -(1.0/rho)*(p[i+1][j] - p[i-1][j]) * (dt/(2*dy));
            float s4 = vis * ( (v[i+1][j] - 2*v[i][j] + v[i-1][j])*(dt/(dx*dx)) ); 
            float s5 = vis * ( (v[i][j+1] - 2*v[i][j] + v[i][j-1])*(dt/(dy*dy)) ); 
            v[i][j] + s1 + s2 + s3 + s4 + s5;
                
            float b1 = ( (p[i+1][j] + p[i-1][j]) * (dy*dy) + (p[i][j+1] + p[i][j-1]) * (dx*dx) )/(2*(dx*dx+dy*dy));
            float b2 = ( (rho * dx*dx*dy*dy)/(2*dt*(dx*dx+dy*dy)) ) * ( (u[i+1][j] - u[i-1][j])/(2*dx) +(v[i][j+1] - v[i][j-1])/(2*dy*dy) );  
            float b3 = ( (rho * dx*dx*dy*dy)/(2*(dx*dx+dy*dy)) ) * ( ( (u[i+1][j] - u[i-1][j])/(2*dx) )*( (u[i+1][j] - u[i-1][j])/(2*dx) ) );
            float b4 = ( (rho * dx*dx*dy*dy)/(2*(dx*dx+dy*dy)) ) * ( 2 * ( (u[i][j+1] - u[i][j-1])/(2*dy) ) * ( (v[i+1][j] - v[i-1][j])/(2*dx) ) );
            float b5 = ( (rho * dx*dx*dy*dy)/(2*(dx*dx+dy*dy)) ) * ( ( (v[i][j+1] - v[i][j-1])/(2*dy) )*( (v[i][j+1] - v[i][j-1])/(2*dy) ) );    
            p[i][j] = b1 + b2 + b3 + b4 + b5;
        }
    }
    
    for(int k = 0;k<50;++k)
    {
        for(int l = 0;l<50;++l)
        {
            std::cout<<u[k][l]<<" "; 
        }
        std::cout<<'\n';
    }
    
    std::cout<<"\n";   
						
   }
    //----------End Simulation
    
    glClearColor(0.0,0.0,0.1,1.0); 
    glClear(GL_COLOR_BUFFER_BIT);
    
    glfwSwapBuffers(window);
}
glfwTerminate();

//std::cout<<"Hello Nathan!"<<std::endl;
//std::cout<<argv<<std::endl;

return 0;
}










