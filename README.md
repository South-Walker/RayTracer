# RayTracer
## 简介
开坑光线追踪，初步计划是读完Peter Shirley的[光线追踪三部曲](https://github.com/RayTracing/InOneWeekend)
## 光线追踪：一个周末（Ray Tracing in One Weekend）
代码已上传，基于文章最后项目渲染得到的图像如下<br><br>
![](/img/out1.png)<br><br>
图像为1920*1080像素，每个像素点在20个不同的cpu上同时基于不同的随机种子采样，每次任务采样100次，得到的20张图各像素取平均。<br>

## 光线追踪：下一周（Ray Tracing The Next Week）
代码已上传，基于文章最后项目渲染得到的图像如下<br><br>
![](/img/out2.png)<br><br>
图像为1920*1080像素，每个像素点在20个不同的cpu上同时基于不同的随机种子采样，每次任务采样1000次，得到的20张图各像素取平均。<br>

## 光线追踪：余生（Ray Tracing The Rest Of Your Life）
代码已上传，修改dielectric材质后并利用重要性采样算法渲染得到下图<br><br>
![](/img/out3.png)<br><br>
