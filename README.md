# asio
Thread safety https client using boost::asio & openssl

## Build & How to Use
### Windows
Build Solution,

__$(boost_1660)__ = boost include dir<br>
__$(boost_1660_lib32)__ = boost x32 lib dir<br>
__$(boost_1660_lib64)__ = boost x64 lib dir<br>

```
setx boost_1660 "BOOST INCLUDE DIR"
setx boost_1660_lib32 "BOOST LIB32 DIR"
setx boost_1660_lib64 "BOOST LIB64 DIR"
```

or change include dir and linker dir.

### Linux
```
git clone https://github.com/okanon/asio.git
cd asio/https/https
cp https.cpp https.h ../test

g++ -o https.o -c https.cpp
g++ -o main.o -c main.cpp -I.
g++ -o client main.o https.o -lssl -lcrypto -lboost_system -lpthread

./client
```

## SnapShot
 ![](https://raw.githubusercontent.com/okanon/asio/master/resources/snapshots/google.png)

