#pragma once
#include <pch.hpp>
#include <Core/Coordinator.hpp>
#include <Core/Serialization/Serializer.hpp>
class CommandManager {
private:
	using ParamPack = std::vector<std::any>;
	struct CommandBlock {
		std::string command;
		ParamPack params;

		//im just dicking around lol
		template <typename... _args>
		CommandBlock(std::string const& cmd, _args&&... args) : command{ cmd } {
			// Unpack the parameter pack and emplace each argument into the vector
			(params.push_back(std::any(std::forward<_args>(args))), ...);
		}
	};
public:
	static std::shared_ptr<CommandManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<CommandManager>();
		return _mSelf;
	}
	template <typename... _args>
	void AddCommand(std::string const& cmd, _args... args) {
		mCommandStack.push_back(CommandBlock{ cmd, args... });
	}
	inline void UndoCommand(){
		if (mCommandStack.empty()) return;
		(mCommandLookup.at(mCommandStack.back().command))(mCommandStack.back().params);
		mCommandStack.pop_back();
	}

private:
	static std::shared_ptr<CommandManager> _mSelf;
	std::deque<CommandBlock> mCommandStack;
	//tch: todo replace with some reflection
	const std::map<std::string, std::function<void(ParamPack const&)>> mCommandLookup{
		{ "Transform", [&](ParamPack const& pp) {
			Entity e{ std::any_cast<Entity>(pp[0]) };
			Transform t{ std::any_cast<Transform>(pp[1]) };
			if (pp.size()>2 && Coordinator::GetInstance()->HasComponent<Collider>(e)) {
				Coordinator::GetInstance()->GetComponent<Collider>(e) = std::any_cast<Collider>(pp[2]);
			}
			Coordinator::GetInstance()->GetComponent<Transform>(e) = t;
		} },
		{ "Create" , [&](ParamPack const& pp) {
			Entity e{ std::any_cast<Entity>(pp[0]) };
			Coordinator::GetInstance()->DestroyEntity(e);
			
		} },
		{ "Destroy", [&](ParamPack const& pp) {
			Entity e{ std::any_cast<Entity>(pp[0]) };
			std::map<std::string, std::any> c{ std::any_cast<std::map<std::string, std::any>>(pp[1])};
			//this updates all instances of the deleted entity in the stack to be our current entity

			Entity ne{ Serializer::UndestroyEntity(c) };
			for (CommandBlock& element : mCommandStack) {
				if (std::any_cast<Entity>(element.params[0]) == e) {
					element.params[0] = std::any{ ne };
				}
			}
		} },
	};
};