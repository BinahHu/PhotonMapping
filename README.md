实现了基于光子映射的真实感图形学绘制模型，关于该模型的原理和功能，请参照doc文件夹中的文档

一、使用方法：
在src文件夹下执行如下指令
1. cmake .
2. make

然后执行 ./PhotonMapping 即可运行程序

二、输入输出界定
输入文件在input文件夹中，具体格式见后文，如需调整输入文件，可自行调整main.cpp中的相关代码并再次编译
程序运行过程中所需的纹理图片请以png格式存放在input文件夹中。具体调用格式见后文
输出文件放在output文件夹中，统一命名为picture.png，注意新的图片会将原来同名的图片覆盖

三、输入格式规定
输入信息遵循以下格式：
[Type] [SubType]
[var] [valuelist]
end


每一个这样的输入单元描述一个实体，实体包括：相机，背景光，物体，光源
在TYPE和end之间的部分用于描述 该实体的各项特征，具体输入方式如下：

1、相机， [Type =] camera
[var =] O，三维向量 ，后接三个实数，用于表示相机视点在场景中的位置
sight，三维向量，用于表示视线正前方，不必归一化，程序中会自己归一化
up，三维向量，用于表示视线正上方
f，实数，表示相机视点到幕布的距离（即焦距）
w，正整数，表示纵向分辨率
h，正整数，表示横向分辨率
phyw，实数，表示幕布在场景中的纵向大小
phyh，实数，表示幕布在场景中的横向大小
sizeceof，实数，玄学参数，用于修正光子映射中KNN算法中除以距离平方时的几何尺度因素

2、背景光， background
color， 三维向量， 后接3个0到1之间的实数，表示背景光颜色

3、物体 [Type =] obj
//以下为对所有object通用的输入信息，对每个子物体专门的信息见后文
color，三维向量，表示在没有纹理贴图时表面的颜色
incolor，三维向量，表示透明物体内部的颜色
absorb，三维向量，表示透明物体内部对RGB三种颜色的光的吸收系数，0 0 0 表示完全不吸收（注：根据啤酒定律，这个数要乘以在透明物体内部移动的距离然后取指数，所以这一项千万不要设的太大）
n，实数，折射率
diff，实数，表面漫反射系数
refl，实数，表面反射系数
refc，实数，表面折射系数
spec，实数，Phong模型中的高光项系数
abso，实数，光子映射中的吸收率
texture，文件名，指定该物体的纹理文件，（注：程序中会自动加上"../input"前缀，因此只需要写文件名即可）



3.1 矩形 [SubType =] rect
N，三维向量，矩形面片的法向，不必归一化
O，三维向量，表示矩形的某一个顶点在场景中的位置
Dx，Dy，三维向量，表示以O为原点，两条边以O为交点的边的边向量

3.2 球 sphe
O，三维向量，球心坐标
R，实数，球的半径

3.3 Bezier旋转面 bezier

Deg，曲线阶次，建议次数不要太高，否则会有病态现象

O，三维向量，旋转轴原点坐标

Dx，Dy，三维向量，基准平面对应x，y轴方向向量

P，三维向量，控制点在基准平面中的坐标

具体使用方法及实现机理见文档



4、光源 light
color，三维向量，表示光源颜色和亮度，其三个分量可以超过1，这样在光子映射的时候可以有更好的结果，而光线跟踪的时候会自动将这个亮度归一化，将其当做一个三个分量都在0到1之间的颜色，推荐设置的大一点
max_photon，正整数，表示该光源可以发射的最大光子数

4.1点光源 point
O，三维向量，表示点光源的位置

4.2面积光源 area
定义方法和矩形完全相同



注：对于一个实体，以上参数并不要求输入完整，对于没有输入的参数，会自动设置成默认值

四、其他
程序内部还有一些其他参数，比如光子映射和光线跟踪的最大深度等等，读者可根据上下文自行理解和修改
另外，源代码中的lodepng.h和lodepng.cpp是网上开源的用于读写png文件的库，在这里直接拿来使用以代替其他复杂的库，如OpenCV
