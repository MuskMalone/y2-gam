/******************************************************************************/
/*!
\par        Image Engine
\file       DecisionTree.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Oct 20, 2023

\brief      Header file for DecisionTree

            This file contains the declaration of a tree-based 
            decision structure in the game engine. It includes methods to create 
            and evaluate the decision tree nodes. It also provides utilities for 
            the different nodes.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once

enum class DecisionResults {
    IMGUI_MODE,
    GAME_MODE,
    IMGUI_PLAY_MODE,
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
    DecisionResults currentMode = DecisionResults::IMGUI_MODE;

    bool IsInGameMode() const;
    bool IsInImGuiPlayMode() const;
    void ToggleGameMode();
    void SetGameMode();
    void SetImGuiMode();
    void SetImGuiPlayMode();

public:
    DecisionTree();
    DecisionTreeNode* CreateRootNode(int id);
    DecisionTreeNode* AddNode(DecisionTreeNode* parent, int id);
    DecisionResults GetCurrentMode() const;
    void Evaluate(DecisionTreeNode* node);
    void Evaluate();
    void CheckToggleKey();
    void ToggleImGuiMode();
    void BuildDecisionTree();
};
