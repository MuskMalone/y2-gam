#include "../include/pch.hpp"
#include <Core/Coordinator.hpp>

std::shared_ptr<Coordinator> Coordinator::_mSelf = 0;

std::shared_ptr<Coordinator> Coordinator::GetInstance() {
	if (!_mSelf) return _mSelf = std::make_shared<Coordinator>();
	return _mSelf;
}