## JSONitor v1.0——可视化JSON编辑器

---

* 目录下JSONitor v1.0.rar下载解压运行JSONitor.exe 即用！

### 实现目标

* 实现所见即所得的json格式编辑器
* 可以将文件储存于服务器端数据库

### 界面截图

<img src="pics\1.png" alt="1" style="zoom:33%;" />

<img src="pics\2.png" alt="2" style="zoom:33%;" />

<img src="pics\3.png" alt="3" style="zoom:33%;" />

### 功能以及开发记录（碎碎念）

* 很不满意自带的QWidget窗口，于是找了个无边框的QUIWidget，仍不满意于是改了下qss和Menu布局，增加了logo和自己ps做的初始界面底图。
* 坚持一向的去button，去label化设计理念，增加快捷键，尽量做的轻量（高情商，实际就是没得做做的功能少）。
* 本地文本编辑器的基础功能：
  * 复制粘贴撤销
  * 代码高亮、折叠、错误代码提示、行号显示、括号补全
  * 可实时通过修改树形图更新代码同样也可以根据代码实时刷新树形图
  * 支持整理代码对齐
  * 支持开多个独立窗口
  * 支持文件保存新建另存为

* 支持和服务器mysql通讯（阿里云服务器2021-07-10到期，很快就没得用了，但我觉得本地编辑器已经挺好用的了）

* 随便用用吧，这次不像java那么卡体验还可以呐。

  ​																					(,,• ₃ •,,)

### 关于大作业想说的

* 老师布置的要求太迷惑了。要求服务器说是想让人学写socket？socket之前好多大作业倒是写过好多遍，反而这个大作业没得用。
* 以及所谓的加分项目，我还是比较反对的。不只是卷不卷的问题，是因为在基础内容还没完成好的情况下去追求加分项，例如在已经实现的功能不够完备和鲁棒的情况下还在一味追求多功能，实际上用户完全不需要那么多杂七杂八的功能（甚至服务器也不用，我用了这么久typora，word也没往服务器上传过）。不如好好考虑怎样把“文本编辑器”实现的足够合理和有设计感，独特的地方。
* 还是付出了比较多的思虑的，还老被人恶心（

