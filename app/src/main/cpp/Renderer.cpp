#include "Renderer.h"

#include <game-activity/native_app_glue/android_native_app_glue.h>
#include <GLES3/gl3.h>
#include <memory>
#include <vector>
#include <android/imagedecoder.h>

#include "AndroidOut.h"
#include "Shader.h"
#include "Utility.h"
#include "TextureAsset.h"

//! executes glGetString and outputs the result to logcat
#define PRINT_GL_STRING(s) {aout << #s": "<< glGetString(s) << std::endl;}

/*!
 * @brief if glGetString returns a space separated list of elements, prints each one on a new line
 *
 * This works by creating an istringstream of the input c-style string. Then that is used to create
 * a vector -- each element of the vector is a new element in the input string. Finally a foreach
 * loop consumes this and outputs it to logcat using @a aout
 */
#define PRINT_GL_STRING_AS_LIST(s) { \
std::istringstream extensionStream((const char *) glGetString(s));\
std::vector<std::string> extensionList(\
        std::istream_iterator<std::string>{extensionStream},\
        std::istream_iterator<std::string>());\
aout << #s":\n";\
for (auto& extension: extensionList) {\
    aout << extension << "\n";\
}\
aout << std::endl;\
}

//! Color for cornflower blue. Can be sent directly to glClearColor
#define CORNFLOWER_BLUE 100 / 255.f, 149 / 255.f, 237 / 255.f, 1

// Vertex shader, you'd typically load this from assets
static const char *vertex = R"vertex(#version 300 es
in vec3 inPosition;
in vec2 inUV;

out vec2 fragUV;

uniform mat4 uProjection;

void main() {
    fragUV = inUV;
    gl_Position = uProjection * vec4(inPosition, 1.0);
}
)vertex";

// Fragment shader, you'd typically load this from assets
static const char *fragment = R"fragment(#version 300 es
precision mediump float;

in vec2 fragUV;

uniform sampler2D uTexture;
uniform float uAlpha;

out vec4 outColor;

void main() {
    outColor = texture(uTexture, fragUV);
    outColor.a *= uAlpha;
}
)fragment";

/*!
 * Half the height of the projection matrix. This gives you a renderable area of height 4 ranging
 * from -2 to 2
 */
static constexpr float kProjectionHalfHeight = 2.f;

/*!
 * The near plane distance for the projection matrix. Since this is an orthographic projection
 * matrix, it's convenient to have negative values for sorting (and avoiding z-fighting at 0).
 */
static constexpr float kProjectionNearPlane = -1.f;

/*!
 * The far plane distance for the projection matrix. Since this is an orthographic porjection
 * matrix, it's convenient to have the far plane equidistant from 0 as the near plane.
 */
static constexpr float kProjectionFarPlane = 1.f;

Renderer::~Renderer() {
    if (display_ != EGL_NO_DISPLAY) {
        eglMakeCurrent(display_, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context_ != EGL_NO_CONTEXT) {
            eglDestroyContext(display_, context_);
            context_ = EGL_NO_CONTEXT;
        }
        if (surface_ != EGL_NO_SURFACE) {
            eglDestroySurface(display_, surface_);
            surface_ = EGL_NO_SURFACE;
        }
        eglTerminate(display_);
        display_ = EGL_NO_DISPLAY;
    }
    cleanupAudio();
}

