> C/C++实现聊天服务器



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



## 2、缺少#include <glib.h>

```
sudo apt-get install libglib2.0-dev
```

