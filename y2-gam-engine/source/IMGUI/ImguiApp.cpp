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
            //gCoordinator->AddComponent(newEntity, ImguiComponent{});
            gSelectedEntity = newEntity;
        }

        if (gSelectedEntity != MAX_ENTITIES && ImGui::Button("Destroy Entity")) {
            gCoordinator->DestroyEntity(gSelectedEntity);
            gSelectedEntity = MAX_ENTITIES;
        }

        for (auto const& entity : mEntities) {
            bool isSelected = (gSelectedEntity == entity);
            if (ImGui::Selectable(std::to_string(entity).c_str(), isSelected)) {
                gSelectedEntity = entity;
            }
        }
        //std::cout << "Number of entities: " << mEntities.size() << std::endl;

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
                ImGui::SliderFloat("Pos X", &transform.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                ImGui::SliderFloat("Pos Y", &transform.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);

                // Rotation
                ImGui::Text("Rotation");
                ImGui::SliderFloat("Rot Z", &transform.rotation.z, -180, 180); // change to Degree(gPI) same as glm func in math ultiles

                // Scale
                ImGui::Text("Scale");
                ImGui::SliderFloat("Scale X", &transform.scale.x, 1, 50);
                ImGui::SliderFloat("Scale Y", &transform.scale.y, 1, 50);
                //if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                //    RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(gSelectedEntity);
                //    rigidBody.position = Vec3{ transform.position.x,transform.position.y,0 };
                //    rigidBody.rotation =transform.rotation.z;
                //    rigidBody.dimension = Vec3{ transform.scale.x,transform.scale.y,0 };
                //}

      /*          if (ImGui::Button("Remove Transform Component")) {
                    gCoordinator->RemoveComponent<Transform>(gSelectedEntity);
                }*/
            }
            if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                Sprite& sprite = gCoordinator->GetComponent<Sprite>(gSelectedEntity);
                //Color
                ImGui::Separator();
                ImGui::Text("Color");
                ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                //Tex
                //ImGui::Separator();
                /*ImGui::Text("Sprite");*/
                //ImGui::Text("Add asset file path i think");
                //if (ImGui::Button("Remove Sprite Component")) {
                //    gCoordinator->RemoveComponent<Sprite>(gSelectedEntity);
                //}
            }
            if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                RigidBody& rigidBody = gCoordinator->GetComponent<RigidBody>(gSelectedEntity);
                
                ImGui::Separator();
                ImGui::Text("RigidBody");
                //Pos
                ImGui::Text("Position");
                ImGui::SliderFloat("Pos X", &rigidBody.position.x, -ENGINE_SCREEN_WIDTH / 4.f, ENGINE_SCREEN_WIDTH / 4.f);
                ImGui::SliderFloat("Pos Y", &rigidBody.position.y, -ENGINE_SCREEN_HEIGHT / 4.f, ENGINE_SCREEN_HEIGHT / 4.f);
                // Rotation
                ImGui::Text("Rotation");
                ImGui::SliderFloat("Rot Z", &rigidBody.rotation, -180, 180); // change to Degree(gPI) same as glm func in math ultiles
                // Scale
                ImGui::Text("Dimension");
                ImGui::SliderFloat("Scale X", &rigidBody.dimension.x, 1, 50);
                ImGui::SliderFloat("Scale Y", &rigidBody.dimension.y, 1, 50);
                // Mass
                ImGui::Text("Mass");
                ImGui::InputFloat("Mass", &rigidBody.mass);
                rigidBody.SetMass(rigidBody.mass);

                //if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                //    Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                //    transform.position = Vec3{ rigidBody.position.x,rigidBody.position.y,0 };
                //    transform.rotation = Vec3{ 0,0,rigidBody.rotation };
                //    transform.scale = Vec3{ rigidBody.dimension.x,rigidBody.dimension.y,0 };
                //}
            }

            //if (gCoordinator->HasComponent<BoxCollider>(gSelectedEntity)) {
            //}
            //if (gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
            //}
            if (gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                Gravity& gravity = gCoordinator->GetComponent<Gravity>(gSelectedEntity);
                ImGui::Separator();
                //Force
                ImGui::Text("Gravity");
                ImGui::SliderFloat("Force X", &gravity.force.x, -10, 10);
                ImGui::SliderFloat("Force Y", &gravity.force.y, -10, 10);
            }
        }
        ImGui::End();
    }
    void PropertyWindow() {
        ImGui::Begin("Property");
        const char* components[] = { "Transform", "Sprite", "RigidBody", "Collision","Animation","Gravity"};
        static int selectedComponentToAdd{ -1 };
        static int selectedComponentToRemove{ -1 };
        if (gSelectedEntity != MAX_ENTITIES) {
            ImGui::Text("Entity ID: %d", gSelectedEntity);
            //Combo box click to add components
            if (ImGui::Combo("Add Component", &selectedComponentToAdd, components, IM_ARRAYSIZE(components))) {
                switch (selectedComponentToAdd) {
                case 0: {
                    if (!gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                        ImGuiViewport* vP = ImGui::GetWindowViewport();
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Transform{
                                {vP->Pos.x,vP->Pos.y,0},
                                {0,0,0},
                                {5,5,5}
                            });
                    }
                }
                    break;
                case 1: {
                    if (!gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Sprite{
                                {1,0,0, 1},
                                nullptr
                            });
                    }
                }
                      break;
                case 2: {
                    if (!gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                        if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                            Transform& transform = gCoordinator->GetComponent<Transform>(gSelectedEntity);
                            gCoordinator->AddComponent(
                                gSelectedEntity,
                                RigidBody{
                                    Vec2{transform.position.x,transform.position.y},
                                    transform.rotation.z,
                                    50.f,
                                    Vec2{transform.scale.x,transform.scale.y}
                                });
                        }
                    }
                }
                    break;
                case 3: {
                    if (!gCoordinator->HasComponent<BoxCollider>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            BoxCollider{});
                    }
                    
                }
                      break;
                case 4: {
                    if (!gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
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
                }
                      break;
                case 5: {
                    if (!gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                        gCoordinator->AddComponent(
                            gSelectedEntity,
                            Gravity{ Vec2{0.f,0.f} });
                    }
                }
                    break;
                }
            }
            if (ImGui::Combo("Remove Component", &selectedComponentToRemove, components, IM_ARRAYSIZE(components))) {
                switch (selectedComponentToRemove) {
                case 0: {
                    // Remove Transform component
                    if (gCoordinator->HasComponent<Transform>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Transform>(gSelectedEntity);
                    }
                }
                    break;
                case 1: {
                    // Remove Sprite component
                    if (gCoordinator->HasComponent<Sprite>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Sprite>(gSelectedEntity);
                    }
                }
                    break;
                case 2: {
                    // Remove RigidBody component
                    if (gCoordinator->HasComponent<RigidBody>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<RigidBody>(gSelectedEntity);
                    }
                }
                    break;
                case 3: {
                    // Remove BoxCollider component
                    if (gCoordinator->HasComponent<BoxCollider>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<BoxCollider>(gSelectedEntity);
                    }
                }
                    break;
                case 4: {
                    // Remove Animation component
                    if (gCoordinator->HasComponent<Animation>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Animation>(gSelectedEntity);
                    }
                }
                    break;
                case 5: {
                    // Remove Gravity component
                    if (gCoordinator->HasComponent<Gravity>(gSelectedEntity)) {
                        gCoordinator->RemoveComponent<Gravity>(gSelectedEntity);
                    }
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