void Renderer::render() {
    // Check to see if the surface has changed size. This is _necessary_ to do every frame when
    // using immersive mode as you'll get no other notification that your renderable area has
    // changed.
    updateRenderArea();

    // Calculate delta time
    auto currentTime = std::chrono::steady_clock::now();
    float deltaTime = std::chrono::duration<float>(currentTime - lastFrameTime_).count();
    lastFrameTime_ = currentTime;

    // Update fade
    const float fadeSpeed = 2.0f; // Seconds to complete fade
    if (isFadingOut_) {
        fadeAlpha_ -= deltaTime * fadeSpeed;
        if (fadeAlpha_ <= 0.0f) {
            fadeAlpha_ = 0.0f;
            isFadingOut_ = false;
            // Execute the delayed state change
            if (pendingState_ == GameState::CHARACTER_CREATION) {
                startNativeActivity("com.example.taticasdodestino.CreationActivity");
            } else if (pendingState_ == GameState::LOAD_GAME) {
                startNativeActivity("com.example.taticasdodestino.LoadActivity");
            } else if (pendingState_ == GameState::TITLE_SCREEN) {
                startNativeActivity("com.example.taticasdodestino.MainActivity");
            }
        }
    } else if (fadeAlpha_ < 1.0f) {
        fadeAlpha_ += deltaTime * fadeSpeed;
        if (fadeAlpha_ > 1.0f) fadeAlpha_ = 1.0f;
    }

    // When the renderable area changes, the projection matrix has to also be updated. This is true
    // even if you change from the sample orthographic projection matrix as your aspect ratio has
    // likely changed.
    if (shaderNeedsNewProjectionMatrix_) {
        // a placeholder projection matrix allocated on the stack. Column-major memory layout
        float projectionMatrix[16] = {0};

        // build an orthographic projection matrix for 2d rendering
        Utility::buildOrthographicMatrix(
                projectionMatrix,
                kProjectionHalfHeight,
                float(width_) / height_,
                kProjectionNearPlane,
                kProjectionFarPlane);

        // send the matrix to the shader
        // Note: the shader must be active for this to work. Since we only have one shader for this
        // demo, we can assume that it's active.
        shader_->setProjectionMatrix(projectionMatrix);

        // make sure the matrix isn't generated every frame
        shaderNeedsNewProjectionMatrix_ = false;
    }

    // clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // Set transparency
    glUniform1f(glGetUniformLocation(shader_->getProgram(), "uAlpha"), fadeAlpha_);

    // Render all the models. There's no depth testing in this sample so they're accepted in the
    // order provided. But the sample EGL setup requests a 24 bit depth buffer so you could
    // configure it at the end of initRenderer
    if (!models_.empty()) {
        for (const auto &model: models_) {
            shader_->drawModel(model);
        }
    }

    // Present the rendered image. This is an implicit glFlush.
    auto swapResult = eglSwapBuffers(display_, surface_);
    assert(swapResult == EGL_TRUE);
}

void Renderer::initRenderer() {
    // Choose your render attributes
    constexpr EGLint attribs[] = {
            EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 24,
            EGL_NONE
    };

    // The default display is probably what you want on Android
    auto display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, nullptr, nullptr);

    // figure out how many configs there are
    EGLint numConfigs;
    eglChooseConfig(display, attribs, nullptr, 0, &numConfigs);

    // get the list of configurations
    std::unique_ptr<EGLConfig[]> supportedConfigs(new EGLConfig[numConfigs]);
    eglChooseConfig(display, attribs, supportedConfigs.get(), numConfigs, &numConfigs);

    // Find a config we like.
    // Could likely just grab the first if we don't care about anything else in the config.
    // Otherwise hook in your own heuristic
    auto config = *std::find_if(
            supportedConfigs.get(),
            supportedConfigs.get() + numConfigs,
            [&display](const EGLConfig &config) {
                EGLint red, green, blue, depth;
                if (eglGetConfigAttrib(display, config, EGL_RED_SIZE, &red)
                    && eglGetConfigAttrib(display, config, EGL_GREEN_SIZE, &green)
                    && eglGetConfigAttrib(display, config, EGL_BLUE_SIZE, &blue)
                    && eglGetConfigAttrib(display, config, EGL_DEPTH_SIZE, &depth)) {

                    aout << "Found config with " << red << ", " << green << ", " << blue << ", "
                         << depth << std::endl;
                    return red == 8 && green == 8 && blue == 8 && depth == 24;
                }
                return false;
            });

    aout << "Found " << numConfigs << " configs" << std::endl;
    aout << "Chose " << config << std::endl;

    // create the proper window surface
    EGLint format;
    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
    EGLSurface surface = eglCreateWindowSurface(display, config, app_->window, nullptr);

    // Create a GLES 3 context
    EGLint contextAttribs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
    EGLContext context = eglCreateContext(display, config, nullptr, contextAttribs);

    // get some window metrics
    auto madeCurrent = eglMakeCurrent(display, surface, surface, context);
    assert(madeCurrent);

    display_ = display;
    surface_ = surface;
    context_ = context;

    // make width and height invalid so it gets updated the first frame in @a updateRenderArea()
    width_ = -1;
    height_ = -1;

    PRINT_GL_STRING(GL_VENDOR);
    PRINT_GL_STRING(GL_RENDERER);
    PRINT_GL_STRING(GL_VERSION);
    PRINT_GL_STRING_AS_LIST(GL_EXTENSIONS);

    shader_ = std::unique_ptr<Shader>(
            Shader::loadShader(vertex, fragment, "inPosition", "inUV", "uProjection"));
    assert(shader_);

    // Note: there's only one shader in this demo, so I'll activate it here. For a more complex game
    // you'll want to track the active shader and activate/deactivate it as necessary
    shader_->activate();

    // setup any other gl related global states
    glClearColor(CORNFLOWER_BLUE);

    // enable alpha globally for now, you probably don't want to do this in a game
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // get some demo models into memory
    createModels();
    initAudio();
}

