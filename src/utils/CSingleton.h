#pragma once

template<typename T>
class CSingleton
{
protected:
	CSingleton() = default;
	virtual ~CSingleton() = default;

public:
	static inline T* Get()
	{
		static T s;
		return &s;
	}
};
