#ifndef ANDROIDGLINVESTIGATIONS_RENDERER_H
#define ANDROIDGLINVESTIGATIONS_RENDERER_H

#include <EGL/egl.h>
#include <memory>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include <chrono>

#include "Model.h"
#include "Shader.h"

struct android_app;

class Renderer {
public:
    /*!
     * @param pApp the android_app this Renderer belongs to, needed to configure GL
     */
    inline Renderer(android_app *pApp) :
            app_(pApp),
            display_(EGL_NO_DISPLAY),
            surface_(EGL_NO_SURFACE),
            context_(EGL_NO_CONTEXT),
            width_(0),
            height_(0),
            shaderNeedsNewProjectionMatrix_(true) {
        determineInitialState();
        initRenderer();
    }

    virtual ~Renderer();

    /*!
     * Handles input from the android_app.
     *
     * Note: this will clear the input queue
     */
    void handleInput();

    /*!
     * Renders all the models in the renderer
     */
    void render();

private:
    /*!
     * Performs necessary OpenGL initialization. Customize this if you want to change your EGL
     * context or application-wide settings.
     */
    void initRenderer();

    /*!
     * @brief we have to check every frame to see if the framebuffer has changed in size. If it has,
     * update the viewport accordingly
     */
    void updateRenderArea();

    /*!
     * Creates the models for this sample. You'd likely load a scene configuration from a file or
     * use some other setup logic in your full game.
     */
    void createModels();

    enum class GameState {
        TITLE_SCREEN,
        CHARACTER_CREATION,
        LOAD_GAME
    };

    void changeState(GameState newState);

    void playClickSound();

    void determineInitialState();
    void startNativeActivity(const char* activityClassName);

    android_app *app_;
    EGLDisplay display_;
    EGLSurface surface_;
    EGLContext context_;
    EGLint width_;
    EGLint height_;

    GameState currentState_ = GameState::TITLE_SCREEN;

    // Fade transition members
    float fadeAlpha_ = 1.0f; // 1.0 = opaque, 0.0 = transparent
    bool isFadingOut_ = false;
    GameState pendingState_ = GameState::TITLE_SCREEN;
    std::chrono::time_point<std::chrono::steady_clock> lastFrameTime_;

    // Audio members
    SLObjectItf engineObject_ = nullptr;
    SLEngineItf engineEngine_ = nullptr;
    SLObjectItf outputMixObject_ = nullptr;
    SLObjectItf clickPlayerObject_ = nullptr;
    SLPlayItf clickPlayerPlay_ = nullptr;
    SLSeekItf clickPlayerSeek_ = nullptr;

    SLObjectItf bgmPlayerObject_ = nullptr;
    SLPlayItf bgmPlayerPlay_ = nullptr;
    SLSeekItf bgmPlayerSeek_ = nullptr;

    void initAudio();
    void cleanupAudio();

    bool shaderNeedsNewProjectionMatrix_;

    std::unique_ptr<Shader> shader_;
    std::vector<Model> models_;
};

#endif //ANDROIDGLINVESTIGATIONS_RENDERER_H