void Renderer::updateRenderArea() {
    EGLint width;
    eglQuerySurface(display_, surface_, EGL_WIDTH, &width);

    EGLint height;
    eglQuerySurface(display_, surface_, EGL_HEIGHT, &height);

    if (width != width_ || height != height_) {
        width_ = width;
        height_ = height;
        glViewport(0, 0, width, height);

        // Recreate models to match the new aspect ratio (important for full-screen images)
        createModels();

        // make sure that we lazily recreate the projection matrix before we render
        shaderNeedsNewProjectionMatrix_ = true;
    }
}

/**
 * @brief Create any demo models we want for this demo.
 */
void Renderer::createModels() {
    auto assetManager = app_->activity->assetManager;
    models_.clear();

    std::string assetName;
    if (currentState_ == GameState::TITLE_SCREEN) {
        assetName = "Tela_inicial.png";
    } else if (currentState_ == GameState::CHARACTER_CREATION) {
        assetName = "Tela_Criação.png";
    } else if (currentState_ == GameState::LOAD_GAME) {
        // Use o nome exato do arquivo que você salvou nos assets
        assetName = "Tela_Continuar.png";
    }

    // Load the appropriate image based on game state
    auto spTex = TextureAsset::loadAsset(assetManager, assetName);

    /* Full Screen Quad */
    // Forçamos o quad a preencher a tela inteira ignorando a proporção da imagem original
    // para que ela estique e ocupe todo o fundo azul que você viu.
    float aspectRatio = float(width_) / height_;
    float halfWidth = aspectRatio * kProjectionHalfHeight;

    std::vector<Vertex> bgVertices = {
            Vertex(Vector3{-halfWidth, kProjectionHalfHeight, 0}, Vector2{0, 0}),
            Vertex(Vector3{halfWidth, kProjectionHalfHeight, 0}, Vector2{1, 0}),
            Vertex(Vector3{halfWidth, -kProjectionHalfHeight, 0}, Vector2{1, 1}),
            Vertex(Vector3{-halfWidth, -kProjectionHalfHeight, 0}, Vector2{0, 1})
    };
    std::vector<Index> indices = {0, 1, 2, 0, 2, 3};
    models_.emplace_back(bgVertices, indices, spTex);
}

void Renderer::changeState(GameState newState) {
    if (isFadingOut_) return; // Already transitioning

    playClickSound();

    // Start Fade Out transition
    isFadingOut_ = true;
    pendingState_ = newState;
}

void Renderer::determineInitialState() {
    JNIEnv* env;
    app_->activity->vm->AttachCurrentThread(&env, nullptr);

    jclass activityClass = env->GetObjectClass(app_->activity->javaGameActivity);
    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getNameMethod = env->GetMethodID(classClass, "getName", "()Ljava/lang/String;");
    jstring nameString = (jstring)env->CallObjectMethod(activityClass, getNameMethod);

    const char* name = env->GetStringUTFChars(nameString, nullptr);
    aout << "Started with Activity: " << name << std::endl;

    if (strstr(name, "CreationActivity")) {
        currentState_ = GameState::CHARACTER_CREATION;
    } else if (strstr(name, "LoadActivity")) {
        currentState_ = GameState::LOAD_GAME;
    } else {
        currentState_ = GameState::TITLE_SCREEN;
    }

    env->ReleaseStringUTFChars(nameString, name);
    app_->activity->vm->DetachCurrentThread();
}

