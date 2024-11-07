# 面向对象编程，其实我认为需要这个我不如写cc
"""
class <className>:
    <class_attribute_name> = <value>

    def __init__(self,<param1>, <param2>, ...):
        self.<attr1> = <param1>
        self.<attr2> = <param2>
        # As many attributes as needed

   def <method_name>(self, <param1>, ...):
       <code>

   # As many methods as needed
"""

# 尽量使用大驼峰命名法

class Animal:
    name_ = ""
    age_ = 0
    _private_value = {}     # 名称前加一个下划线表示非公开的属性

    def __init__(self, age, name):
        self.age_ = age
        self.name_ = name
        self._private_value.update({1: "lulu"})

    def print_info(self):
        print(self.age_.__str__() + " " + self.name_ + " " + self._private_value.__str__())

    def add(self, b):
        print(self.age_ + b)

animal = Animal(20, "kuku")
animal.print_info()

del animal.name_    # 删除属性
del animal          # 删除对象

# 我们如果直接通过类修改属性，则所有对象都会共享（废话一句）
print(Animal.age_)

animal = Animal(10, "shuiniu")
animal.add(3)

class Dog(Animal):
    type = "dog"

    def print_info(self):
        Animal.print_info(self)
        print(self.type.__str__())

dog = Dog(10, "pipi")
dog.print_info()