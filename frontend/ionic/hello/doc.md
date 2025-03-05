1. web端可以直接访问

```shell
npm run dev
```

2. 到android-studio运行

```shell
npm run build
ionic capacitor copy android
ionic capacitor open android
```

3. 热更新
   我们需要用usb连接手机，然后打开开发者模式，然后打开usb调试，然后打开ionic的开发者模式，然后就可以热更新

```shell
ionic capacitor run android -1 --external
```