void Renderer::startNativeActivity(const char* activityClassName) {
    JNIEnv* env;
    app_->activity->vm->AttachCurrentThread(&env, nullptr);

    // Get the class of the current activity
    jobject activityObj = app_->activity->javaGameActivity;
    jclass activityClass = env->GetObjectClass(activityObj);

    // Get the ClassLoader of the current activity
    jclass classClass = env->FindClass("java/lang/Class");
    jmethodID getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoaderObj = env->CallObjectMethod(activityClass, getClassLoaderMethod);

    // Get the loadClass method from the ClassLoader
    jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
    jmethodID loadClassMethod = env->GetMethodID(classLoaderClass, "loadClass", "(Ljava/lang/String;)Ljava/lang/Class;");

    // Load the target activity class using the app's ClassLoader
    jstring classNameString = env->NewStringUTF(activityClassName);
    jclass targetActivityClass = (jclass)env->CallObjectMethod(classLoaderObj, loadClassMethod, classNameString);

    if (env->ExceptionCheck()) {
        env->ExceptionDescribe();
        env->ExceptionClear();
        aout << "Failed to load class: " << activityClassName << std::endl;
        app_->activity->vm->DetachCurrentThread();
        return;
    }

    // Create the Intent
    jclass intentClass = env->FindClass("android/content/Intent");
    jmethodID intentConstructor = env->GetMethodID(intentClass, "<init>", "(Landroid/content/Context;Ljava/lang/Class;)V");
    jobject intent = env->NewObject(intentClass, intentConstructor, activityObj, targetActivityClass);

    // Start the Activity
    jclass contextClass = env->FindClass("android/content/Context");
    jmethodID startActivityMethod = env->GetMethodID(contextClass, "startActivity", "(Landroid/content/Intent;)V");
    env->CallVoidMethod(activityObj, startActivityMethod, intent);

    app_->activity->vm->DetachCurrentThread();
}

void Renderer::initAudio() {
    SLresult result;

    // create engine
    result = slCreateEngine(&engineObject_, 0, nullptr, 0, nullptr, nullptr);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*engineObject_)->Realize(engineObject_, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*engineObject_)->GetInterface(engineObject_, SL_IID_ENGINE, &engineEngine_);
    if (SL_RESULT_SUCCESS != result) return;

    // create output mix
    result = (*engineEngine_)->CreateOutputMix(engineEngine_, &outputMixObject_, 0, nullptr, nullptr);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*outputMixObject_)->Realize(outputMixObject_, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) return;

    // configure audio source (using assets)
    AAssetManager* assetManager = app_->activity->assetManager;
    AAsset* asset = AAssetManager_open(assetManager, "Cliques.wav", AASSET_MODE_UNKNOWN);
    if (asset == nullptr) return;

    off_t start, length;
    int fd = AAsset_openFileDescriptor(asset, &start, &length);
    AAsset_close(asset);

    SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
    SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
    SLDataSource audioSrc = {&loc_fd, &format_mime};

    // configure audio sink
    SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, outputMixObject_};
    SLDataSink audioSnk = {&loc_outmix, nullptr};

    // create audio player
    const SLInterfaceID ids[2] = {SL_IID_PLAY, SL_IID_SEEK};
    const SLboolean req[2] = {SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE};
    result = (*engineEngine_)->CreateAudioPlayer(engineEngine_, &clickPlayerObject_, &audioSrc, &audioSnk, 2, ids, req);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*clickPlayerObject_)->Realize(clickPlayerObject_, SL_BOOLEAN_FALSE);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*clickPlayerObject_)->GetInterface(clickPlayerObject_, SL_IID_PLAY, &clickPlayerPlay_);
    if (SL_RESULT_SUCCESS != result) return;
    result = (*clickPlayerObject_)->GetInterface(clickPlayerObject_, SL_IID_SEEK, &clickPlayerSeek_);
    if (SL_RESULT_SUCCESS != result) return;

    // --- Background Music (BGM) Configuration ---
    AAsset* bgmAsset = AAssetManager_open(assetManager, "Tema1.mp3", AASSET_MODE_UNKNOWN);
    if (bgmAsset != nullptr) {
        off_t bgmStart, bgmLength;
        int bgmFd = AAsset_openFileDescriptor(bgmAsset, &bgmStart, &bgmLength);
        AAsset_close(bgmAsset);

        SLDataLocator_AndroidFD bgmLoc_fd = {SL_DATALOCATOR_ANDROIDFD, bgmFd, bgmStart, bgmLength};
        SLDataFormat_MIME bgmFormat_mime = {SL_DATAFORMAT_MIME, nullptr, SL_CONTAINERTYPE_UNSPECIFIED};
        SLDataSource bgmSrc = {&bgmLoc_fd, &bgmFormat_mime};

        result = (*engineEngine_)->CreateAudioPlayer(engineEngine_, &bgmPlayerObject_, &bgmSrc, &audioSnk, 2, ids, req);
        if (SL_RESULT_SUCCESS == result) {
            (*bgmPlayerObject_)->Realize(bgmPlayerObject_, SL_BOOLEAN_FALSE);
            (*bgmPlayerObject_)->GetInterface(bgmPlayerObject_, SL_IID_PLAY, &bgmPlayerPlay_);
            (*bgmPlayerObject_)->GetInterface(bgmPlayerObject_, SL_IID_SEEK, &bgmPlayerSeek_);

            if (bgmPlayerPlay_ != nullptr && bgmPlayerSeek_ != nullptr) {
                // Set Looping
                (*bgmPlayerSeek_)->SetLoop(bgmPlayerSeek_, SL_BOOLEAN_TRUE, 0, SL_TIME_UNKNOWN);
                // Start Playing
                (*bgmPlayerPlay_)->SetPlayState(bgmPlayerPlay_, SL_PLAYSTATE_PLAYING);
            }
        }
    }
}

