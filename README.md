# RayTracer
## 间接
开坑光线追踪，初步计划是读完Peter Shirley的[光线追踪三部曲](https://github.com/RayTracing/InOneWeekend)
## 光线追踪：一个周末（Ray Tracing in One Weekend）
代码已上传，基于文章最后项目渲染得到的图像如下<br><br>
![](/img/out.png)<br><br>
图像为1920*1080像素，每个像素点在20个不同的cpu上同时基于不同的随机种子采样，每次任务采样100次，得到的20张图各像素取平均。
平均前：![](/img/200beforavg.png)<br><br>
平均后：![](/img/200avg.png)<br><br>
注意到阴影与背景部分有明显差别。

## 光线追踪：下一周（Ray Tracing The Next Week）
## 光线追踪：余生（Ray Tracing The Rest Of Your Life）
