# asio
Thread safety https client using boost::asio & openssl

## Build & How to Use
### Windows
Build Solution and copy libeay32.lib, ssleay32.lib in __resource/dylib/(Platform)/__ directory to the directory with __client.exe__.

## Linux
```
git clone https://github.com/okanon/asio.git
cd asio/https/https
cp https.cpp https.h ../test

g++ -o https.o -c https.cpp
g++ -o main.o -c main.cpp
g++ -o client main.o https.o -lssl -lcrypto -lboost_system -lpthread

./client
```

