# Game    
1.是c++游戏服务器  
2.是跨平台服务器   
3.大量用到c++11 新特性  
4.libevent网络库  

# Client  
1.是c#客户端demo  
2.网络库是用c++ libevent写的  
3.windows用iocp  
4.linux用epoll  
5.提高客户端性能  

# Cxxdll  
1.跨平台库编译  
2.是在vs2022下编译动态库dll用的  
3.windows IOCP   
4.是在linux下编译动态库so用的  
5.linux epoll
6.g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp  
7.不写Makefile是因为实在用不到  
8.在Android用

# Data
1.配置文件  
2.tolua++  
3.lua调用c++读取配置  
4.根据pkg和exe生成接口供lua调用  
