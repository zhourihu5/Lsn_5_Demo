#include <jni.h>
#include <string>
//import .......
#include <malloc.h>
#include <android/bitmap.h>
#include <jpeglib.h>

void write_JPEG_file(uint8_t *temp, int w, int h, jint q, const char *path);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_administrator_lsn_15_1demo_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_lsn_15_1demo_MainActivity_testImage(JNIEnv *env, jobject instance) {


}

extern "C"
JNIEXPORT void JNICALL
Java_com_example_administrator_lsn_15_1demo_MainActivity_nativeCompress(JNIEnv *env,
                                                                        jobject instance,
                                                                        jobject bitmap, jint q,
                                                                        jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);


    //从bitmap获取argb数据
    AndroidBitmapInfo info;//info=new 对象();
    //获取里面的信息
    AndroidBitmap_getInfo(env, bitmap, &info);
    //得到图片中的像素信息
    uint8_t *pixels;//uint8_t char    java   byte     *pixels可以当byte[]
    AndroidBitmap_lockPixels(env, bitmap, (void **) &pixels);
    //jpeg argb中去掉他的a ===>rgb
    int w = info.width;
    int h = info.height;
    int color;
    //开一块内存用来存入rgb信息
    uint8_t *data = (uint8_t *) malloc(w * h * 3);//data中可以存放图片的所有内容
    uint8_t *temp = data;
    uint8_t r, g, b;
    //循环取图片的每一个像素
    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            color = *(int *) pixels;//0-3字节  color4 个字节  一个点
            //取出rgb
            r = (color >> 16) & 0xFF;//    #rrggbb  16  0000rr   8  00rrgg
            g = (color >> 8) & 0xFF;
            b = color & 0xFF;
            //存放，以前的主流格式jpeg    bgr
            *data = b;
            *(data + 1) = g;
            *(data + 2) = r;
            data += 3;
            //指针跳过4个字节
            pixels += 4;
        }
    }
    //把得到的新的图片的信息存入一个新文件 中
    write_JPEG_file(temp,w,h,q,path);

    AndroidBitmap_unlockPixels(env, bitmap);
    free(data);

    env->ReleaseStringUTFChars(path_, path);
}

void write_JPEG_file(uint8_t *temp, int w, int h, jint q, const char *path) {

}