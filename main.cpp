#include "shaders.h"

#include "solarSystemLogik.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <fstream>
#include <boost/numeric/odeint.hpp>
#include <unistd.h>
#include "point_type.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <chrono>



void processInput(GLFWwindow *window, float &distanceCamera, float &angleRotateXachsis,  double &gameTimeFactor )
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        distanceCamera += 1.0f; // change this value accordingly (might be too slow or too fast based on system hardware)
        
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        distanceCamera -= 1.0f; // change this value accordingly (might be too slow or too fast based on system hardware)
    }
     if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        angleRotateXachsis -= 1.0f; // change this value accordingly (might be too slow or too fast based on system hardware)
        
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        angleRotateXachsis += 1.0f; // change this value accordingly (might be too slow or too fast based on system hardware)
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        gameTimeFactor -= 10.0; // change this value accordingly (might be too slow or too fast based on system hardware)
        
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        gameTimeFactor += 10.0f; // change this value accordingly (might be too slow or too fast based on system hardware)
    }
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}



int main( int argc , char **argv )
{

    using namespace std;
    using namespace boost::numeric::odeint;

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    int SCR_width = 1000;
    int SCR_height = 800;
    GLFWwindow* window = glfwCreateWindow(SCR_width, SCR_height, "solar system", nullptr, nullptr); // Windowed
    //GLFWwindow* window2 = glfwCreateWindow(800, 600, "OpenGL", glfwGetPrimaryMonitor(), nullptr); // Fullscreen
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    framebuffer_size_callback(window, SCR_width, SCR_height);
    glewExperimental = GL_TRUE;

    glewInit();    
    
     // build and compile our shader program
    // ------------------------------------
    Shaders ourShader("shader.vs", "shader.fs"); // you can name your shader files however you like

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  

        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f, -0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  

         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  

        -0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f, -0.5f,  
         0.5f, -0.5f,  0.5f,  
         0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f,  0.5f,  
        -0.5f, -0.5f, -0.5f,  

        -0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f, -0.5f,  
         0.5f,  0.5f,  0.5f,  
         0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f,  0.5f,  
        -0.5f,  0.5f, -0.5f
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glEnable(GL_DEPTH_TEST); 
    // -------------------------------------------------------------------------------------------
    ourShader.use();




    mass_type masses = {{
            1.00000597682 ,      // sun
            0.000954786104043 ,  // jupiter
            0.000285583733151 ,  // saturn
            0.0000437273164546 , // uranus
            0.0000517759138449 , // neptune
            1.0 / ( 1.3e8 )      // pluto
    }};

    container_type q = {{
            point_type( 0.0 , 0.0 , 0.0 ) ,                        // sun
            point_type( -3.5023653 , -3.8169847 , -1.5507963 ) ,   // jupiter
            point_type( 9.0755314 , -3.0458353 , -1.6483708 ) ,    // saturn
            point_type( 8.3101420 , -16.2901086 , -7.2521278 ) ,   // uranus
            point_type( 11.4707666 , -25.7294829 , -10.8169456 ) , // neptune
            point_type( -15.5387357 , -25.2225594 , -3.1902382 )   // pluto
    }};

    container_type p = {{
            point_type( 0.0 , 0.0 , 0.0 ) ,                        // sun
            point_type( 0.00565429 , -0.00412490 , -0.00190589 ) , // jupiter
            point_type( 0.00168318 , 0.00483525 , 0.00192462 ) ,   // saturn
            point_type( 0.00354178 , 0.00137102 , 0.00055029 ) ,   // uranus
            point_type( 0.00288930 , 0.00114527 , 0.00039677 ) ,   // neptune
            point_type( 0.00276725 , -0.00170702 , -0.00136504 )   // pluto
    }};



    point_type qmean = center_of_mass( q , masses );
    point_type pmean = center_of_mass( p , masses );
    for( size_t i=0 ; i<n ; ++i )
    {
        q[i] -= qmean ;
        p[i] -= pmean;
    }

    for( size_t i=0 ; i<n ; ++i ) p[i] *= masses[i];

    


    /*std::ofstream outfile2;
    outfile2.open("solar_system2.dat");*/
    symplectic_rkn_sb3a_mclachlan< container_type > rkn;
    auto a = chrono::high_resolution_clock::now();
    double gameTimeFactor = 5000;
    double gameDeltaTime;
    double gameTime = 0;
    double targetFrameTime = 0.1;
    float distanceCamera = 45.0;
    float angleRotateXachsis = 0.0;
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window, distanceCamera, angleRotateXachsis, gameTimeFactor);

        double realDeltaTime = chrono::duration<double>(chrono::high_resolution_clock::now() - a).count();
        a = chrono::high_resolution_clock::now();
        gameDeltaTime = realDeltaTime * gameTimeFactor;
        // Process inputs
        
        //update game world with gameDeltaTime
        rkn.do_step( make_pair( solar_system_coor( masses ) , solar_system_momentum( masses ) ) ,  make_pair( boost::ref( q ) , boost::ref( p ) )  , gameTime , gameDeltaTime);
        
        /*
        outfile2 << gameTime;
        for( size_t i=0 ; i<q.size() ; ++i ) outfile2 << "\t" << q[i];

        outfile2 << "\n";
*/
        gameTime += gameDeltaTime;
        // Render outputs
       // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!

        // activate shader
        ourShader.use();
        

        // create transformations
        glm::mat4 view          = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 projection    = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(distanceCamera), (float)SCR_width / (float)SCR_height, 0.1f, 100.0f);
        view       = glm::translate(view, glm::vec3(0.0f, 0.0f, -50.0f));
        view       = glm::rotate(view, glm::radians(angleRotateXachsis) , glm::vec3(1.0f , 0.0f, 0.0f));
        // pass transformation matrices to the shader
        ourShader.setMat4("projection", projection); // note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
        ourShader.setMat4("view", view);

        // render boxes
        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < n; ++i)
        {
            // calculate the model matrix for each object and pass it to shader before drawing
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model,glm::vec3(q[i][0], q[i][1], q[i][2]) );
            ourShader.setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }



         // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();

                
        if (chrono::duration<double>(chrono::high_resolution_clock::now() - a).count() < targetFrameTime - 0.01)
        {
            sleep(targetFrameTime - chrono::duration<double>(chrono::high_resolution_clock::now() - a).count());
        }
      
    }

     // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();


    //outfile2.close();

    return 0;

}

//
//
//This is a implementation of N body problem using boost's Runge-Kutta-Nystroem solver and visualized with openGL.