void Renderer::cleanupAudio() {
    if (bgmPlayerObject_ != nullptr) {
        (*bgmPlayerObject_)->Destroy(bgmPlayerObject_);
        bgmPlayerObject_ = nullptr;
    }
    if (clickPlayerObject_ != nullptr) {
        (*clickPlayerObject_)->Destroy(clickPlayerObject_);
        clickPlayerObject_ = nullptr;
    }
    if (outputMixObject_ != nullptr) {
        (*outputMixObject_)->Destroy(outputMixObject_);
        outputMixObject_ = nullptr;
    }
    if (engineObject_ != nullptr) {
        (*engineObject_)->Destroy(engineObject_);
        engineObject_ = nullptr;
    }
}

void Renderer::playClickSound() {
    if (clickPlayerPlay_ != nullptr && clickPlayerSeek_ != nullptr) {
        (*clickPlayerPlay_)->SetPlayState(clickPlayerPlay_, SL_PLAYSTATE_STOPPED);
        (*clickPlayerSeek_)->SetPosition(clickPlayerSeek_, 0, SL_SEEKMODE_FAST);
        (*clickPlayerPlay_)->SetPlayState(clickPlayerPlay_, SL_PLAYSTATE_PLAYING);
    }
}

void Renderer::handleInput() {
    // handle all queued inputs
    auto *inputBuffer = android_app_swap_input_buffers(app_);
    if (!inputBuffer) {
        // no inputs yet.
        return;
    }

    // handle motion events (motionEventsCounts can be 0).
    for (auto i = 0; i < inputBuffer->motionEventsCount; i++) {
        auto &motionEvent = inputBuffer->motionEvents[i];
        auto action = motionEvent.action;

        // Find the pointer index, mask and bitshift to turn it into a readable value.
        auto pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK)
                >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
        aout << "Pointer(s): ";

        // get the x and y position of this event if it is not ACTION_MOVE.
        auto &pointer = motionEvent.pointers[pointerIndex];
        auto x = GameActivityPointerAxes_getX(&pointer);
        auto y = GameActivityPointerAxes_getY(&pointer);

        // Convert screen coordinates to world coordinates
        float worldX = (x / (float)width_ * 2.0f - 1.0f) * ((float)width_ / height_ * kProjectionHalfHeight);
        float worldY = (1.0f - y / (float)height_ * 2.0f) * kProjectionHalfHeight;

        // determine the action type and process the event accordingly.
        switch (action & AMOTION_EVENT_ACTION_MASK) {
            case AMOTION_EVENT_ACTION_DOWN:
            case AMOTION_EVENT_ACTION_POINTER_DOWN: {
                aout << "Touch at World: (" << worldX << ", " << worldY << ")" << std::endl;

                // Proporções baseadas na largura da tela
                float halfWidth = ((float)width_ / height_ * kProjectionHalfHeight);

                // Zonas de toque recalibradas
                if (currentState_ == GameState::TITLE_SCREEN) {
                    // Botões no lado direito
                    if (worldX > halfWidth * 0.2f) {
                        // NOVO JOGO
                        if (worldY < 0.3f && worldY > -0.1f) {
                            aout << "Clicked: NOVO JOGO" << std::endl;
                            changeState(GameState::CHARACTER_CREATION);
                        }
                        // CONTINUAR
                        else if (worldY < -0.3f && worldY > -0.7f) {
                            aout << "Clicked: CONTINUAR" << std::endl;
                            changeState(GameState::LOAD_GAME);
                        }
                        // OPÇÕES
                        else if (worldY < -0.9f && worldY > -1.3f) {
                            aout << "Clicked: OPÇÕES" << std::endl;
                            playClickSound();
                        }
                        // SAIR
                        else if (worldY < -1.5f && worldY > -1.9f) {
                            aout << "Clicked: SAIR" << std::endl;
                            playClickSound();
                        }
                    }
                } else if (currentState_ == GameState::CHARACTER_CREATION) {
                    // --- TELA DE MONTAGEM DE PERSONAGEM ---
                    // Botão VOLTAR (Expandido e deslocado levemente para a direita para alinhar com o visual)
                    if (worldX < 0.0f && worldX > -1.5f && worldY < -1.3f) {
                        aout << "Clicked: VOLTAR" << std::endl;
                        changeState(GameState::TITLE_SCREEN);
                    }
                    // Botão ALEATÓRIO (MEIO inferior)
                    else if (worldX >= 0.0f && worldX < 1.0f && worldY < -1.3f) {
                        aout << "Clicked: ALEATÓRIO" << std::endl;
                        playClickSound();
                    }
                    // Botão CONFIRMAR (Canto inferior DIREITO)
                    else if (worldX >= 1.0f && worldY < -1.3f) {
                        aout << "Clicked: CONFIRMAR" << std::endl;
                        playClickSound();
                    }
                } else if (currentState_ == GameState::LOAD_GAME) {
                    // --- TELA DE GRIMÓRIO DE MEMÓRIAS ---
                    // Botão VOLTAR (Canto inferior DIREITO nesta tela específica)
                    if (worldX > halfWidth - 2.5f && worldY < -1.4f) {
                        aout << "Clicked: VOLTAR" << std::endl;
                        changeState(GameState::TITLE_SCREEN);
                    }
                }
                break;
            }

            case AMOTION_EVENT_ACTION_CANCEL:
                // treat the CANCEL as an UP event: doing nothing in the app, except
                // removing the pointer from the cache if pointers are locally saved.
                // code pass through on purpose.
            case AMOTION_EVENT_ACTION_UP:
            case AMOTION_EVENT_ACTION_POINTER_UP:
                aout << "(" << pointer.id << ", " << x << ", " << y << ") "
                     << "Pointer Up";
                break;

            case AMOTION_EVENT_ACTION_MOVE:
                // There is no pointer index for ACTION_MOVE, only a snapshot of
                // all active pointers; app needs to cache previous active pointers
                // to figure out which ones are actually moved.
                for (auto index = 0; index < motionEvent.pointerCount; index++) {
                    pointer = motionEvent.pointers[index];
                    x = GameActivityPointerAxes_getX(&pointer);
                    y = GameActivityPointerAxes_getY(&pointer);
                    aout << "(" << pointer.id << ", " << x << ", " << y << ")";

                    if (index != (motionEvent.pointerCount - 1)) aout << ",";
                    aout << " ";
                }
                aout << "Pointer Move";
                break;
            default:
                aout << "Unknown MotionEvent Action: " << action;
        }
        aout << std::endl;
    }
    // clear the motion input count in this buffer for main thread to re-use.
    android_app_clear_motion_events(inputBuffer);

    // handle input key events.
    for (auto i = 0; i < inputBuffer->keyEventsCount; i++) {
        auto &keyEvent = inputBuffer->keyEvents[i];
        aout << "Key: " << keyEvent.keyCode <<" ";
        switch (keyEvent.action) {
            case AKEY_EVENT_ACTION_DOWN:
                aout << "Key Down";
                break;
            case AKEY_EVENT_ACTION_UP:
                aout << "Key Up";
                break;
            case AKEY_EVENT_ACTION_MULTIPLE:
                // Deprecated since Android API level 29.
                aout << "Multiple Key Actions";
                break;
            default:
                aout << "Unknown KeyEvent Action: " << keyEvent.action;
        }
        aout << std::endl;
    }
    // clear the key input count too.
    android_app_clear_key_events(inputBuffer);
}