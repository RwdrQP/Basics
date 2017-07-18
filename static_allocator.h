#ifndef _STATIC_ALLOCATOR_H_
#define _STATIC_ALLOCATOR_H_

#include <vector>
#include <stdint.h>
#include <assert.h>

#define THREAD_SAFE

#ifdef THREAD_SAFE
#include <mutex>
#endif

template<typename T>
class StaticAllocator
{
public:
	StaticAllocator(uint32_t size = 1024)
		:objects_(1024), size_(size)
	{
		uint32_t i = 0;
		uint32_t n = objects_.size();
		for(; i < n; ++i)
		{
			freeobjects_.push_back(i);
		}
	}
	~StaticAllocator(){};
	T* get()
	{
#ifdef THREAD_SAFE
		mutex_.lock();
#endif
		assert(!freeobjects_.empty());
		uint32_t pos = *(freeobjects_.rbegin());
		freeobjects_.pop_back();
#ifdef THREAD_SAFE
		mutex_.unlock();
#endif
		return objects_.data() + pos * sizeof(T);
	}
	void free(T* obj)
	{
#ifdef THREAD_SAFE
		mutex_.lock();
#endif
		assert(obj);
		uint32_t pos = (obj - objects_.data()) / sizeof(T);
		freeobjects_.push_back(pos);
#ifdef THREAD_SAFE
		mutex_.unlock();
#endif
	}
private:
	uint32_t size_;
	std::vector<T>  		objects_;
	std::vector<uint32_t>	freeobjects_;
	std::mutex				mutex_;
};

#endif //_STATIC_ALLOCATOR_H_