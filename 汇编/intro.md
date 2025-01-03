## introduction
### ***来历***
&emsp;&emsp;最早的时候，编写程序就是手写二进制指令，然后通过各种开关输入计算机，比如要做加法了，就按一下加法开关。后来，发明了纸带打孔机，通过在纸带上打孔，将二进制指令自动输入计算机。

&emsp;&emsp;为了解决二进制指令的可读性问题，工程师将那些指令写成了八进制。二进制转八进制是轻而易举的，但是八进制的可读性也不行。很自然地，最后还是用文字表达，加法指令写成 ADD。内存地址也不再直接引用，而是用标签表示。

&emsp;&emsp;这样的话，就多出一个步骤，要把这些文字指令翻译成二进制，这个步骤就称为 assembling，完成这个步骤的程序就叫做 assembler。它处理的文本，自然就叫做 aseembly code。标准化以后，称为 assembly language，缩写为 asm，中文译为汇编语言。

### ***基础认知***
&emsp;&emsp;我们应当先知道寄存器和内存模型。  

#### ***寄存器***
&emsp;&emsp;CPU 本身只负责运算，不负责储存数据。数据一般都储存在内存之中，CPU 要用的时候就去内存读写数据。但是，CPU 的运算速度远高于内存的读写速度，为了避免被拖慢，CPU 都自带一级缓存和二级缓存。基本上，CPU 缓存可以看作是读写速度较快的内存。

&emsp;&emsp;但是，CPU 缓存还是不够快，另外数据在缓存里面的地址是不固定的，CPU 每次读写都要寻址也会拖慢速度。因此，除了缓存之外，CPU 还自带了寄存器（register），用来储存最常用的数据。也就是说，那些最频繁读写的数据（比如循环变量），都会放在寄存器里面，CPU 优先读写寄存器，再由寄存器跟内存交换数据。  

&emsp;&emsp;寄存器不依靠地址区分数据，而依靠名称。每一个寄存器都有自己的名称，我们告诉 CPU 去具体的哪一个寄存器拿数据，这样的速度是最快的。有人比喻寄存器是 CPU 的零级缓存。  

&emsp;&emsp;寄存器的种类很多，比如通用寄存器、浮点寄存器、条件码寄存器、堆栈指针寄存器等等：EAX&emsp;EBX&emsp;ECX&emsp;EDX&emsp;ESI&emsp;EDI&emsp;EBP&emsp;ESP。

- EAX（累加器）：算术运算和I/O操作，常作为函数返回值。  
- EBX（基址器）：存储数据的基址，方便内存访问。   
- ECX（计数器）：在循环和字符串操作中用作计数器。  
- EDX（数据器）：I/O操作，或在乘法和除法中存储高位结果。  
- ESI（源索引器）：指向源数据的地址，常用于字符串和数组操作。  
- EDI（目的索引器）：指向目标数据的地址，通常与ESI配合使用。  
- EBP（基指针器）：指向当前函数的栈帧基址，访问局部变量。  
- ESP（栈顶指针器）：指向栈的顶部，管理函数调用和返回中的栈空间。

#### ***内存模型***
&emsp;&emsp;寄存器只能存放很少量的数据，大多数时候，CPU 要指挥寄存器，直接跟内存交换数据。所以，除了寄存器，还必须了解内存怎么储存数据，此处我们介绍栈区内存模型和堆区内存模型。

&emsp;&emsp;首先来看堆区内存模型。堆区内存模型是指，程序运行时，需要动态分配内存，就从堆区申请内存，用完之后再释放。堆区内存模型的特点是，申请和释放内存的速度很快，但内存的利用率不高。  

- &emsp;&emsp;程序运行的时候，操作系统会给它分配一段内存，用来储存程序和运行产生的数据。这段内存有起始地址和结束地址，比如从0x1000到0x8000，起始地址是较小的那个地址，结束地址是较大的那个地址。

- &emsp;&emsp;程序运行过程中，对于动态的内存占用请求（比如新建对象，或者使用malloc命令），系统就会从预先分配好的那段内存之中，划出一部分给用户，具体规则是从起始地址开始划分（实际上，起始地址会有一段静态数据，这里忽略）。举例来说，用户要求得到10个字节内存，那么从起始地址0x1000开始给他分配，一直分配到地址0x100A，如果再要求得到22个字节，那么就分配到0x1020，这也是我们俗称的向上生长。  

&emsp;&emsp;接下来我们看一下栈区内存模型。栈区内存模型是指，程序运行时，需要临时保存一些数据，就从栈区申请内存，用完之后就自动释放,我们可以简单理解为函数运行而临时占用的内存区域。栈区内存模型的特点是，申请和释放内存的速度很快，但内存的利用率不高。  

- &emsp;&emsp;Stack 是由内存区域的结束地址开始，从高位（地址）向低位（地址）分配。比如，内存区域的结束地址是0x8000，第一帧假定是16字节，那么下一次分配的地址就会从0x7FF0开始；第二帧假定需要64字节，那么地址就会移动到0x7FB0。  

![内存四区图片](https://subingwen.cn/linux/file-descriptor/image-20210130093015907.png)

&emsp;&emsp;保留区: 位于虚拟地址空间的最底部，未赋予物理地址。任何对它的引用都是非法的，程序中的空指针（NULL）指向的就是这块内存地址。  

&emsp;&emsp;.text段: 代码段也称正文段或文本段，通常用于存放程序的执行代码(即CPU执行的机器指令)，代码段一般情况下是只读的，这是对执行代码的一种保护机制。  

&emsp;&emsp;.data段: 数据段通常用于存放程序中已初始化且初值不为0的全局变量和静态变量。数据段属于静态内存分配(静态存储区)，可读可写。  

&emsp;&emsp;.bss段: 未初始化以及初始为0的全局变量和静态变量，操作系统会将这些未初始化变量初始化为0。  

&emsp;&emsp;堆(heap)：用于存放进程运行时动态分配的内存，堆中内容是匿名的，不能按名字直接访问，只能通过指针间接访问，堆向高地址扩展(即“向上生长”)，是不连续的内存区域。这是由于系统用链表来存储空闲内存地址，自然不连续，而链表从低地址向高地址遍历。  

&emsp;&emsp;内存映射区(mmap)：作为内存映射区加载磁盘文件，或者加载程序运作过程中需要调用的动态库。  

&emsp;&emsp;栈(stack): 存储函数内部声明的非静态局部变量，函数参数，函数返回地址等信息，栈内存由编译器自动分配释放。栈和堆相反地址“向下生长”，分配的内存是连续的。  

&emsp;&emsp;命令行参数：存储进程执行的时候传递给main()函数的参数，argc，argv[]。  

&emsp;&emsp;环境变量: 存储和进程相关的环境变量, 比如: 工作路径, 进程所有者等信息。  