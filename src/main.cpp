#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include<Tool/Shader.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include<Tool/stb_image.h>

void frambuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
int main()
{
    glfwInit();                                    //初始化
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //设置主要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //设置次要版本

    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //设置OpenGL模式为核心模式

    //@创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "Hello Shader", NULL, NULL); //创建窗口宽高，名字
    if (window == NULL)                                                            //如果未创建成功，则打印失败，结束窗口并返回
    {
        std::cout << "Fail" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); //创建成功，得到一个窗口对象

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
    Shader ourShader("D:\\opengl\\code\\NewCode\\04_Hello_Texture\\src\\shader\\vertex.glsl","D:\\opengl\\code\\NewCode\\04_Hello_Texture\\src\\shader\\fragment.glsl");
    
    //@定义顶点数组，添加颜色
    float vertices1[] = {
//     ---- 位置 ----   ---- 颜色 ----     - 纹理坐标 -
      0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // 右上
      0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 右下
      -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0, 0.0f, // 左下
      -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // 左上
    };

    unsigned int indices[]={
        0,1,3,
        1,2,3
    };

   

    //@创建vbo顶点缓冲对象来储存大量顶点
    unsigned int VAO ,VBO,EBO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&EBO);


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

    //填充EBO数据到缓冲
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    
    //设置顶点属性指针
    glVertexAttribPointer(0 /*硬代码的location决定，相当于target*/, 3 /*顶点属性是一个vector几的量*/, GL_FLOAT /*float类型*/, GL_FALSE /*不被标准化*/, 8 * sizeof(float) /*步长*/, (void *)0);
    glVertexAttribPointer(1 /*硬代码的location决定，相当于target*/, 3 /*顶点属性是一个vector几的量*/, GL_FLOAT /*float类型*/, GL_FALSE /*不被标准化*/, 8 * sizeof(float) /*步长*/, (void *)(3*sizeof(float)));
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void *)(6*sizeof(float)));
    glEnableVertexAttribArray(0); //调用VBO
    glEnableVertexAttribArray(1);
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

    unsigned int texture;
    glGenTextures(1,&texture);      //通过ID来GEN一个对象

    glBindTexture(GL_TEXTURE_2D,texture);   //绑定texture



    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width,height,nrChannels;
    unsigned char *data = stbi_load("D:/opengl/code/NewCode/04_Hello_Texture/src/sources/container.jpg",&width,&height,&nrChannels,0);
    std::cout<<width<<" "<<height<<" "<<nrChannels<<" "<<std::endl;
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout<<"读取文件失败"<<std::endl;
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


    //@开启线框模式（非必须）
    glPolygonMode(GL_FRONT_AND_BACK, GL_TRIANGLES);
    /*GL_FILL是全绘制，而GL_LINE就是绘制线框,同理，GL_POINT就是绘制点*/
   

    //@通过循环持续渲染
    while (!glfwWindowShouldClose(window)) //检查用户是否需要退出
    {
        //输入
        processInput(window);
        //渲染指令

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT); //清除颜色缓冲，其他的还有深度缓冲盒模板缓冲
        
       
        glBindTexture(GL_TEXTURE_2D,texture);
         ourShader.use();
        //ourShader.ChangeColor("Color");
        
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0); //实心三角形
        

        //检查并调用事件，交换缓冲
        glfwSwapBuffers(window); //对这个窗口进行一个颜色的双缓冲，避免单缓冲造成照片闪烁
        glfwPollEvents();        //监听窗口是否发生事件，若发生，则会调用相应的回调函数
    }
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1,&EBO);
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
    {
        glfwSetWindowShouldClose(window, true); //给ShouleCLose传true值，并在渲染循环中更新
    }
}

