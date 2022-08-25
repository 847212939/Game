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
1.是在vs2022下编译动态库dll用的  
2.windows IOCP  

# Cxxso   
1.是在linux下编译动态库so用的  
2.g++ -shared -fpic -lm -ldl -levent -lpthread -std=c++11 -O3 -o Cxxdll.so *.cpp  
3.不写Makefile是因为实在用不到  
