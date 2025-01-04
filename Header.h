#pragma once
#include"Macros.h"
#include<type_traits>
#include<iostream>
#include<utility>
_PANAGIOTIS_BEGIN
template<class _Ty>
class stack {
private:
	class stack_node {
	public:
		stack_node* next;
		_Ty data;
#if _HAS_CXX20
		template<typename t=_Ty>
		requires(std::is_default_constructible_v<t>)
		stack_node() :data{}, next{ nullptr } {

		}
#endif
		stack_node(_Ty data1) :data{ std::move(data1) }, next{nullptr}
		{

		}
	};
	size_t size1;
	stack_node* head;
	stack_node* ptr;

public:
#if _HAS_CXX17
	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>,"no references allowed");
	static_assert(std::is_copy_constructible_v<_Ty>, "this stack doesnt take non copy constructible types");
	static_assert(!std::is_const_v<std::remove_reference_t<_Ty>>, "no const types are allowed");
	static_assert(!std::is_volatile_v<std::remove_reference_t<_Ty>>, "no volatile types are allowed");
#else
	static_assert(std::is_object<_Ty>::value, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference<_Ty>::value, "no references allowed");
	static_assert(std::is_copy_constructible<_Ty>::value, "this stack doesnt take non copy constructible types");
	static_assert(!std::is_const<std::remove_reference_t<_Ty>>::value, "no const types are allowed");
	static_assert(!std::is_volatile<std::remove_reference_t<_Ty>>::value, "no volatile types are allowed");
#endif

	stack() :head{ nullptr }, ptr{ nullptr }, size1{ 0 }
	{

	}
	stack(const stack& right) {
		if (this != &right) {
			const stack_node* ptr1 = right.head;
			for (size_t i = 0; i < right.size1; i++) {
				push_back(ptr1->data);

				ptr1 = ptr1->next;

			}
			size1 = right.size1;
			if (size1 == 0) {//empty object to empty object
				head = nullptr;
				ptr = nullptr;
			}
		}
		else {
			head = nullptr;
			size1 = 0;
			ptr = nullptr;
		}
	}
	stack(stack&& right)noexcept(noexcept(std::exchange(head, right.head)) && noexcept(std::exchange(ptr, right.ptr))) {
		if (this != &right) {
			head=std::exchange(right.head,nullptr);
			ptr=std::exchange(right.ptr, nullptr);
			size1 = std::exchange(right.size1, 0);
			return;
		}
		else {
			head = nullptr;
			size1 = 0;
			ptr = nullptr;
		}
	}

	
	void push(const _Ty&data1)noexcept(std::is_nothrow_copy_constructible_v<_Ty>&& std::is_nothrow_copy_assignable_v<_Ty >) {
		if (size1 == 0) {
			size1++;
			
			head = new (std::nothrow) stack_node(data1);
			if (head == nullptr) {
				return;
			}
			ptr = head;
		}
		else {
			size1++;
			ptr->next = new (std::nothrow)stack_node(data1);
			if (ptr->next == nullptr) { return; };
			ptr = ptr->next;
		}
	}
	void push(_Ty&& data1)noexcept(std::is_nothrow_move_constructible_v<_Ty>&& std::is_nothrow_move_assignable_v<_Ty>) {
		if (size1 == 0) {
			size1++;
			head = new(std::nothrow) stack_node(std::move(data1));

			if (head == nullptr) { return; };
			ptr = head;


		}
		else {
			size1++;
			ptr->next = new(std::nothrow) stack_node(std::move(data1));
			if (ptr->next == nullptr) { return; };
			ptr = ptr->next;
		}
	}
    _NODISCARD  size_t size()const noexcept {
		return size1;
	}
	_NODISCARD   bool empty()const noexcept {
		if (head == nullptr)return true;
		return false;
	}
	_NODISCARD _Ty& top() {
		if (!empty()) {
			return ptr->data;
		}
		
		throw bad_stack_access{ "access an empty stack" };

	}
	
	void pop() {
		if (size1 > 1) {
			delete ptr;
			size1--;
			ptr = head;
			for (size_t i = 0; i < size1 - 1; i++) {
				ptr = ptr->next;
			}
			 ptr->next = nullptr;
			return;
		}
		else if (size1 == 1) {
			size1 = 0;
			ptr = nullptr;
			delete head;
			head = nullptr;
			return;
		}
		throw pop_from_empty_stack("tried to pop an element from an empty stack");

	}

	template <class... _Valty>
	void emplace(_Valty&&... _Val)noexcept(std::is_nothrow_copy_constructible_v<_Ty>&& std::is_nothrow_copy_assignable_v<_Ty >&& std::is_nothrow_move_constructible_v<_Ty>&& std::is_nothrow_move_assignable_v<_Ty>) {
		if (size1 == 0) {
			size1++;

			head = new (std::nothrow) stack_node(std::forward<_Valty>(_Val)...);
			if (head == nullptr) {
				return;
			}
			ptr = head;
		}
		else {
			size1++;
			ptr->next = new (std::nothrow)stack_node(std::forward<_Valty>(_Val)...);
			if (ptr->next == nullptr) { return; };
			ptr = ptr->next;
		}
	}

	void swap(stack &right)noexcept(noexcept(std::exchange(head, right.head))&&noexcept(std::exchange(ptr, right.ptr))) {
		
		right.head=std::exchange(head, right.head);
		right.ptr=std::exchange(ptr, right.ptr);
		size_t temp = size1;
		size1 = right.size1;
		right.size1 = temp;
		return;
	}
	~stack() noexcept{

		stack_node* ptr1 = head;
		stack_node* ptr2 = head;
		for (size_t i = 0; i < size1; i++) {
			ptr2 = ptr2->next;
			delete ptr1;
			ptr1 = ptr2;


		}
		size1 = 0;
		head = nullptr;
		ptr = nullptr;
		return;

	}



};


_PANAGIOTIS_END