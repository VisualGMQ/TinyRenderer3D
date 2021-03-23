# TinyRenderer3D

一个小型的3D渲染器，主要是在学习OpenGL时候做的实践玩具🤣，可以内嵌于任何支持OpenGL的环境中。

## 功能

*   模型加载和渲染
*   三种光照：平行光，点光源，聚光
*   阴影（目前只有平行光有）
*   天空盒子
*   纹理加载和渲染，支持漫反射纹理，法线纹理，镜面光纹理和放射光纹理
*   允许手动配置纹理和材质的多边形几何体

## 如何运行测试

在开始使用前你可能想要运行一下看看效果。在`test`文件夹下是测试文件，里面包含了渲染器各个功能的测试。

需要用到的第三方库：

*   GLEW：用于引导OpenGL
*   SDL：用于提供窗口平台
*   Assimp：用于加载模型
*   glm：用于数学运算

在`根目录`下使用`make`编译：

```bash
make
```

然后**直接在根目录运行测试**：

```bash
./test/TestClearScreen
```

## 如何使用

### 需要包含的文件

所有的文件在`include`文件夹下，源代码在`src`下，别忘记将`shader`文件夹复制到你的工程以便于渲染器找到着色器。

### 创建和删除渲染器

首先你必须配置好OpenGL的环境（包括窗口环境和调用`glewInit()`函数），然后使用`CreateRenderer()`函数创建渲染器：

```c++
tinyrenderer3d::Renderer* render = CreateRenderer(WindowWidth, WindowHeight);
```

不要忘记在程序退出时删除：

```c++
tinyrenderer3d::DestroyRenderer(render);
```

由于这只是个渲染器而不是个引擎，所以他**不会**帮你自动删除任何你使用`CreateXXX`函数创建出来的东西，你必须使用相对应的`DestroyXXX()`函数来释放你创建出来的东西。

### 清屏

然后你可以使用`render->SetClearColor()`和`render->Clear()`来进行清屏。



###  绘制一个平面

Tinyrenderer3D允许你给入顶点，材质来自己配置一个几何体，这里以绘制平面举例（详情可见[test/TestDrawMesh.cpp](test/TestDrawMesh.cpp)）

首先所有的几何体（包括模型导入的几何体）都是`Mesh`类型，通过实例化Mesh类和配置其中的信息，我们可以自定义一个几何体：

```c++
const vector<Vertex> vertices = {  // 几何体的顶点属性
    {{ 0.5f, 0.0f,  0.5f},  {1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    {{ 0.5f, 0.0f, -0.5f},  {1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.0f, -0.5f},  {0.0f, 1.0f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.0f,  0.5f},  {0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
};

const vector<uint> indices = {  // 几何体顶点的一个索引
    0, 1, 2,
    0, 2, 3
};

plane_.center = {0, -1, 0}; // 几何体的中心坐标
plane_.vertices = vertices; // 将顶点传入
plane_.indices = indices;   // 将索引传入
plane_.scale = {10, 1, 10}; // 设置缩放

// 接下来设置材质
plane_.material.ambient = {0.6, 0.6, 0.6};
plane_.material.specular = {0.6, 0.6, 0.6};
plane_.material.shininess = 32;

// 如果有纹理的话设置纹理，就不需要设置相关材质了
plane_.material.diffuse_texture = texture_;
```

`vertices`是几何体的顶点，`Vertex`类型一共有五个属性：

*   `position`：顶点的位置
*   `texcoord`：如果有纹理的话，纹理坐标
*   `normal`：顶点的法向量
*   `tangent`：如果有法线贴图的话，法线贴图的切向量
*   `bitangent`：如果偶发现贴图的话，法线贴图的副切向量

如果你不需要其中的某些信息可以不设置，但是一定要设置`position`,`texcoord`,`normal`。

`indices`指定了顶点索引，如果你没有索引也可以不给（让`plane_.indices`置空即可）。

然后是几何体的设置和材质的设置。

材质方面除了可以直接给出颜色之外（颜色在0到1之间），还允许有纹理。存在

*   漫反射纹理`material.diffuse_texture`
*   法线纹理`material.normal_texture`
*   镜面光纹理`material.specular_texture`
*   放射光纹理`material.emission_texture`

如果不需要设置为`nullptr`即可。设置了贴图就不需要设置相关的颜色了。



当你设置好所有的信息后，通过`render->AddObject(&plane_)`将其放入渲染器中渲染：

```c++
render->AddObject(&plane_);
```



别忘记使用`Draw()`函数将物体绘制到屏幕上：

