此文件用于将tree后的文件转换成path格式，使用方式如下
t2p.exe src.txt dst.txt
src为源文件，此处可自定义修改
dst为目标文件，可自定义修改
可以更方便地查找文件
lib/
├── binfmt.d
├── cpp -> ../bin/cpp
├── debug
│   ├── bin -> usr/bin
│   ├── lib -> usr/lib
│   ├── lib64 -> usr/lib64
│   ├── sbin -> usr/sbin
│   └── usr
│       ├── bin
│       ├── lib
│       ├── lib64
│       └── sbin
├── dracut
│   ├── dracut.conf.d
│   │   ├── 01-dist.conf


/lib/
/binfmt.d
/cpp -> ../bin/cpp
/debug
/debug/bin -> usr/bin
/debug/lib -> usr/lib
/debug/lib64 -> usr/lib64
/debug/sbin -> usr/sbin
/debug/usr
/debug/usr/bin
/debug/usr/lib
/debug/usr/lib64
/debug/usr/sbin
/dracut
/dracut/dracut.conf.d
