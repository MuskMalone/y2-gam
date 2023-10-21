/******************************************************************************/
/*!
\par        Image Engine
\file       TagComponent.hpp

\author     Ng Meng Yee, Darren (ng.m@digipen.edu)
\date       Oct 13, 2023

\brief      Header file for Tag component

            This file sets up a TagComponent to be used for the ECS system.
            It allows entities to be tagged with a name.

\copyright  Copyright (C) 2023 DigiPen Institute of Technology. Reproduction
            or disclosure of this file or its contents without the prior
            written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************************/
#pragma once
#include <string>
#include <rapidjson/document.h>
#include <Core/Serialization/SerializationManager.hpp>

struct Tag {
    std::string tag;

    Tag() = default;
    Tag(std::string const& name) : tag{ name } {}
    Tag(rapidjson::Value const& obj) {
        tag = obj["Tag"].GetString();
    }
    /*  _________________________________________________________________________ */
    /*! Serialize

    @param none.

    @return bool
    returns true

    This function is inserts values into the serializer
    */
    bool Serialize(rapidjson::Value& obj) {
        std::shared_ptr<Serializer::SerializationManager> sm = Serializer::SerializationManager::GetInstance();
        sm->InsertValue(obj, "Tag", tag);
        return true;
    }
};