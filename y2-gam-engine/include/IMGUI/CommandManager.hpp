#pragma once
#include <pch.hpp>


class CommandManager {
private:
	struct CommandBlock {
		std::string command;
		std::vector<std::any> params;

		template <typename... _args>
		CommandBlock(std::string const& cmd, _args... args) : command{ cmd } {
			// Unpack the parameter pack and emplace each argument into the vector
			(params.push_back(std::any(std::forward<Args>(args))), ...);
		}
	};
public:
	static std::shared_ptr<CommandManager> GetInstance() {
		if (!_mSelf) return _mSelf = std::make_shared<CommandManager>();
		return _mSelf;
	}
	template <typename... _args>
	void AddCommand(std::string const& cmd, _args... args) {
		mCommandQueue.push(CommandBlock{cmd, args...})
	}
	inline void UndoCommand(){
	}

private:
	static std::shared_ptr<CommandManager> _mSelf;
	std::queue<CommandBlock> mCommandQueue;
};