#include "Imgui/ImguiApp.hpp"

Entity selectedEntity = MAX_ENTITIES;

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}
namespace Image {
 
	void AppRender(){
        

        //ImGui::ShowDemoWindow();
       HierarchyWindow();
       InspectorWindow();
	}
    //std::vector<std::string> entities = { "Entity1", "Entity2", "Entity3" };
    //int selectedEntityIndex = -1; // -1 means no entity is selected
    //char entityNameBuffer[256]; // Buffer to hold the name for renaming
	void HierarchyWindow(){
        // Hierarchy Panel
        ::gCoordinator = Coordinator::GetInstance();
        ImGui::CollapsingHeader("Hierarchy");

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

            //ImGui::end();
        
	}
	void InspectorWindow(){
        // Inspector Panel
        ImGui::Begin("Inspector");

        if (selectedEntity != MAX_ENTITIES) {
            //TransformComponent if the entity has one
            if (gCoordinator->GetComponentType<Transform>()) {
                Transform& transform = gCoordinator->GetComponent<Transform>(selectedEntity);
                // Position
                ImGui::Text("Position");
                ImGui::InputFloat("X", &transform.position.x);
                ImGui::InputFloat("Y", &transform.position.y);
                ImGui::InputFloat("Z", &transform.position.z);

                // Rotation
                ImGui::Text("Rotation");
                ImGui::InputFloat("Rot X", &transform.rotation.x);
                ImGui::InputFloat("Rot Y", &transform.rotation.y);
                ImGui::InputFloat("Rot Z", &transform.rotation.z);

                // Scale
                ImGui::Text("Scale");
                ImGui::InputFloat("Scale X", &transform.scale.x);
                ImGui::InputFloat("Scale Y", &transform.scale.y);
                ImGui::InputFloat("Scale Z", &transform.scale.z);

                if (ImGui::Button("Remove Transform Component")) {
                    gCoordinator->RemoveComponent<Transform>(selectedEntity);
                }
            }
            if (gCoordinator->GetComponentType<Sprite>()) {
                Sprite& sprite = gCoordinator->GetComponent<Sprite>(selectedEntity);
                //Color
                ImGui::Text("Color");
                ImGui::ColorPicker4("Color Picker", &sprite.color.r);
                //Tex
                ImGui::Text("Texture");
                ImGui::Text("Add asset file path i think");
                if (ImGui::Button("Remove Sprite Component")) {
                    gCoordinator->RemoveComponent<Sprite>(selectedEntity);
                }
                else {
                    if (ImGui::Button("Add Sprite Component")) {
                        Sprite newSprite{};
                        gCoordinator->AddComponent(selectedEntity, newSprite);
                    }
                }
            }
            

            // ... Repeat for other component types
        }

        ImGui::End();
	}
    void LoggingWindow(){

    }
}
