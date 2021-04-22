# KYM-PBRT-cpp

[TOC]
## 其他语言
[中文](README.md) [English](README.en.md)

## 说明

代码已上传[github](https://github.com/kymWorship/NJUpbrt-cpp/tree/master) master branch（master branch 为图像渲染部分，只是用了openmp）。

>  本来这两周计划测试不同bvh算法、实现photonmapping和GUI的，但是最近要去实验室，所以没空写也就算了。

## 渲染效果样例

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\02cylinder_20000spp.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图1 样例</div> </center>

## 系统构架

目前没有使用make/cmake，但都留了接口


│  background.h			背景光    
│  bound.h					即AABB，用于hit时的搜索  
│  camera.h					相机模块  
│  hitable.h					光线与物体相交模块，管理hitable文件夹中的各种形状  
│  log.txt						运行日志  
│  main.cpp					CLI 主函数  
│  main.exe					CLI编译程序  
│  material.h					材质模块，管理material文件夹各种材质  
│  picture.ppm				输出的ppm格式图片  
│  ray.h							光线Class  
│  README.md				  
│  sampler.h					取样模块，即Monte-Carlo，管理sampler文件夹采样函数  
│  scene.h						场景（目前全部写在一个文件中）  
│  vec3.h						3vector Class  
│        
├─accelerator				加速模块，目前只写了bvh加速，kd-tree和基于其上的photon-mapping还没来及写  
│  │  bvh.h						bvh Class  
│  │    
│  └─bvh_method	  
│          bvh_build.h			bvh 函数定义  
│          bvh_node.h			bvh使用的struct  
│            
├─gui							GUI界面，使用Qt5  
├─hitable						  
│      aabb.h						实际上使用的是bound.h  
│      cube.h						采用intersect方法写的cube（还可以使用三角面片等方法实现）  
│      cylinder.h					圆柱体  
│      hitable_list.h				其实应该叫做 Union，是hitable集合的并  
│      intersect.h					Intersect， hitable集合的交  
│      plane.h						无穷大平面  
│      sphere.h					球  
│      triangle.h					三角面片  
│        
├─loader							文件加载器  
│  │  myobjloader.h			自己写的c based obj/mtl 模型文件加载  
│  │  mypngloader.h			基于stb_image的图片加载器（理论上不仅支持png），本来尝试使用pnglib，但很笨重，最后决定直接用stb  
│  │    
│  └─external  
│          stb_image.h  
│            
├─material  
│      glass.h						玻璃  
│      lambertian.h				余弦体  
│      metal.h						金属  
│      source.h					光源  
│      texture.h					贴图  
│        
├─model							模型文件（目前仅支持obj/mtl，可以使用开源的blender转格式）  
│                
├─mylib								  
│      common.h					基本所有模块都要用到的基本头  
│      mycode.h					我coding是的一些习惯性宏或函数  
│      mylogo.h						Cli界面LOGO  
│      myrand.h						随机数生成器  
│      onb.h							onb 模块，用的不是太多  
│      settings.h					编译参数宏（利于使用什么算法，是否打开色散etc.）可以用cmake输入，根据需求进行编译  
│        
├─notes								笔记  
│      bvh.md					  
│      idea.md  
│        
├─pics									输出（各种测试）图片  
│  ├─png								png格式输出  
│  └─ppm								ppm格式输出  
│                 
├─sampler								  
│      cos_sampler.h				余弦分布采样  
│      hit_sampler.h					光源oriented采样  
│      mix_sampler.h				混合采样  
│        
└─test									测试模块  

## 交互界面

GUI使用Qt5，但是还没来及写好，下图是作为替代随便写的Cli交互界面

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\cli.PNG">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图2 CLI</div> </center>

## Hitable

#### 几何体

几何体实现都不复杂因为懒只实现了cube、sphere、plane、cylinder，其实使用牛顿下山等方法，任意曲面都很好实现。


- cylinder

  目前的实现在代码层面上有些暴力，可优化

- cube

  cube虽然有多种方法，但这里实现的其实更鲁棒一点，实际上实现的是平行六面体，比cube有更高的自由度。

#### 逻辑物体

hitable_list 和 Intersect实现的时物体组的交并

#### 三角面片

很简单（解$t,\lambda, \mu$的三元线性方程组即可）又很重要，不做赘述，值得一提的时obj文件是右手螺旋顺序，以及当下的实现尽可能将计算量预处理在生成三角面片时，但是会浪费存储（目前跑下来加上贴图和模型内存占用也并不大，所以不是什么问题）

如图二是加载了02模型加贴图，使用了bvh树，20000spp，8线程openmp的资源占用：

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\main-mission.PNG">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图3 任务管理器</div> </center>

## Material

#### 玻璃

##### 色散（dispersion）

玻璃的折射与投射都是很基础的内容，很容易实现，这里额外实现了色散（dispersion）效果，因为只是将光线在第一次发生色散时分为三个通道分别走rgb而已（和容易看到，光线可逆对色散也成立，所以可以这样做），复杂度增加是常数量级（<3），在settings.h种可以设置宏的值开关决定是否编译色散的部分，为了节省资源，一半会将色散关闭。

正常玻璃rgb折射率跨度只有约0.5，因此没必要用**Sellmeier equation**，不过为了让效果稍微明显点，图4使用了1.5-1.6的折射率跨度，注意物体的边缘有明显的色散效果。

##### 全反射

显然，全反射在球形玻璃中不会出现，但图4中立方体中可以看到明显的全反射。

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\disp_cube2.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图4 dispersion</div> </center>

#### 金属和余弦体

没什么好说的

#### 光源

光源目前使用的是球状面光源，三角面片的面光源也实现了，但并未使用，因为在Monte-Carlo中为了使用重要性采样，需要计算面光源的立体角，而三角面片的立体角无法在O(1)时间内精准计算，只能在足够远或三角面片足够小的近似下计算，所以有可能造成光源近处物体的失真（但是应该看不出来，而且只要三角面片够小也可以）

另外还有directed source，准备和photon-mapping一起实现（因为可以做出汇聚光的效果）

## Texture

Texture通过计算碰撞点相对贴图的uv坐标返回反射率或颜色，目前只有png和常数贴图（即单色），其他实现的必要似乎也不大。

只实现png的主要原因是png是无损压缩，大部分模型贴图使用的都是png格式。

下图5.1是02mainbody贴图，5.2是在室外的渲染效果，颜色比较淡的原因是使用了gamma修正，而图5.3则是关闭gamma修正的效果（都是100spp）。

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\model\zero-two-fully-rigged\textures\body_main.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图5.1 02 main body texture</div> </center>

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\100spp_outdoor_gamma_on.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图5.2 gamma on</div> </center>

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\100spp_outdoor_gamma_off.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图5.2 gamma on</div> </center>

## Camera

目前Camera只能设置FOV，LookFrom，LookAt，还不支持焦距，之所以不支持原因是现在只是用了纯Monte-Carlo，因此噪音已经很大了，如果加上焦距（和延迟摄影）效果，采样自由度又会增加，从而造成噪音不可控（具体可以参见sampler部分）。

但是只要利用焦平面和透镜组三基点（主点、节点、焦点）的性质，并在Class ray中加上时间自由度，景深和延迟摄影效果都很好实现。

## Sampler

采用了重要性采样的Monte-Carlo，简单而言就是让更多的光线打到亮的地方（例如光源，平面镜，玻璃），之后通过加权得到真实的亮度，这样有两点好处

1. shader更容易打到光源上终止，即shader深度降低，大大提升程序效率
2. 因为大部分sample光强比较大，这样绝对误差就会更小
3. 同样较低的采样率，使用monte-carlo显然会更亮（更接近实际情况）

特别是在黑暗的环境中，如果不用重要性采样的Monte-Carlo，场景就会一片黑暗

图6.1， 6.2是同样sample数下（300spp）使用与不使用重要性采样的对比（其中图6.2中墙上的影子是一个小bug，已经修复了）

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\300spp_no_mc.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图6.1 300spp不使用重要性采样的Monte Carlo</div> </center>

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\300spp_5-5mc.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图6.1 300spp使用重要性采样的Monte Carlo</div> </center>

但是Monte-Carlo这种纯采样机制，注定会噪声很大，图7是wiki上的对比，每张图都比前一张采样率翻倍，可以看到noise是Monte-Carlo无法避免且致命的缺陷。



<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\Path_tracing_sampling_values.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图7 Monte Carlo 噪声变化</div> </center>

#### 随机数生成

随机数生成对图片的噪声（显然）也有很大影响

图8.1和8.2是同样spp同一场景下使用不同随机数生成方法得到的结果（因为场景生成也用了随机数，所以场景也略有不同）

<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\rand_double_no_mc.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图8.1 随机数生成器1</div> </center>

``` c++

inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}
```



<center>    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\drand48.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图8.2 随机数生成器2</div> </center>

```c++
static double random_double()
{
	static std::random_device seed_gen;
	static std::mt19937 engine(seed_gen());
	static std::uniform_real_distribution<> dist(0.0, 1.0);
	return dist(engine);
}
```



#### Cos-sampler

按余弦分布采样，对于有环境光的场景（室外），以为余弦体就是余弦分布，这样采样采样率分布与实际贡献一致，误差最小

#### hit-sampler

像光源（或玻璃）采样

#### mix-sampler

支持任意数量sampler按任意权重加权采样，一般采用cos，hit各一半进行采样。

## Shader

目前没有单独出来作为一个模块，功能主要是递归的path tracing。

## Loader

写的时候使用了C风格的输入输出，后面会替换程cpp风格

图9是没有加载贴图的效果

<center align="center">    <img style="border-radius: 0.3125em;    box-shadow: 0 2px 4px 0 rgba(34,36,38,.12),0 2px 10px 0 rgba(34,36,38,.08); zoom:80%"     src=".\demo-pics\notext_02_indoor_10000spp.png">    <br>    <div style="color:orange; border-bottom: 1px solid #d9d9d9;    display: inline-block;    color: #999;    padding: 2px;">图9 no texture 02 indoor</div> </center>

## Accelerator

加速模块主要是bvh、kd-tree、photon mapping，不过目前只实现了bvh

#### bvh

之所以使用bvh而非kd-tree（photon mapping 会使用kd-tree），原因在于bvh虽然是基于物体的分割（kd-tree是基于空间的分割），造成无法保证前后遮挡关系，不能终止树的遍历，从而导致性能不如kd-tree，但是kd-tree可以维护（以为着可以适用于RTRT场景）并且通过一些加速算法（例如我使用的ordered bvh），可以达到与bvh不相上下的效果，自然会更popular，可以参考[Stack Exchange](https://computergraphics.stackexchange.com/questions/10098/is-bvh-faster-than-the-octree-kd-tree-for-raytracing-the-objects-on-a-gpu)上的回答。

本文参考了[Wald et. al.](https://dl.acm.org/doi/10.1145/1189762.1206075)的论文和PBRT的实现，不过应为还没来及测试各种算法实际跑出来的速率，所以具体细节会放到下一次作业，这里只简单提一下：

1. 因为现在渲染的是静态场景，所以考虑的是尽可能牺牲bvh build的时间和空间换取shading的速度。因此采用的是遍历O(n)种SAH取其中最优解的方法（共O($2^n$)种），因为无法避免排序，所以bvh build的时间是$O(n\lg^2 n)$，从图二可以看到，02的bvh build time大约20sec，使用bucket SAH应该可以有十倍以上加速，但不知道shading速度会减多少。
2. 采用了排序的bvh，这样每次都会先访问中心靠光线更近的bvh，虽然不能立刻终止，但可以尽可能减小t_max（光线hit的上界）从而尽早终止。
3. 所有物体都按照2种访问顺序重排，这样有更大的概率访问内存种连续部分，且顺序访问
4. bvh node使用了32位对准

还有很多可以优化的地方，没来及测试，参考相关代码部分的`TODO:`

