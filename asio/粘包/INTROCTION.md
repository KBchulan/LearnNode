#### 原因

​	因为TCP底层通信是面向字节流的，TCP只保证发送数据的准确性和顺序性，字节流以字节为单位，客户端每次发送N个字节给服务端，N取决于当前客户端的发送缓冲区是否有数据，比如发送缓冲区总大小为10个字节，当前有5个字节数据(上次要发送的数据比如’loveu’)未发送完，那么此时只有5个字节空闲空间，我们调用发送接口发送hello world！其实就是只能发送Hello给服务器，那么服务器一次性读取到的数据就很可能是loveuhello。而剩余的world！只能留给下一次发送，下一次服务器接收到的就是world！

​	客户端的发送频率远高于服务器的接收频率，就会导致数据在服务器的tcp接收缓冲区滞留形成粘连，比如客户端1s内连续发送了两个hello world！,服务器过了2s才接收数据，那一次性读出两个hello world！。
​	tcp底层的安全和效率机制不允许字节数特别少的小包发送频率过高，tcp会在底层累计数据长度到一定大小才一起发送，比如连续发送1字节的数据要累计到多个字节才发送，可以了解下tcp底层的Nagle算法。
​	再就是我们提到的最简单的情况，发送端缓冲区有上次未发送完的数据或者接收端的缓冲区里有未取出的数据导致数据粘连。

#### 解决方案

​	在应用层选用tlv协议，发送和接收都满足此协议（也就是我们的切包处理）

