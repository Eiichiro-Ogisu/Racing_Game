#pragma once

#include <memory>

template <class T>
class Singleton
{
public:
	static T& singleton()
	{
		static typename T::singleton_pointer_type s_singleton(T::createInstance());

		return getReference(s_singleton);
	}

private:
	typedef std::unique_ptr<T> singleton_pointer_type;

	// メモリ上存在している関数に飛ぶ時間を減らす(処理が早い！)
	inline static T *createInstance() { return new T(); }

	// 返してるよ
	inline static T &getReference(const singleton_pointer_type &ptr) { return *ptr; }

protected:
	Singleton() {}

private:
	// コピーを禁止する
	Singleton(const Singleton &) = delete;
	Singleton& operator=(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton& operator=(Singleton &&) = delete;
};

