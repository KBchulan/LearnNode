// 我们这个文件从cpp->汇编
// 可以通过as进行编译，但是ld会报错，因为没有生成符号表，所以我们需要用objdump查看符号表
// 然后手动添加符号表到汇编代码中，就可以生成可执行文件了。

int add(int &a, int &b)
{
    return a + b;
}

int main()
{
    int x = 5;
    int y = 7;
    int z = add(x, y);
    return 0;
}