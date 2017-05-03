#ifndef TESTNATIVESURFACE_H
#define TESTNATIVESURFACE_H

#include <jni.h>
#include <android/native_window.h>
#include <pthread.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_test_a3rdpartylibpng_NativeFunc_create(JNIEnv *env, jclass type, jint id);
JNIEXPORT void JNICALL Java_com_test_a3rdpartylibpng_NativeFunc_surfaceCreated(JNIEnv *env, jclass type, jint id, jobject surface);
JNIEXPORT void JNICALL Java_com_test_a3rdpartylibpng_NativeFunc_surfaceChanged(JNIEnv *env, jclass type, jint id, jint width, jint height);
JNIEXPORT void JNICALL Java_com_test_a3rdpartylibpng_NativeFunc_surfaceDestroyed(JNIEnv *env, jclass type, jint id);

#ifdef __cplusplus
}
#endif

class CppSurfaceView {
private:
    const char *VERTEXSHADER =
        "attribute vec4 a_Position;\n"
        "attribute vec2 a_Texcoord;\n"
        "varying vec2 texcoordVarying;\n"
        "void main() {\n"
        "    gl_Position = a_Position;\n"
        "    texcoordVarying = a_Texcoord;\n"
        "}\n";

    const char *FRAGMENTSHADER =
        "precision mediump float;\n"
        "varying vec2 texcoordVarying;\n"
        "uniform sampler2D u_Sampler;\n"
        "void main() {\n"
        "    gl_FragColor = texture2D(u_Sampler, texcoordVarying);\n"
        "}\n";

    static const float vertices[];
    static const float texcoords[];

private:
    GLuint mProgramId = -1;
    GLuint ma_Position= -1;
    GLuint ma_Texcoord= -1;
    GLuint mTextureId = -1;
    GLuint mu_Sampler = -1;

private:
    GLuint createProgram(const char *vertexshader, const char *fragmentshader);
    GLuint loadShader(int i, const char *vertexshader);
    void checkGlError(const char *argstr);

public:
    static const int STATUS_NONE   = 0;
    static const int STATUS_INITIALIZING = 1;
    static const int STATUS_DRAWING= 2;
    static const int STATUS_FINISH = 3;
    int mStatus = STATUS_NONE;
    int mId = -1;
    pthread_t mThreadId = -1;
    ANativeWindow *mWindow = NULL;
    EGLDisplay mEGLDisplay = NULL;
    EGLContext mEGLContext = NULL;
    EGLSurface mEGLSurface = NULL;
    bool isSurfaceCreated = false;
    int DspW = 0;
    int DspH = 0;

public:
    CppSurfaceView(int id);
    virtual ~CppSurfaceView();
    static void *draw_thread(void *pArg);
    void createThread(JNIEnv *pEnv, jobject surface);
    void initEGL();
    void finEGL();
    void initGL();
    void predrawGL();
    void drawGL();
    void destroy();
};

#endif //TESTNATIVESURFACE_H
