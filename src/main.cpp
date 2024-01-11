#include "editor/editor.h"

int main()
{
    DEngine::Editor editor;
    editor.setImGuiRenderLogic(
        []()
        {
            ImGui::Begin("Hello, world!");
            ImGui::Button("Look at this pretty button");
            ImGui::End();
        });
    editor.mainLoop();

    return 0;
}
