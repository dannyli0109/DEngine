#pragma once
#include "graphics_include.h"

namespace DEngine
{
    enum CameraType
    {
        Camera2D,
        Camera3D
    } CameraType;

    typedef struct Camera
    {
        glm::vec3 position;
        float aspectRatio;
        union
        {
            struct Camera2D
            {
                float zoom;
            } camera2D;

            struct Camera3D
            {
                glm::vec3 rotation;
                float fieldOfView;
                float nearClip;
                float farClip;
            } camera3D;
        };

        enum CameraType type;
    };

    Camera *createCamera2D(glm::vec2 position, float zoom, glm::vec2 windowSize)
    {
        Camera *camera = (Camera *)malloc(sizeof(Camera));
        camera->type = CameraType::Camera2D;
        camera->position = glm::vec3(position, 10.0f);
        camera->aspectRatio = windowSize.x / windowSize.y;
        camera->camera2D.zoom = zoom;
        return camera;
    }

    glm::mat4 getProjectionMatrix(Camera *camera)
    {
        switch (camera->type)
        {
        case CameraType::Camera2D:
        {
            float left = -camera->aspectRatio * camera->camera2D.zoom;
            float right = camera->aspectRatio * camera->camera2D.zoom;
            float bottom = -camera->camera2D.zoom;
            float top = camera->camera2D.zoom;
            return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        }
        case CameraType::Camera3D:
        {
            return glm::perspective(glm::radians(camera->camera3D.fieldOfView), camera->aspectRatio, camera->camera3D.nearClip, camera->camera3D.farClip);
        }
        }
    }

}