> C/C++实现聊天服务器

# C语言版聊天服务器实现

**CChatServer_eclipse**:这个工程是用Eclipse+Makefile编译的工程



# 附录

## 1、编译缺少<mysql/mysql.h>

参考： linux 找不到 mysql.h 文件 解决[http://blog.csdn.net/weihua1984/article/details/5454121]

解决：

`sudo apt-get install libmysqlclient15-dev` 安装此软件包
然后在/usr/include/mysql里可以找到mysql.h              

编译的时候 

```
gcc xx.c -o xx -L /usr/include/mysql -l mysqlclient
```

不行的话就设置一下环境变量http://james23dier.iteye.com/blog/763274

好像不用引入下面这么多

```
-L/usr/lib/mysql  -I/usr/include/mysql -lmysqlclient
```





## 2、缺少#include <glib.h>

```
sudo apt-get install libglib2.0-dev
sudo apt-get install glib2.0
```

要保证/usr/include有头文件，没有就找地方复制过来


单文件编译
```
gcc glibtest.c -o testglib `pkg-config --cflags --libs glib-2.0`
```








