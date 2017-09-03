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

	// �������㑶�݂��Ă���֐��ɔ�Ԏ��Ԃ����炷(�����������I)
	inline static T *createInstance() { return new T(); }

	// �Ԃ��Ă��
	inline static T &getReference(const singleton_pointer_type &ptr) { return *ptr; }

protected:
	Singleton() {}

private:
	// �R�s�[���֎~����
	Singleton(const Singleton &) = delete;
	Singleton& operator=(const Singleton &) = delete;
	Singleton(Singleton &&) = delete;
	Singleton& operator=(Singleton &&) = delete;
};

