#include "../include/pch.hpp"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include <iostream>
#include <Core/Coordinator.hpp>
#include "Systems/InputSystem.hpp"
#include <Core/Globals.hpp>

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}

DecisionTreeNode::DecisionTreeNode(int id) : nodeId(id) {}

bool DecisionTree::IsInGameMode() const {
    return currentMode == DecisionResults::GAME_MODE;
}

void DecisionTree::ToggleGameMode() {
    if (IsInGameMode()) {
        SetImGuiMode();
    }
    else {
        SetGameMode();
    }
}

void DecisionTree::SetGameMode() {
    currentMode = DecisionResults::GAME_MODE;
    std::cout << "Game mode set!" << std::endl;
}

void DecisionTree::SetImGuiMode() {
    currentMode = DecisionResults::IMGUI_MODE;
    std::cout << "ImGui mode set!" << std::endl;
}

DecisionTree::DecisionTree() {

    BuildDecisionTree();
}

DecisionTreeNode* DecisionTree::CreateRootNode(int id) {
    rootNode = std::make_unique<DecisionTreeNode>(id);
    return rootNode.get();
}

DecisionTreeNode* DecisionTree::AddNode(DecisionTreeNode* parent, int id) {
    auto newNode = std::make_unique<DecisionTreeNode>(id);
    parent->children.push_back(std::move(newNode));
    return parent->children.back().get();
}

void DecisionTree::Evaluate(DecisionTreeNode* node) {
    if (node->actionCallback) {
        node->actionCallback();
        return;
    }

    for (const auto& child : node->children) {
        if (child->decisionFunction && child->decisionFunction()) {
            Evaluate(child.get());
            break;
        }
    }
}

void DecisionTree::Evaluate() {
    Evaluate(rootNode.get());
}

void DecisionTree::CheckToggleKey() {
    ::gCoordinator = Coordinator::GetInstance();
    auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
    if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M)) {
        ToggleGameMode();
        std::cout << "hi";
    }
}

void DecisionTree::BuildDecisionTree() {
    auto root = CreateRootNode(1);
    root->decisionFunction = [this]() -> bool {
        return IsInGameMode();
    };

    auto childNodeForGame = AddNode(root, 2);
    childNodeForGame->actionCallback = [this]() { SetGameMode(); };

    auto childNodeForImGui = AddNode(root, 3);
    childNodeForImGui->actionCallback = [this]() { SetImGuiMode(); };
}
