#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <Tool/Shader.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include <Tool/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<Tool/Camera.h>

void frambuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);        // xpos,ypos 为鼠标当前的位置
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset); // xoffset 与yoffset为当前帧和上一帧的偏移量
void processInput(GLFWwindow *window);
float deltaTime = 0.0f;                                // 当前帧与上一帧的时间差
float lastFrame = 0.0f;                                //  上一帧的时间
bool firstMouse = true;
Camera camera(glm::vec3(0.0f,0.0f,3.0f));

const float SWIDTH = 800.0f;
const float SHEIGHT = 600.0f;

float lastX = SWIDTH / 2.0f; // 鼠标上一帧的位置
float lastY = SHEIGHT / 2.0f;

float fov = 45.0f;          //视角极限


int main()
{
    glfwInit();                                    //初始化
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //设置主要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //设置次要版本

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置OpenGL模式为核心模式
    /*//@Camera的定义与操作

    glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);        //确定摄像机的位置向量

    glm::vec3 cameraTarget = glm::vec3(0.0f,0.0f,0.0f);     //确定摄像机的方向

    //通过摄像机的方向和位置可以得到相机到原点的方向，也就得到了一个坐标轴
    glm::vec3 cameraDirection = glm::normalize(cameraPos-cameraTarget);


    //通过定义上向量得到右轴

    glm::vec3 Position = glm::vec3(0.0f,1.0f,0.0f);

    glm::vec3 cameraRight = glm::normalize(glm::cross(Position,cameraDirection));

    //通过右轴和前轴可以得到上轴
    glm::vec3 cameraPosi  tioPosition = glm::cross(cameraDirection,cameraRight);
*/

    //创建LookAt矩阵
    /*glm::mat4 View = glm::mat4(1.0f);

    View = glm::lookAt(glm::vec3(0.0f,0.0f,3.0f),
                        glm::vec3(0.0f,0.0f,0.0f),
                        glm::vec3(0.0f,1.0f,0.0f));


    float radius = 10.0f;
    float iTime = glfwGetTime();

    float camX = sin(iTime)*radius;
    float camZ = cos(iTime)*radius;

    View = glm::lookAt(glm::vec3(camX,0.0,camZ),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));*/

    /*
    LookAt:

    LookAt =
    {                               {
        Rx  Ry  Rz  0                   1   0   0   -Px
        Ux  Uy  Uz  0       *           0   1   0   -Py
        Dx  Dy  Dz  0                   0   0   1   -Pz
        0   0   0   1                   0   0   0    1
    }                               }

    R是右向量
    U是上向量
    D是方向向量
    P是cameraPos向量


    */

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //@创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Shader", NULL, NULL); //创建窗口宽高，名字
    if (window == NULL)                                                          //如果未创建成功，则打印失败，结束窗口并返回
    {
        std::cout << "Fail" << std::endl;
        glfwTerminate();
        return -1;
    }

    //创建各种摄像机回调函数的对象
    glfwMakeContextCurrent(window); //创建成功，得到一个窗口对象
    glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    //@初始化glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //由于glad管理OpenGL函数指针，使用使用前都需要初始化一遍
    {
        std::cout << "Fail to init GLAD" << std::endl; //如果初始化失败，就打印并退出
        return -1;
    }

    //@调整视口大小,这样OpenGL才只能知道怎样将位置坐标转化为屏幕坐标
    glViewport(0, 0, 800, 600); //调整窗口维度
    //前面两个参数表示的是窗口左下角的位置，后两个是宽和高

    glEnable(GL_PROGRAM_POINT_SIZE);
    //@注册窗口监听函数，当窗口大小发生变换时，会调用这个函数，也就调用回调函数
    glfwSetFramebufferSizeCallback(window, frambuffer_size_callback);

    glEnable(GL_DEPTH_TEST); // Z-Buffer缓冲

    Shader ourShader("D:\\opengl\\code\\NewCode\\09_Hello_CameraClass\\src\\shader\\vertex.glsl", "D:\\opengl\\code\\NewCode\\09_Hello_CameraClass\\src\\shader\\fragment.glsl");

    //@定义顶点数组，添加颜色
    float vertices1[] = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    //@创建vbo顶点缓冲对象来储存大量顶点
    unsigned int VAO, VBO, EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    /*索引缓冲对象和VBO（顶点缓冲对象）相似，一个实存储顶点信息
一个是管理顶点信息*/

    //@绑定VAO对象
    glBindVertexArray(VAO);

    //@绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //@填充VBO数据到缓冲里面
    glBufferData(GL_ARRAY_BUFFER /*缓冲类型*/, sizeof(vertices1) /*开辟多少内存存储*/, vertices1 /*数据*/, GL_STATIC_DRAW); //这个时候才真正吧数组数据传到了缓冲区
    /*
        告诉显卡如何管理给定的数据：
        GL_STATIC_DRAW ：数据不会或几乎不会改变。
        GL_DYNAMIC_DRAW：数据会被改变很多。
        GL_STREAM_DRAW ：数据每次绘制时都会改变。
    */

    //设置顶点属性指针
    glVertexAttribPointer(0 /*硬代码的location决定，相当于target*/, 3 /*顶点属性是一个vector几的量*/, GL_FLOAT /*float类型*/, GL_FALSE /*不被标准化*/, 5 * sizeof(float) /*步长*/, (void *)0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(0); //调用VBO
    glEnableVertexAttribArray(2);
    /*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    |        0. 复制顶点数组到缓冲中供OpenGL使用
    |    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    |    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    |        1. 设置顶点属性指针
    |    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    |    glEnableVertexAttribArray(0);
    |        2. 当我们渲染一个物体时要使用着色器程序
    |    glUseProgram(shaderProgram);
    |        3. 绘制物体
    |    someOpenGLFunctionThatDrawsOurTriangle();
    |
    ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    //暂时解绑VAO
    glBindVertexArray(0);
    stbi_set_flip_vertically_on_load(true); // y轴翻转
    unsigned int texture1, texture2;
    glGenTextures(1, &texture1); //通过ID来GEN一个对象

    glBindTexture(GL_TEXTURE_2D, texture1); //绑定texture

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("D:/opengl/code/NewCode/09_Hello_CameraClass/src/sources/dio.png", &width, &height, &nrChannels, 0);
    std::cout << width << " " << height << " " << nrChannels << " " << std::endl;
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "读取文件失败" << std::endl;
    }

    /*
    第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
    第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
    第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
    第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
    下个参数应该总是被设为0（历史遗留的问题）。
    第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
    最后一个参数是真正的图像数据。

    */
    stbi_image_free(data);

    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("D:\\opengl\\code\\NewCode\\09_Hello_CameraClass\\src\\sources\\awesomeface.png", &width, &height, &nrChannels, 0);
    std::cout << width << " " << height << " " << nrChannels << " " << std::endl;

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Fail to get picture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    //@开启线框模式（非必须）
    // glPolygonMode(GL_Target_AND_BACK, GL_TRIANGLES);

    /*GL_FILL是全绘制，而GL_LINE就是绘制线框,同理，GL_POINT就是绘制点*/
    /*  1.uniform的位置值
        2.要传送多少个矩阵
        3.是否需要矩阵转置
        4.传递矩阵数据，把数据变成OpenGL所接受的
    */


    //@通过循环持续渲染
    while (!glfwWindowShouldClose(window)) //检查用户是否需要退出
    {
        //输入
        processInput(window);
        //渲染指令

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);                       //清除颜色缓冲，其他的还有深度缓冲盒模板缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除Z-Buffer缓冲

        // glBindTexture(GL_TEXTURE_2D,texture);
        ourShader.use();

        // ourShader.ChangeColor("Color");
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        unsigned int transformModel = glGetUniformLocation(ourShader.ID, "model");
        unsigned int transformView = glGetUniformLocation(ourShader.ID, "view");
        unsigned int transformProject = glGetUniformLocation(ourShader.ID, "project");
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-65.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        float iTime = glfwGetTime();

        //相机移动速度控制

        /*图形程序和游戏通常会跟踪一个时间差(Deltatime)变量，它储存了渲染上一帧所用的时间。
        我们把所有速度都去乘以deltaTime值。结果就是，如果我们的deltaTime很大，就意味着上一帧的渲染花费了更多时间，
        所以这一帧的速度需要变得更高来平衡渲染所花去的时间。使用这种方法时，无论你的电脑快还是慢，摄像机的速度都会相应平衡，
        这样每个用户的体验就都一样了*/

        //在每一帧中我们计算出新的deltaTime以备后用。
        deltaTime = lastFrame - iTime;
        lastFrame = iTime;

        glm::mat4 project = glm::mat4(1.0f);
        project = glm::perspective(glm::radians(fov), (float)width / (float)height, 0.1f, 100.0f);
        glUniformMatrix4fv(transformProject, 1, GL_FALSE, glm::value_ptr(project));

        glm::mat4 view = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        glUniformMatrix4fv(transformView, 1, GL_FALSE, glm::value_ptr(view));

        glBindVertexArray(VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            unsigned int transformLoc = glGetUniformLocation(ourShader.ID, "transform");

            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::rotate(trans, glm::radians(iTime * 30.0f), glm::vec3(1.0f, 1.0f, 1.0f));

            glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            glUniformMatrix4fv(transformModel, 1, GL_FALSE, glm::value_ptr(model));
            

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window); //对这个窗口进行一个颜色的双缓冲，避免单缓冲造成照片闪烁
        glfwPollEvents();        //监听窗口是否发生事件，若发生，则会调用相应的回调函数
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //@结束渲染
    glfwTerminate();
    return 0;
}

//回调函数
//用户调整窗口大小时，会调整视口，这个函数就可以重新调整视口大小为需要的大小
void frambuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

//输入的回调函数
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //如果按下SPACE，退出渲染
        glfwSetWindowShouldClose(window, true);           //给ShouleCLose传true值，并在渲染循环中更新
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(Backward,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(Forward,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(Right,deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(Left,deltaTime);
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow *window, double xposIn, double yposIn)
{

    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if(firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;// reversed since y-coordinates go from bottom to top
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; 
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}