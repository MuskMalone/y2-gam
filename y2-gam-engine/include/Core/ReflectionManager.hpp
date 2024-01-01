#pragma once
#include <pch.hpp>
class ReflectionManager{
public:
    template <typename _return, typename ... _pack>
    static _return InvokeClassFunction(std::string const& classname, std::string const& funcname, _pack&&... params) {
        // Retrieve the class type
        rttr::type classType = rttr::type::get_by_name(classname);
        if (!classType.is_valid()) {
            throw std::runtime_error("Class not found: " + classname);
        }

        // Retrieve the method
        rttr::method method = classType.get_method(funcname);
        if (!method.is_valid()) {
            throw std::runtime_error("Method not found: " + funcname);
        }

        // Invoke the method
        rttr::variant result = method.invoke(rttr::instance(), std::forward<_pack>(params)...);
        if (!result.is_valid()) {
            throw std::runtime_error("Method invocation failed");
        }
        //std::cout << result.get_type().get_name() << std::endl;
        // Convert the result
        if (!result.can_convert<_return>()) {
            throw std::runtime_error("Return type mismatch");
        }
        return result.get_value<_return>();
    }

    //caveat: class must have a registered function in TypeReflection.hpp called GetInstance for its GetInstance function
    template <typename _return, typename ... _pack>
    static _return InvokeSingletonClassFunction(std::string const& classname, std::string const& funcname, _pack&&... params) {
        // Retrieve the class type
        rttr::type classType = rttr::type::get_by_name(classname);
        if (!classType.is_valid()) {
            throw std::runtime_error("Class not found: " + classname);
        }

        // Retrieve GetInstance
        rttr::method instanceMethod = classType.get_method("GetInstance");
        if (!instanceMethod.is_valid()) {
            throw std::runtime_error("No GetInstance available");
        }

        // Retrieve the method
        rttr::method method = classType.get_method(funcname);
        if (!method.is_valid()) {
            throw std::runtime_error("Method not found: " + funcname);
        }

        rttr::variant singletonInstance = instanceMethod.invoke({});
        if (!singletonInstance.is_valid()) {
            throw std::runtime_error("Singleton invocation failed");
        }

        // Invoke the method
        rttr::variant result = method.invoke(singletonInstance, std::forward<_pack>(params)...);
        if (!result.is_valid()) {
            throw std::runtime_error("Method invocation failed");
        }
        //std::cout << result.get_type().get_name() << std::endl;
        // Convert the result
        if (!result.can_convert<_return>()) {
            throw std::runtime_error("Return type mismatch");
        }
        return result.get_value<_return>();
    }
};