#include "Imgui/ImguiApp.hpp"

Entity selectedEntity = MAX_ENTITIES;
constexpr float gPI{ 3.141592653589793238462643383279502884197169399375105f };

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}
namespace Image {
 
	void AppRender(){
        

        static bool showDockSpace{ false };
        //Press z to change dock space and non dock space
        if (ImGui::IsKeyReleased(ImGuiKey_Z)) {
            showDockSpace = !showDockSpace;
        }

        if (showDockSpace) {
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        }
        //ImGui::ShowDemoWindow();
        MainMenuWindow();
        HierarchyWindow();
        InspectorWindow();
        PropertyWindow();
        BufferWindow();
       
	}
    void MainMenuWindow() {
        if (ImGui::BeginMainMenuBar())
        {
            //FILE
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        if (ImGui::BeginMenu("Recurse.."))
                        {

                            ImGui::EndMenu();
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As..")) {}

                ImGui::Separator();

                if (ImGui::BeginMenu("Options"))
                {
                    static bool enabled = true;
                    ImGui::MenuItem("Enabled", "", &enabled);
                    ImGui::BeginChild("child", ImVec2(0, 60), true);
                    for (int i = 0; i < 10; i++)
                        ImGui::Text("Scrolling Text %d", i);
                    ImGui::EndChild();
                    static float f = 0.5f;
                    static int n = 0;
                    ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                    ImGui::InputFloat("Input", &f, 0.1f);
                    ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                    ImGui::EndMenu();
                }


                //if (ImGui::BeginMenu("Colors"))
                //{
                //    float sz = ImGui::GetTextLineHeight();
                //    for (int i = 0; i < ImGuiCol_COUNT; i++)
                //    {
                //        const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                //        ImVec2 p = ImGui::GetCursorScreenPos();
                //        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                //        ImGui::Dummy(ImVec2(sz, sz));
                //        ImGui::SameLine();
                //        ImGui::MenuItem(name);
                //    }
                //    ImGui::EndMenu();
                //}
                ImGui::EndMenu();
            }

            //EDIT
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }
    //std::vector<std::string> entities = { "Entity1", "Entity2", "Entity3" };
    //int selectedEntityIndex = -1; // -1 means no entity is selected
    //char entityNameBuffer[256]; // Buffer to hold the name for renaming
    void HierarchyWindow() {
        // Hierarchy Panel
        ::gCoordinator = Coordinator::GetInstance();
        ImGui::Begin("Hierarchy");

        for (Entity entity{ 0 }; entity < gCoordinator->GetEntityCount(); ++entity) {
            bool isSelected = (selectedEntity == entity);
            if (ImGui::Selectable(std::to_string(entity).c_str(), isSelected)) {
                selectedEntity = entity;
            }
        }


        if (ImGui::Button("Create Entity")) {
            Entity newEntity = gCoordinator->CreateEntity();
            ImGuiViewport* vP = ImGui::GetWindowViewport();
            
            gCoordinator->AddComponent(
                newEntity,
                Transform{
                    .position = Vec3(vP->Pos.x,vP->Pos.y,0),
                    .rotation = Vec3(0,0,0),
                    .scale = Vec3(5,5,5)
                });
            gCoordinator->AddComponent(
                newEntity,
                Sprite{
                    .color = Vec4(1,0,0, 1),
                    .texture = nullptr
                });
            selectedEntity = newEntity;
            //Renderer::DrawLine();

        }

        if (selectedEntity != MAX_ENTITIES && ImGui::Button("Destroy Entity")) {
            gCoordinator->DestroyEntity(selectedEntity);
            selectedEntity = MAX_ENTITIES;
        }

        ImGui::End();

    }
    void InspectorWindow() {
        // Inspector Panel
        ImGui::Begin("Inspector");

        if (selectedEntity != MAX_ENTITIES) {
            //TransformComponent if the entity has one
            if (gCoordinator->GetComponentType<Transform>()) {
                Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);
                // Position
                ImGui::Text("Position");
                ImGui::SliderFloat("X", &transform.position.x, -ENGINE_SCREEN_WIDTH / 2, ENGINE_SCREEN_WIDTH / 2);
                ImGui::SliderFloat("Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT / 2, ENGINE_SCREEN_HEIGHT / 2);
                //ImGui::InputFloat("Z", &transform.position.z);

                // Rotation
                ImGui::Text("Rotation");
                //ImGui::SliderFloat("Rot X", &transform.rotation.x, -2*gPI, 2*gPI);
                //ImGui::SliderFloat("Rot Y", &transform.rotation.y, -2 * gPI, 2 * gPI);
                ImGui::SliderFloat("Rot Z", &transform.rotation.z, -180, 180); // change to Degree(gPI) 

                // Scale
                ImGui::Text("Scale");
                ImGui::SliderFloat("Scale X", &transform.scale.x, 1, 50);
                ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, 50);
                //ImGui::InputFloat("Scale Z", &transform.scale.z);

                if (ImGui::Button("Remove Transform Component")) {
                    gCoordinator->RemoveComponent<Transform>(selectedEntity);
                }
            }
            if (gCoordinator->GetComponentType<Sprite>()) {
                Sprite& sprite = gCoordinator->GetComponent<Sprite>(selectedEntity);
                //Color

                ImGui::Text("\nColor");
                ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                //Tex
                ImGui::Text("\nTexture");
                ImGui::Text("Add asset file path i think");
                if (ImGui::Button("Remove Sprite Component")) {
                    gCoordinator->RemoveComponent<Sprite>(selectedEntity);
                }
                /*else {
                    if (ImGui::Button("Add Sprite Component")) {
                        Sprite newSprite{};
                        gCoordinator->AddComponent(selectedEntity, newSprite);
                    }
                }*/
            }
        }

        ImGui::End();
    }
    void PropertyWindow() {
        ImGui::Begin("Property");
        if (selectedEntity != MAX_ENTITIES) {
            ImGui::Text("Entity ID: %d", selectedEntity);
            //ImGui::Text("Transform Component: %s", gCoordinator->GetComponent<Transform>(selectedEntity)?"True":"False");
            //ImGui::Text("Sprite Component: %s", gCoordinator->GetComponent<Sprite>(selectedEntity) ? "True" : "False");
            //ImGui::Text("Gravity Component: %s", gCoordinator->GetComponent<Gravity>(selectedEntity) ? "True" : "False");
            //ImGui::Text("Collsion Component: %s", gCoordinator->GetComponent<BoxCollider>(selectedEntity) ? "True" : "False");
            //ImGui::Text("Animation Component: %s", gCoordinator->GetComponent<Animation>(selectedEntity) ? "True" : "False");
            //ImGui::Text("Camera Component: %s", gCoordinator->GetComponent<Camera>(selectedEntity) ? "True" : "False");
            //ImGui::Text("OrthoCamera Component: %s", gCoordinator->GetComponent<OrthoCamera>(selectedEntity) ? "True" : "False");
            //ImGui::Text("RigidBody Component: %s", gCoordinator->GetComponent<RigidBody>(selectedEntity) ? "True" : "False");
        }
        ImGui::End();
    }
    void BufferWindow() {
        ImGui::Begin("Image Game Engine");

        unsigned int texHdl = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer()->GetColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(1040, 585), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
    }
    void LoggingWindow() {

    }
}
