/******************************************************************************/
/*!
\par        Image Engine
\file       DecisionTree.cpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Oct 20, 2023

\brief      Implementation file for DecisionTree

            This file contains the implementation of a tree-based 
            decision structure in the game engine. It includes methods to create 
            and evaluate the decision tree nodes. It also provides utilities for 
            the different nodes.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/

#include "../include/pch.hpp"
#include "DataMgmt/DecisionTree/DecisionTree.hpp"
#include <iostream>
#include <Core/Coordinator.hpp>
#include "Systems/InputSystem.hpp"
#include <Core/Globals.hpp>
#include "Systems/ImguiSystem.hpp"

namespace {
    std::shared_ptr<Coordinator> gCoordinator;
}
/*  _________________________________________________________________________ */
/*! DecisionTreeNode

@param id
The unique identifier for the node.

@return none.

Creates a new DecisionTreeNode with the specified ID.
*/

DecisionTreeNode::DecisionTreeNode(int id) : nodeId{ id } {}

/*  _________________________________________________________________________ */
/*! IsInGameMode

@param none.

@return bool
Returns true if in game mode, false otherwise.

Checks if the system is currently in the game mode.
*/
bool DecisionTree::IsInGameMode() const {
    return currentMode == DecisionResults::GAME_MODE;
}

/*  _________________________________________________________________________ */
/*! ToggleGameMode

@param none.

@return none.

Switches the mode based on the current mode. If currently in game mode, it
switches to ImGui mode and vice-versa.
*/
void DecisionTree::ToggleGameMode() {
    if (IsInGameMode()) {
        SetImGuiMode();
    }
    else {
        SetGameMode();
    }
}

/*  _________________________________________________________________________ */
/*! SetGameMode

@param none.

@return none.

Sets the current mode to GAME_MODE and outputs a notification to the console.
*/
void DecisionTree::SetGameMode() {
    currentMode = DecisionResults::GAME_MODE;
    std::cout << "Game mode set!" << std::endl;
}

/*  _________________________________________________________________________ */
/*! SetImGuiMode

@param none.

@return none.

Sets the current mode to IMGUI_MODE.
*/
void DecisionTree::SetImGuiMode() {
    currentMode = DecisionResults::IMGUI_MODE;
}

/*  _________________________________________________________________________ */
/*! DecisionTree

@param none.

@return none.

Default constructor for DecisionTree, calls BuildDecisionTree function to
initialize the tree structure.
*/
DecisionTree::DecisionTree() {

    BuildDecisionTree();
}

/*  _________________________________________________________________________ */
/*! CreateRootNode

@param id
The unique identifier for the node.

@return DecisionTreeNode*.
Returns a pointer to the created root node.

Creates and sets the root node for the decision tree using the provided ID.
*/
DecisionTreeNode* DecisionTree::CreateRootNode(int id) {
    rootNode = std::make_unique<DecisionTreeNode>(id);
    return rootNode.get();
}

/*  _________________________________________________________________________ */
/*! AddNode

@param parent
A pointer to the parent node.

@param id
The unique identifier for the new node.

@return DecisionTreeNode*.
Returns a pointer to the newly added node.

Adds a new child node to the provided parent node with the given ID.
*/
DecisionTreeNode* DecisionTree::AddNode(DecisionTreeNode* parent, int id) {
    auto newNode = std::make_unique<DecisionTreeNode>(id);
    parent->children.push_back(std::move(newNode));
    return parent->children.back().get();
}

/*  _________________________________________________________________________ */
/*! GetCurrentMode

@param none.

@return DecisionResults.
Returns the current operational mode (GAME_MODE or IMGUI_MODE).

Provides the current mode of the system.
*/
DecisionResults DecisionTree::GetCurrentMode() const {
    return currentMode;
}

/*  _________________________________________________________________________ */
/*! Evaluate 

@param node
A pointer to the DecisionTreeNode being evaluated.

@return none.

Recursively evaluates the provided node and its children based on the decision
functions and action callbacks associated with each node.
*/
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

/*  _________________________________________________________________________ */
/*! Evaluate (overloaded)

@param none.

@return none.

Initiates the evaluation process from the root node of the decision tree.
*/
void DecisionTree::Evaluate() {
    Evaluate(rootNode.get());
}

/*  _________________________________________________________________________ */
/*! CheckToggleKey

@return none.

Checks if the toggle key (n) has been pressed. If so, it toggles the current game mode.
*/
void DecisionTree::CheckToggleKey() {
    ::gCoordinator = Coordinator::GetInstance();
    auto inputSystem = ::gCoordinator->GetSystem<InputSystem>();
    if (inputSystem->CheckKey(InputSystem::InputKeyState::KEY_CLICKED, GLFW_KEY_N)) {
        ToggleGameMode();
    }
}


/*  _________________________________________________________________________ */
/*! BuildDecisionTree

@param none.

@return none.

Sets up the structure of the decision tree by creating nodes and establishing
their relationships.
*/
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
