import platform
Import("env")

osType = platform.system()

print('Current OS is: ', osType)

if osType == 'Windows':
    env.Replace(MKSPIFFSTOOL=env.get("PROJECT_DIR") + '/mklittlefs.exe')
else:
    env.Replace(MKSPIFFSTOOL=env.get("PROJECT_DIR") + '/mklittlefs')
