
#pragma once

#include"Macros.h"
#include<type_traits>
#include<initializer_list>
#include<iostream>
#include<utility>

#if __cplusplus > 202002L
_PANAGIOTIS_BEGIN

template<class _Ty>
class stack final{
private:

	class stack_node {
	public:
		stack_node* next;
		_Ty data;

		template<typename t = _Ty>
		requires(std::is_default_constructible_v<t>)
		stack_node() :data{}, next{ nullptr } {

		}
		template<typename t = _Ty>
		stack_node(_Ty data1) : next{ nullptr }
		{
			if constexpr (std::is_move_assignable_v<decltype(data1)>) {

				data = std::move(data1);
				return;
			}
			else if constexpr (std::is_copy_assignable_v<decltype(data1)>) {

				data = data1;
				return;
			}

		}


	};
	size_t size1 = 0;
	stack_node* head;
	stack_node* ptr;

	inline constexpr static size_t max_size1 = { 4611686018427387903 };

public:

	static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
		"because of [container.requirements].");
	static_assert(!std::is_reference_v<_Ty>, "no references allowed");
	static_assert(std::is_copy_constructible_v<_Ty>, "this stack doesnt take non copy constructible types");
	static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
	static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");


	stack() noexcept:head{ nullptr }, ptr{ nullptr }, size1{ 0 }
	{

	}
	stack(const std::initializer_list<_Ty>& a)noexcept :head { nullptr }, ptr{ nullptr }, size1{0} {
		const _Ty* b = a.begin();
		for (std::size_t i = 0; i < a.size(); i++) {
			push(*b);
			b++;
		}
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
			head = std::exchange(right.head, nullptr);
			ptr = std::exchange(right.ptr, nullptr);
			size1 = std::exchange(right.size1, 0);
			return;
		}
		else {
			head = nullptr;
			size1 = 0;
			ptr = nullptr;
		}
	}


	void push(const _Ty& data1)/*noexcept(std::is_nothrow_copy_constructible_v<_Ty>&& std::is_nothrow_copy_assignable_v<_Ty >)*/ {
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
	void push(_Ty&& data1)/*noexcept(std::is_nothrow_move_constructible_v<_Ty>&& std::is_nothrow_move_assignable_v<_Ty>)*/ {
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
	_NODISCARD   size_t size()const noexcept {
		return size1;
	}
	_NODISCARD   bool empty()const noexcept {
		if (head == nullptr)return true;
		return false;
	}
	_NODISCARD _Ty& top()const {
		if (!empty()) {
			return ptr->data;
		}

		throw bad_stack_access_{ "access an empty stack" };

	}

	_NODISCARD _CONSTEXPR20 size_t max_size() const noexcept {
		return max_size1;
	}

	stack& operator =(const stack& right)& {
		
		if (this != &right) {
			if (this->head != nullptr) {
				if (right.head == nullptr) {
					this->~stack();
					return *this;
				}
				this->~stack();
				const stack_node* ptr1 = right.head;
				for (size_t i = 0; i < right.size1; i++) {
					push(ptr1->data);
					ptr1 = ptr1->next;
				}
				size1 = right.size1;
			}
			else {
				const stack_node* ptr1 = right.head;
				for (size_t i = 0; i < right.size1; i++) {
					push(ptr1->data);

					ptr1 = ptr1->next;
				}
				size1 = right.size1;
			}


		}
		return *this;
	}
	stack& operator =(stack&& right) & noexcept(noexcept(std::exchange(head, right.head)) && noexcept(std::exchange(ptr, right.ptr))) {
		
		if (this != &right) {
			if (this->head != nullptr) {
				if (right.head == nullptr) {
					this->~stack();
					return *this;
				}
				this->~stack();



				head = std::exchange(right.head, nullptr);
				ptr = std::exchange(right.ptr, nullptr);
				size1 = std::exchange(right.size1, 0);
			}
			else {

				if (right.head == nullptr) {
					return *this;
				}

				head = std::exchange(right.head, nullptr);
				ptr = std::exchange(right.ptr, nullptr);
				size1 = std::exchange(right.size1, 0);

			}


		}
		else {
			this->~stack();
		}
		return *this;
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
		throw pop_from_empty_stack_("tried to pop an element from an empty stack");

	}

	template <class... _Valty>
	void emplace(_Valty&&... _Val) {
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
	void swap(stack& right)noexcept(noexcept(std::exchange(head, right.head)) && noexcept(std::exchange(ptr, right.ptr))) {

		right.head = std::exchange(head, right.head);
		right.ptr = std::exchange(ptr, right.ptr);
		size_t temp = size1;
		size1 = right.size1;
		right.size1 = temp;
		return;
	}


	~stack() noexcept {

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

/*#pragma once
#include"Header1.h"
#include<iostream>
#include<type_traits>
#include<initializer_list>
#include<utility>
#include<new>
_PANAGIOTIS_BEGIN
	template<typename _Ty>
	class Stack final {
	private:
		class Stack_Node final{
		public:
			_Ty data;
			Stack_Node* next;

			template<typename t=_Ty>
			requires(std::is_nothrow_default_constructible_v<_Ty>)
			Stack_Node() noexcept : data{}, next{}
			{
				//this constructos should not throw 
			}
			Stack_Node(const _Ty& item) noexcept(noexcept(data=item))
				:data{item},next{nullptr}
			{
				
			}
			Stack_Node(_Ty&& item) noexcept(noexcept(data =std::move( item)))
				:data{std::move( item) }, next{ nullptr }
			{

			}
			
		};
		std::size_t count;
		Stack_Node* head;
		
	public:
		static_assert(std::is_object_v<_Ty>, "The C++ Standard forbids container adaptors of non-object types "
			"because of [container.requirements].");
		static_assert(!std::is_reference_v<_Ty>, "no references allowed");
		static_assert(!std::is_const_v<_Ty>, "no const types are allowed");
		static_assert(!std::is_volatile_v<_Ty>, "no volatile types are allowed");
		Stack() noexcept:  head { nullptr }, count{0}
		{

		}//default contructor
		Stack(const std::initializer_list<_Ty>& l):  head { nullptr }, count{ 0 }
		{
			//to do 
		}
		

		//push func has O(1) complexity 
		void push(const _Ty &data)//if it fails nothing changes in the stack
		{
			if (count == 0) {
				head = new(std::nothrow) Stack_Node(data);
				if (head != nullptr) {
					count++;
				}
			}
			else {
				
				head = new(std::nothrow) Stack_Node(data);
				if (head != nullptr) {
					Stack_Node* ptr = head;
					count++;
					head->next = ptr;
				}
			}
		}
		void push(_Ty&& data) {
			if (count == 0) {
				head = new(std::nothrow) Stack_Node(std::move(data));
				if (head != nullptr) {
					count++;
				}
			}
			else {

				head = new(std::nothrow) Stack_Node(std::move(data));
				if (head != nullptr) {
					Stack_Node* ptr = head;
					count++;
					head->next = ptr;
				}
			}
		}
		//push func end
		
		//the size of the stack 
		std::size_t size() {
			return count;
		}
		//if the stack is empty
		bool isEmpty() {
			return count==0;
		}
		

		
};
_PANAGIOTIS_END
*/
#endif