```c++
render->Draw();
```



这是测试用例的效果，展示了有贴图和没贴图的两种情况：

<img src="https://i.loli.net/2021/03/23/H9Fn1vOIQ85bNwE.png" alt="drawMesh" style="zoom: 50%;" />

###  增加一些光源

默认场景中存在一个平行光，其会以`(1, 1, 1)`的全局光分量，向着`(0, -1, -1)`的方向照射（就是最亮的全局光），这会导致场景中的所有物体都被照到最亮。

如果你想要自己设置光线，请通过实例化`LightSet`类并且使用`render->SetLights()`来设置，像这样（参考[test/TestLight.cpp](test/TestLight.cpp)）：

```c++
LightSet lights;

// 设置平行光，这里将平行光灯关掉以突显点光源和聚光的效果
lights.dirlight.SetAmbient(0, 0, 0);
lights.dirlight.SetDirection(0, 0, 0);
lights.dirlight.SetSpecular(0, 0, 0);

// 设置点光源
DotLight dotlight;
dotlight.SetPosition(1, 1, 1);
dotlight.SetAmbient(0, 0, 0);
dotlight.SetDiffuse(0.55, 0.55, 0.55);
dotlight.SetSpecular(0.55, 0.55, 0.55);

lights.dotlights.push_back(dotlight);

// 设置聚光
SpotLight spotlight;
spotlight.SetPosition(0, 1, 0);
spotlight.SetDirection(0, -1, 0);
spotlight.SetAmbient(0.8, 0.8, 0.8);
spotlight.SetDiffuse(0.8, 0.8, 0.8);
spotlight.SetSpecular(0.4, 0.4, 0.4);
spotlight.SetParameter(50, 40); // 设置聚光的外切角和内切角

lights.spotlights.push_back(spotlight);

// 设置新的光照
render->SetLights(lights_);
```



这里给出测试用例的效果：

<img src="https://i.loli.net/2021/03/23/UGA1uNxSvHM8ipw.png" alt="lights" style="zoom:50%;" />



场景上方一个聚光向下照射，`(1, 1, 1)`处有一个点光源（可以看到地板的远处和近处的亮度不一样，这是点光源导致的）。

绿色的那个是放射光贴图，可以自己发光，不受光照影响。

### 模型加载

通过实例化`Model`类型，并通过`LoadModel()`就可以了。别忘记通过`render->AddObject()`加入到render中。



### 天空盒子

增加天空盒的方法很简单，先通过`CubeMap::CubeMapTextureDatas`配置到天空盒的六个面，然后通过`CreateCubeMap()`得到一个CubeMap，然后通过`CreateSkyBox()`得到一个天空盒子即可（参考[test/TestSkybox.cpp](test/TestSkybox.cpp)）：

```c++
// 载入图像
CubeMap::CubeMapTextureDatas datas;
datas.negative_x = LoadImage("test_resources/skybox/left.jpg");
datas.negative_y = LoadImage("test_resources/skybox/bottom.jpg");
datas.negative_z = LoadImage("test_resources/skybox/back.jpg");
datas.positive_x = LoadImage("test_resources/skybox/right.jpg");
datas.positive_y = LoadImage("test_resources/skybox/top.jpg");
datas.positive_z = LoadImage("test_resources/skybox/front.jpg");

CubeMap* cubmap = CreateCubeMap(datas);
SkyBox* skybox = CreateSkyBox(cubmap);

// 创建Cubemap后，其会自动将图像放到显存，所以这里可以直接删了
DestroyImage(datas.negative_x);
DestroyImage(datas.negative_y);
DestroyImage(datas.negative_z);
DestroyImage(datas.positive_x);
DestroyImage(datas.positive_y);
DestroyImage(datas.positive_z);

// 设置天空盒子
render->SetSkybox(skybox);
```

你也可以通过`render->RemoveSkyBox()`移除当前的天空盒子。



这里是测试用例的效果：

![skybox](https://i.loli.net/2021/03/23/bYt1sDFBiXaOw3R.gif)



### 摄像机

render自带一个可以水平移动的摄像机。通过`render->GetCamera()`得到他，并且在你的事件处理中对其视角进行处理（详情见[test/test_framework.hpp](test/test_framework.hpp)中的`MainLoop`函数)。

## 其他的效果展示

法线贴图:

![normal map](https://i.loli.net/2021/03/23/5dtmhpDeFbWaoXJ.gif)

模型加载：

<img src="https://i.loli.net/2021/03/23/TV9FaD7Rl1YHSJ4.png" alt="model" style="zoom:50%;" />