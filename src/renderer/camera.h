#pragma once
// C++
#include "graphicsInclude.h"

namespace DEngine
{

    class Camera
    {
    public:
        glm::vec3 position;
        float aspectRatio;

        virtual ~Camera()
        {
        }

        virtual glm::mat4 getViewMatrix() const = 0;
        virtual glm::mat4 getProjectionMatrix() const = 0;
        void setWindowSize(glm::vec2 windowSize)
        {
            aspectRatio = windowSize.x / windowSize.y;
        }
    };

    class Camera2D : public Camera
    {
    public:
        float zoomLevel;

        Camera2D(glm::vec2 position, float zoom, glm::vec2 windowSize)
        {
            this->position = glm::vec3(position, 10.0f);
            this->zoomLevel = zoom;
            this->aspectRatio = windowSize.x / windowSize.y;
        }

        glm::mat4 getViewMatrix() const override
        {
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::translate(view, -position);
            return view;
        }

        glm::mat4 getProjectionMatrix() const override
        {
            float left = -aspectRatio * zoomLevel;
            float right = aspectRatio * zoomLevel;
            float bottom = -zoomLevel;
            float top = zoomLevel;
            return glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
        }
    };

    class Camera3D : public Camera
    {
    public:
        glm::vec3 rotation;
        float fieldOfView;
        float nearClip;
        float farClip;

        Camera3D() : rotation(0.0f, 0.0f, 0.0f),
                     fieldOfView(45.0f),
                     nearClip(0.1f),
                     farClip(100.0f) {}

        glm::mat4 getViewMatrix() const override
        {
            glm::mat4 view = glm::mat4(1.0f);
            view = glm::rotate(view, glm::radians(rotation.x), glm::vec3(1, 0, 0));
            view = glm::rotate(view, glm::radians(rotation.y), glm::vec3(0, 1, 0));
            view = glm::rotate(view, glm::radians(rotation.z), glm::vec3(0, 0, 1));
            view = glm::translate(view, -position);
            return view;
        }

        glm::mat4 getProjectionMatrix() const override
        {
            return glm::perspective(glm::radians(fieldOfView), aspectRatio, nearClip, farClip);
        }
    };
}