#include <jni.h>
#include <memory>
#include <Engine.h>

#define R 5

std::shared_ptr<triangle::Engine> engine;
std::shared_ptr<triangle::Camera> camera;
float x = 0.0f, z = 0.0f;

extern "C"
{

extern "C" JNIEXPORT void
JNICALL Java_io_github_kenneycode_triangle_example_MainActivity_drawFrame(JNIEnv *env, jobject /* this */, jint width, jint height, jlong timestamp) {
    if (engine == nullptr) {
        engine = std::make_shared<triangle::Engine>(width, height);
        camera = std::make_shared<triangle::Camera>(glm::vec3(0.0f, 0.0f, R), glm::vec3(0.0f, 0.0f, 0.0f),
                                                    glm::vec3(0.0f, 1.0f, 0.0f), 70.0f, float(width) / height, 1.0f, 10.0f);
        engine->setDefaultCamera(camera);
        engine->loadGLTF("/sdcard/DamagedHelmet/glTF/DamagedHelmet.gltf");
    }
    auto theta = -timestamp % 100000 / 1000.0f;
    x = R * cos(theta);
    z = R * sin(theta);
    camera->setPosition(glm::vec3(x, 0.0f, z));
    engine->drawFrame();
}

}