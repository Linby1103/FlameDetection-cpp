#                                  火焰检测

## 1、代码组成结构

source：动态区域提取代码，火焰区域提取代码

Vibe：背景建模代码

video：测试视频

## 2、原理

vibe背景建模算法:<https://www.cnblogs.com/ywsoftware/p/4434074.html>

火焰区域提取算法：颜色识别

​    *condition :  1、red > r_threshold  2、red>=green>=blue 3、 S>=((255-R)*ST/r_threshold  )

red :红色分量

green：绿色分量

blue ：蓝色分量

r_threshold  ：红色分量阈值

S:饱和度 

</

float maxValue = std::max(std::max(b_color, g_color), r_color);
float minValue = std::min(std::min(b_color, g_color), r_color);

S = (1 - 3.0*minValue / (b_color + g_color + r_color));

/>



ST：饱和度阈值

## 3、代码地址

代码：https://github.com/Linby1103/FlameDetection-cpp.git