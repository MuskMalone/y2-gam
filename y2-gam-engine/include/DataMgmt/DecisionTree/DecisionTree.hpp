#pragma once

enum class DecisionResults {
    IMGUI_MODE,
    GAME_MODE,
    ATTACK,
    TELEPORT,
    PATROL,
    NONE
};

using DecisionFunction = std::function<bool()>;
using DecisionCB = std::function<void()>;

struct DecisionTreeNode {
    int nodeId;
    DecisionFunction decisionFunction;
    DecisionCB actionCallback;
    std::vector<std::unique_ptr<DecisionTreeNode>> children;

    DecisionTreeNode(int id);
};

class DecisionTree {
private:
    std::unique_ptr<DecisionTreeNode> rootNode;
    DecisionResults currentMode = DecisionResults::GAME_MODE;

    bool IsInGameMode() const;
    void ToggleGameMode();
    void SetGameMode();
    void SetImGuiMode();

public:
    DecisionTree();
    DecisionTreeNode* CreateRootNode(int id);
    DecisionTreeNode* AddNode(DecisionTreeNode* parent, int id);
    void Evaluate(DecisionTreeNode* node);
    void Evaluate();
    void CheckToggleKey();
    void BuildDecisionTree();
};


//#pragma once
//#include <pch.hpp>
//#include "Systems/InputSystem.hpp"
//namespace {
//    std::shared_ptr<Coordinator> gCoordinator{};
//}
//enum class DecisionResults {
//    IMGUI_MODE,
//    GAME_MODE,
//    ATTACK,
//    TELEPORT,
//    PATROL,
//    NONE
//};
//
//
//class DecisionTree;
//using DecisionFunction = std::function<bool()>;
//using DecisionCB = std::function<void()>;
//
//struct DecisionTreeNode {
//    int nodeId;
//    DecisionFunction decisionFunction;
//    DecisionCB actionCallback;
//    std::vector<std::unique_ptr<DecisionTreeNode>> children;
//
//    DecisionTreeNode(int id) : nodeId(id) {}
//};
//
//class DecisionTree {
//private:
//    std::unique_ptr<DecisionTreeNode> rootNode;
//    DecisionResults currentMode = DecisionResults::GAME_MODE;
//
//    bool IsInGameMode() const {
//        return currentMode == DecisionResults::GAME_MODE;
//    }
//
//    void ToggleGameMode() {
//        if (currentMode == DecisionResults::GAME_MODE) {
//            SetImGuiMode();
//        }
//        else {
//            SetGameMode();
//        }
//    }
//
//    void SetGameMode() {
//        currentMode = DecisionResults::GAME_MODE;
//        std::cout << "Game mode set!" << std::endl;
//    }
//
//    void SetImGuiMode() {
//        currentMode = DecisionResults::IMGUI_MODE;
//        std::cout << "ImGui mode set!" << std::endl;
//    }
//
//public:
//    DecisionTree() {
//        BuildDecisionTree();
//    }
//
//    DecisionTreeNode* CreateRootNode(int id) {
//        rootNode = std::make_unique<DecisionTreeNode>(id);
//        return rootNode.get();
//    }
//
//    DecisionTreeNode* AddNode(DecisionTreeNode* parent, int id) {
//        auto newNode = std::make_unique<DecisionTreeNode>(id);
//        parent->children.push_back(std::move(newNode));
//        return parent->children.back().get();
//    }
//
//    void Evaluate(DecisionTreeNode* node) {
//        if (node->actionCallback) {
//            node->actionCallback();
//            return;
//        }
//
//        for (const auto& child : node->children) {
//            if (child->decisionFunction && child->decisionFunction()) {
//                Evaluate(child.get());
//                break;
//            }
//        }
//    }
//
//    void Evaluate() {
//        Evaluate(rootNode.get());
//    }
//    void CheckToggleKey() {
//        auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
//        if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_M)) {
//            ToggleGameMode();
//        }
//    }
//
//    void BuildDecisionTree() {
//        auto root = CreateRootNode(1);
//        root->decisionFunction = [this]() -> bool {
//            return IsInGameMode();
//        };
//
//        auto childNodeForGame = AddNode(root, 2);
//        childNodeForGame->actionCallback = [this]() { SetGameMode(); };
//
//        auto childNodeForImGui = AddNode(root, 3);
//        childNodeForImGui->actionCallback = [this]() { SetImGuiMode(); };
//    }
//};