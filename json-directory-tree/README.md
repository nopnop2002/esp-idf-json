# JSON Example
Converts a directory structure into a nested JSON string.   
This is a complex structure of objects and arrays.   
You need to serialize from the lowest level up.   

# Directory structure
```
$ ls -lR fatfs_image
fatfs_image:
total 20
-rw-rw-r-- 1 nop nop   11 Apr  6 05:37 CMakeLists.txt
drwxrwxr-x 4 nop nop 4096 Apr  6 05:37 libs
drwxrwxr-x 2 nop nop 4096 Apr  6 05:37 obj
-rw-rw-r-- 1 nop nop   22 Apr  6 05:37 sdkconfig
drwxrwxr-x 2 nop nop 4096 Apr  6 05:37 src

fatfs_image/libs:
total 8
drwxrwxr-x 2 nop nop 4096 Apr  6 05:37 lib001
drwxrwxr-x 2 nop nop 4096 Apr  6 05:37 lib002

fatfs_image/libs/lib001:
total 4
-rw-rw-r-- 1 nop nop 33 Apr  6 05:37 lib001.a

fatfs_image/libs/lib002:
total 4
-rw-rw-r-- 1 nop nop 55 Apr  6 05:37 lib002.a

fatfs_image/obj:
total 8
-rw-rw-r-- 1 nop nop 22 Apr  6 05:37 main.o
-rw-rw-r-- 1 nop nop 11 Apr  6 05:37 sub.o

fatfs_image/src:
total 8
-rw-rw-r-- 1 nop nop 44 Apr  6 05:37 main.c
-rw-rw-r-- 1 nop nop 11 Apr  6 05:37 sub.c
```

## Serialize
```
{
        "level":        1,
        "self": "/root",
        "cmakelists.txt":       {
                "size": 11,
                "mtime":        "2025-04-06 05:37:38"
        },
        "sdkconfig":    {
                "size": 22,
                "mtime":        "2025-04-06 05:37:38"
        },
        "child":        [{
                        "level":        2,
                        "self": "/root/libs",
                        "child":        [{
                                        "level":        3,
                                        "self": "/root/libs/lib001",
                                        "lib001.a":     {
                                                "size": 33,
                                                "mtime":        "2025-04-06 05:37:38"
                                        }
                                }, {
                                        "level":        3,
                                        "self": "/root/libs/lib002",
                                        "lib002.a":     {
                                                "size": 55,
                                                "mtime":        "2025-04-06 05:37:38"
                                        }
                                }]
                }, {
                        "level":        2,
                        "self": "/root/obj",
                        "main.o":       {
                                "size": 22,
                                "mtime":        "2025-04-06 05:37:38"
                        },
                        "sub.o":        {
                                "size": 11,
                                "mtime":        "2025-04-06 05:37:38"
                        }
                }, {
                        "level":        2,
                        "self": "/root/src",
                        "main.c":       {
                                "size": 44,
                                "mtime":        "2025-04-06 05:37:38"
                        },
                        "sub.c":        {
                                "size": 11,
                                "mtime":        "2025-04-06 05:37:38"
                        }
                }]
}
```
