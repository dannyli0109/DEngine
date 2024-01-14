#pragma once
#include <vector>
#include "graphicsInclude.h"
#include "texture.h"

namespace DEngine
{
    enum class TextureKey
    {
        white = 0,
        wall = 1
    };

    class ResourceManager
    {
    public:
        static ResourceManager *getInstance()
        {
            if (!instance)
            {
                instance = new ResourceManager();
            }
            return instance;
        }
        void addTexture(Texture *texture)
        {
            textures.push_back(texture);
        }

        Texture *getTexture(int index)
        {
            if (index < 0 || index > textures.size() - 1)
                return nullptr;
            return textures[index];
        }

        static void destroy()
        {
            for (int i = 0; i < instance->textures.size(); i++)
            {
                delete instance->textures[i];
            }
            instance->textures.clear();
            delete instance;
            instance = nullptr;
        }

    private:
        ResourceManager(){};

#pragma region Rule of Five
        ~ResourceManager(){};
        ResourceManager(const ResourceManager &other) = delete;
        ResourceManager &operator=(const ResourceManager &other) = delete;
        ResourceManager(const ResourceManager &&other) = delete;
        ResourceManager &operator=(const ResourceManager &&other) = delete;
#pragma endregion

        static ResourceManager *instance;
        std::vector<Texture *> textures;
    };
    ResourceManager *ResourceManager::instance = nullptr;
} // namespace DEngine