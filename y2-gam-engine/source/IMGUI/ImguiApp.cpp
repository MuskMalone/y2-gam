#include "Imgui/ImguiApp.hpp"

Entity gSelectedEntity=MAX_ENTITIES;
constexpr float gPI{ 3.141592653589793238462643383279502884197169399375105f };

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}
namespace Image {

    void AppRender(std::set<Entity>const& mEntities) {
        static bool showDockSpace{ true };
        //Press z to change dock space and non dock space
        if (ImGui::IsKeyReleased(ImGuiKey_Z)) {
            showDockSpace = !showDockSpace;
        }

        if (showDockSpace) {
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        }
        //ImGui::ShowDemoWindow();
        MainMenuWindow();
        HierarchyWindow(mEntities);
        InspectorWindow();
        PropertyWindow();
        BufferWindow();
        LoggingWindow();

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
    void HierarchyWindow(std::set<Entity>const& mEntities) {
        // Hierarchy Panel
        ::gCoordinator = Coordinator::GetInstance();
        ImGui::Begin("Hierarchy");
        //Create entity and destory first
        if (ImGui::Button("Create Entity")) {
            Entity newEntity = gCoordinator->CreateEntity();
        }

        if (gSelectedEntity != MAX_ENTITIES && ImGui::Button("Destroy Entity")) {
            gCoordinator->DestroyEntity(gSelectedEntity);
            gSelectedEntity = MAX_ENTITIES;
        }


        //for (auto const& entity : mEntities) {
        //    bool isSelected = (selectedEntity == entity);
        //    if (ImGui::Selectable(std::to_string(entity).c_str(), isSelected)) {
        //        selectedEntity = entity;
        //    }
        //}
        std::cout << "Number of entities: " << mEntities.size() << std::endl;

        for (Entity entity{4  }; entity < gCoordinator->GetEntityCount(); ++entity) {
            bool isSelected = (gSelectedEntity == entity);
            if (ImGui::Selectable(std::to_string(entity).c_str(), isSelected)) {
                gSelectedEntity = entity;
            }
        }
        ImGui::End();
    }
    void InspectorWindow() {
        // Inspector Panel
        ImGui::Begin("Inspector");
        //TransformComponent
        if (gSelectedEntity != MAX_ENTITIES) {
            if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                // Position
                ImGui::Text("Position");
                ImGui::SliderFloat("X", &transform.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                ImGui::SliderFloat("Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);

                // Rotation
                ImGui::Text("Rotation");
                ImGui::SliderFloat("Rot Z", &transform.rotation.z, -180, 180); // change to Degree(gPI) same as glm func in math ultiles

                // Scale
                ImGui::Text("Scale");
                ImGui::SliderFloat("Scale X", &transform.scale.x, 1, 50);
                ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, 50);

                if (ImGui::Button("Remove Transform Component")) {
                    gCoordinator->RemoveComponent<Transform>(gSelectedEntity);
                }
            }
            if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                Sprite& sprite = gCoordinator->GetComponent<Sprite>(gSelectedEntity);
                //Color
                ImGui::Separator();
                ImGui::Text("Color");
                ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                //Tex
                ImGui::Separator();
                ImGui::Text("Sprite");
                ImGui::Text("Add asset file path i think");
                if (ImGui::Button("Remove Sprite Component")) {
                    gCoordinator->RemoveComponent<Sprite>(gSelectedEntity);
                }
            }
        }
        ImGui::End();
    }
    void PropertyWindow() {
        ImGui::Begin("Property");
        const char* components[] = { "Transform", "Sprite", "RigidBody", "Collision","Animation","Gravity"};
        static int selectedComponent{-1};

        if (gSelectedEntity != MAX_ENTITIES) {
            ImGui::Text("Entity ID: %d", gSelectedEntity);
            //Combo box click to add components
            if (ImGui::Combo("Add Component", &selectedComponent, components, IM_ARRAYSIZE(components))) {
                switch (selectedComponent) {
                case 0:{
                    ImGuiViewport* vP = ImGui::GetWindowViewport();
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Transform{
                            Vec3(vP->Pos.x,vP->Pos.y,0),
                            Vec3(0,0,0),
                            Vec3(5,5,5)
                        });
                    break;
                }
                case 1: {
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Sprite{
                            Vec4(1,0,0, 1),
                            nullptr
                        });
                }
                      break;
                case 2: {
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        RigidBody{});
                    break;
                }
                case 3: {
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        BoxCollider{});
                }
                      break;
                case 4: {
                    //------------TEMPORARY TO BE READ FROM JSON FILES------------------------------------------------------------------/
                    std::vector<AnimationFrame> idleFrames{ {0.f, 0}, {0.f, 1}, { 0.f, 2 }, { 0.f, 3 }, { 0.f, 4 }, { 0.f, 5 }, { 0.f, 6 }, { 0.f, 7} };
                    std::vector<AnimationFrame> runFrames{ {0.f, 8}, {0.f, 9}, { 0.f, 10 }, { 0.f, 11 }, { 0.f, 12 }, { 0.f, 13 }, { 0.f, 14 }, { 0.f, 15 } };
                    std::vector<AnimationFrame> attackFrames{ {0.f, 16}, {0.f, 17}, { 0.f, 18 }, { 0.f, 19 }, { 0.f, 20 }, { 0.f, 21 }, { 0.f, 22 } };
                    std::unordered_map<ANIM_STATE, std::vector<AnimationFrame>> map{ {ANIM_STATE::IDLE, idleFrames},
                                                                                     {ANIM_STATE::RUN, runFrames},
                                                                                     {ANIM_STATE::ATTACK, attackFrames} };
                    ::gCoordinator->AddComponent(
                        gSelectedEntity,
                        Animation{
                            0.08f,
                            0,
                            ANIM_STATE::IDLE,
                            map
                        });
                }
                      break;
                case 5: {
                    gCoordinator->AddComponent(
                        gSelectedEntity,
                        Gravity{});
                }
                    break;
                }
            }
            ImGui::Text("Transform Component: %s", gCoordinator->HasComponent<Transform>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Sprite Component: %s", gCoordinator->HasComponent<Sprite>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("RigidBody Component: %s", gCoordinator->HasComponent<RigidBody>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Collsion Component: %s", gCoordinator->HasComponent<BoxCollider>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Animation Component: %s", gCoordinator->HasComponent<Animation>(gSelectedEntity) ? "True" : "False");
            ImGui::Text("Gravity Component: %s", gCoordinator->HasComponent<Gravity>(gSelectedEntity) ? "True" : "False");
        }
        ImGui::End();
    }
    void BufferWindow() {
        ImGui::Begin("Image Game Engine");

        unsigned int texHdl = ::gCoordinator->GetSystem<RenderSystem>()->GetFramebuffer()->GetColorAttachmentID();
        ImGui::Image(reinterpret_cast<void*>(static_cast<uintptr_t>(texHdl)), ImVec2(ENGINE_SCREEN_WIDTH / 1.5f, ENGINE_SCREEN_HEIGHT / 1.5f), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
        ImGui::End();
    }
    void LoggingWindow() {
        ImGui::Begin("Log");
        ImGui::End();
    }
}