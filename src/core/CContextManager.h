#pragma once
#include "utils/CSingleton.h"
#include "RmlUi/Core.h"

class CContextManager : public CSingleton<CContextManager>
{
	Rml::Context* context;
public:
	void Create();
	Rml::Context* GetContext();
};